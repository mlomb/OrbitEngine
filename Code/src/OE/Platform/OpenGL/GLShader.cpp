#include "OE/Platform/OpenGL/GLShader.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"
#include "OE/Platform/OpenGL/GLContext.hpp"
#include "OE/Platform/OpenGL/GLStates.hpp"
#include "OE/Graphics/API/VertexLayout.hpp"

namespace OrbitEngine {	namespace Graphics {
	GLShader::GLShader()
	{
		m_ID = OE_CHECK_GL(glCreateProgram());
	}

	GLShader::~GLShader()
	{
		OE_CHECK_GL(glDeleteProgram(m_ID));
	}

	void GLShader::attachFromSource(ShaderType type, const std::string& source)
	{
		GLenum shaderType;

		switch (type) {
		case ShaderType::VERTEX:
			shaderType = GL_VERTEX_SHADER;
			break;
		case ShaderType::FRAGMENT:
			shaderType = GL_FRAGMENT_SHADER;
			break;
#if OE_OPENGL
		case ShaderType::GEOMETRY:
			shaderType = GL_GEOMETRY_SHADER;
			break;
		case ShaderType::TESS_CONTROL:
			shaderType = GL_TESS_CONTROL_SHADER;
			break;
		case ShaderType::TESS_EVAL:
			shaderType = GL_TESS_EVALUATION_SHADER;
			break;
#endif
		}

		const char* rfile = source.c_str();
		GLuint sid = OE_CHECK_GL(glCreateShader(shaderType));
		GLint error;

		OE_CHECK_GL(glShaderSource(sid, 1, &rfile, 0));
		OE_CHECK_GL(glCompileShader(sid));

		OE_CHECK_GL(glGetShaderiv(sid, GL_COMPILE_STATUS, &error));
		if (error == GL_FALSE) { // shouldn't be GL_TRUE?
			GLint logLength;
			OE_CHECK_GL(glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &logLength));
			GLchar* texterror = new GLchar[logLength + 1];
			OE_CHECK_GL(glGetShaderInfoLog(sid, logLength, &logLength, &texterror[0]));
			OE_LOG_FATAL("Error compiling GLSL shader: " << texterror);
			return;
		}

		OE_CHECK_GL(glAttachShader(m_ID, sid));
		OE_CHECK_GL(glDeleteShader(sid));
	}

	void GLShader::finalize()
	{
		OE_CHECK_GL(glLinkProgram(m_ID));

		GLint link_status = GL_FALSE;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &link_status);

		if (link_status == GL_FALSE) {
			GLint logLength;
			OE_CHECK_GL(glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &logLength));
			GLchar* texterror = new GLchar[logLength + 1];
			glGetProgramInfoLog(m_ID, logLength, &logLength, &texterror[0]);
			OE_LOG_FATAL("Error linking GLSL shaders: " << texterror);
			return;
		}

		OE_CHECK_GL(glValidateProgram(m_ID));
		
		// This causes the shader to compile asynchronously :/
		bind();

		// Bind the UBOs automatically
		if (Application::priv::GLContext::GetCurrent()->getInfo().ubo_support) {
			for (auto& pair : p_Reflections) {
				for (ShaderBuffer& buff : pair.second.buffers) {
					buff.slot = OE_CHECK_GL(glGetUniformBlockIndex(m_ID, buff.name.c_str()));
					if (buff.slot == 0xFFFFFFFFu /* GL_INVALID_INDEX */) {
						OE_LOG_WARNING("GL_INVALID_INDEX received when querying the UBO index for " << buff.name);
					}
					else {
						// For convenience, we set bind each UBO so its index match the binding point
						bindUBO(buff.slot, buff.slot);
					}
				}
			}
		}

		// Bind resources automatically
		for (auto& pair : p_Reflections) {
			int index = 0;
			for (ShaderResource& res : pair.second.resources) {
				GLint loc = getUniformLocation(res.name.c_str());
				if (res.referenced && loc == -1) {
					OE_LOG_WARNING("Texture is being referenced and querying the location got -1.");
				}
				else if (!res.referenced && loc != -1) {
					OE_LOG_WARNING("Texture is NOT being referenced and querying the location got " << loc << ".");
				}
				else {
					glUniform1i(loc, index);
				}
				index++;
			}
		}
	}
	
	void GLShader::bind() const
	{
		GLStates* states = static_cast<GLStates*>(Application::priv::GLContext::GetCurrent()->getGlobalStates());
		if (states->cache(0x8677 /* GL_PROGRAM_BINDING_ARB */, m_ID))
			return;

		OE_CHECK_GL(glUseProgram(m_ID));
	}

	void GLShader::attachFromBinary(ShaderType type, const std::vector<char>& binary)
	{
		OE_LOG_FATAL("This isn't supported in OpenGL");
	}

	GLint GLShader::getUniformLocation(const std::string& name)
	{
		auto it = m_UniformLocationCache.find(name);
		if (it != m_UniformLocationCache.end())
			return (*it).second;
		GLint location = OE_CHECK_GL(glGetUniformLocation(m_ID, name.c_str()));
		m_UniformLocationCache.insert(std::make_pair(name, location));
		return location;
	}
	
	GLint GLShader::getUniformBlockIndex(const GLchar* name) const
	{
		return OE_CHECK_GL(glGetUniformBlockIndex(m_ID, name));
	}

	void GLShader::setUniform1f(const std::string& name, float value) {
		glUniform1f(getUniformLocation(name), value);
	}

	void GLShader::setUniform1i(const std::string& name, int value) {
		glUniform1i(getUniformLocation(name), value);
	}

	void GLShader::setUniform2f(const std::string& name, const Math::Vec2f& vector) {
		glUniform2f(getUniformLocation(name), vector.x, vector.y);
	}

	void GLShader::setUniform3f(const std::string& name, const Math::Vec3f& vector) {
		glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
	}

	void GLShader::setUniform4f(const std::string& name, const Math::Vec4f& vector) {
		glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
	}

	void GLShader::setUniformMat4(const std::string& name, const Math::Mat4& matrix) {
		const Math::Mat4 transposed = Math::Mat4::Transpose(matrix);
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &transposed._11);
	}

	void GLShader::setUniform1iv(const std::string& name, int* value, int count) {
		glUniform1iv(getUniformLocation(name), count, value);
	}

	void GLShader::bindUBO(const GLuint block_index, const GLuint uboSlot) const
	{
		OE_CHECK_GL(glUniformBlockBinding(m_ID, block_index, uboSlot));
	}

	void GLShader::matchLayout(Mesh* mesh) const
	{
		int i = 0;
		VertexLayout* vertexLayout = mesh->getVBO()->getLayout();
		const std::vector<VertexElement>& layout = vertexLayout->getElements();

		mesh->draw(0, 0); // quick hack to force the generation of the VAO

		bind();
		for (const ShaderAttribute& sattr : p_Reflections.at(ShaderType::VERTEX).inputAttributes) {
			GLint loc = OE_CHECK_GL(glGetAttribLocation(m_ID, sattr.name.c_str()));
			if (loc >= 0) {
				OE_CHECK_GL(glEnableVertexAttribArray(loc));
				OE_CHECK_GL(glVertexAttribPointer(loc, layout[i].count, layout[i].type, layout[i].normalized, vertexLayout->getStride(), (const GLvoid*)(layout[i].offset)));
			}
			i++;
		}
	}
} }