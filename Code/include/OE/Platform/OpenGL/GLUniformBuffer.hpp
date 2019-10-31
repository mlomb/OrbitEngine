#ifndef GRAPHICS_GLUNIFORM_BUFFER_HPP
#define GRAPHICS_GLUNIFORM_BUFFER_HPP

#include "OE/Platform/OpenGL/GLBuffer.hpp"
#include "OE/Platform/OpenGL/GLContext.hpp"
#include "OE/Platform/OpenGL/GLShader.hpp"

#include "OE/Graphics/API/UniformsPack.hpp"
#include "OE/Graphics/API/Shader.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"

namespace OrbitEngine {	namespace Graphics {
	template <class T>
	class GLUniformBuffer : public UniformsPack<T> {
	public:
		GLUniformBuffer();
		~GLUniformBuffer();

		// If UBOs are disabled bind *must* be called after setData
		void setData(const T& data) override;
		void bind(const ShaderBuffer& buffer, ShaderType shader_type, Shader* shader) const override;

	private:
		GLBuffer* m_Buffer;
		// internal copy of the data in case UBOs are not available
		T* m_Data;
	};

	template<class T>
	inline GLUniformBuffer<T>::GLUniformBuffer() : m_Buffer(NULL), m_Data(NULL) {
		Application::priv::GLContext* glctx = Application::priv::GLContext::GetCurrent();
		if (glctx->getInfo().ubo_support) {
			m_Buffer = new GLBuffer(GL_UNIFORM_BUFFER, GL_STREAM_DRAW);
			m_Buffer->bind();
			m_Buffer->setData(sizeof(T), 0);
		}
		else {
			m_Data = static_cast<T*>(malloc(sizeof(T)));
		}
	}

	template<class T>
	inline GLUniformBuffer<T>::~GLUniformBuffer()
	{
		if (m_Buffer)
			delete m_Buffer;
		if (m_Data)
			delete m_Data;
	}

	template<class T>
	inline void GLUniformBuffer<T>::setData(const T& data)
	{
		if (m_Buffer) {
			m_Buffer->setData(sizeof(T), &data);
		}
		else {
			memcpy(m_Data, &data, sizeof(T));
		}
	}

	template<class T>
	inline void GLUniformBuffer<T>::bind(const ShaderBuffer& buffer, ShaderType shader_type, Shader* shader) const
	{
		if (m_Buffer) {
			if (buffer.slot > GL_MAX_UNIFORM_BUFFER_BINDINGS) {
				OE_LOG_WARNING("Max slot uniform buffer exceeded!");
				return;
			}

			glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)buffer.slot, m_Buffer->getID());
		}
		else {
			GLShader* glshader = static_cast<GLShader*>(shader);
			// Bind individual uniforms
			for (const ShaderUniform& uniform : buffer.uniforms) {
				//GLint location = glshader->getUniformLocation(uniform.name);
				void* ptr = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(m_Data) + uniform.offset);

				switch (uniform.type) {
				case ShaderUniformType::FLOAT:
					switch (uniform.dimensions[1]) { // columns
					case 1: // scalars
						switch (uniform.dimensions[0]) { // rows
						case 1: glshader->setUniform1f(uniform.name, *static_cast<float*>(ptr)); break;
						case 2: glshader->setUniform2f(uniform.name, *static_cast<Math::Vec2f*>(ptr)); break;
						case 3: glshader->setUniform3f(uniform.name, *static_cast<Math::Vec3f*>(ptr)); break;
						case 4: glshader->setUniform4f(uniform.name, *static_cast<Math::Vec4f*>(ptr)); break;
						default: OE_LOG_WARNING("Unhandled scalar dimension.");
						}
						break;
					case 4:
						switch (uniform.dimensions[0]) { // rows
						case 4: glshader->setUniformMat4(uniform.name, *static_cast<Math::Mat4*>(ptr)); break;
						default: OE_LOG_WARNING("Unhandled col=4 # rows.");
						}
						break;
					default: OE_LOG_WARNING("Unhandled # of columns.");
					}

					break;
				case ShaderUniformType::INT:
					if (uniform.dimensions[0] == 1 && uniform.dimensions[1] == 1) {
						glshader->setUniform1i(uniform.name, *static_cast<int*>(ptr)); break;
					}
					else OE_LOG_WARNING("Unhandled dimensions for ints.");
					break;
				default: OE_LOG_WARNING("Unhandled type: " << (int)uniform.type);
				}
			}
		}
	}
} }

#endif