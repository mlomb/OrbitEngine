#include "OE/Platform/OpenGL/GLShader.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"
#include "OE/Platform/OpenGL/GLShaderReflection.hpp"

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
		static_cast<GLShaderReflection*>(p_Reflection)->reflect(m_ID);

		/* Bind the UBOs automatically */
		bind();
		for (const auto& buffer : p_Reflection->getAllBuffers())
			bindUBO(buffer.name.c_str(), buffer.slot);
	}
	
	void GLShader::bind() const
	{
		OE_CHECK_GL(glUseProgram(m_ID));
	}

	void GLShader::unbind() const
	{
		OE_CHECK_GL(glUseProgram(0));
	}

	void GLShader::attachFromBinary(ShaderType type, const std::vector<char>& binary)
	{
		OE_LOG_FATAL("This isn't supported in OpenGL")
	}

	GLint GLShader::getUniformLocation(const GLchar* name) const {
		return glGetUniformLocation(m_ID, name);
	}

	GLint GLShader::getUniformBlockIndex(const GLchar* name) const
	{
		return glGetUniformBlockIndex(m_ID, name);
	}

	void GLShader::setUniform1f(const GLchar* name, float value) const {
		glUniform1f(getUniformLocation(name), value);
	}

	void GLShader::setUniform1i(const GLchar* name, int value) const {
		glUniform1i(getUniformLocation(name), value);
	}

	void GLShader::setUniform2f(const GLchar* name, const Math::Vec2f& vector) const {
		glUniform2f(getUniformLocation(name), vector.x, vector.y);
	}

	void GLShader::setUniform3f(const GLchar* name, const Math::Vec3f& vector) const {
		glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
	}

	void GLShader::setUniform4f(const GLchar* name, const Math::Vec4f& vector) const {
		glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
	}

	void GLShader::setUniformMat4(const GLchar* name, const Math::Mat4& matrix) const {
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix._11);
	}

	void GLShader::setUniform1iv(const GLchar* name, int* value, int count) const {
		glUniform1iv(getUniformLocation(name), count, value);
	}

	void GLShader::bindUBO(const GLchar* name, const unsigned int uboSlot) const
	{
		bindUBO(getUniformBlockIndex(name), uboSlot);
	}

	void GLShader::bindUBO(const unsigned int block_index, const unsigned int uboSlot) const
	{
		glUniformBlockBinding(m_ID, block_index, uboSlot);
	}
} }