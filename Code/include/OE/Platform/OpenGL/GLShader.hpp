#ifndef GRAPHICS_GLSHADER_HPP
#define GRAPHICS_GLSHADER_HPP

#include "OE/Platform/OpenGL/OpenGL.hpp"
#include "OE/Graphics/API/Shader.hpp"

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec3.hpp"
#include "OE/Math/Vec4.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine {	namespace Graphics {
	class GLShader : public Shader {
	public:
		GLShader();
		~GLShader();

		void bind() const override;
		void unbind() const override;
		void attachFromBinary(ShaderType type, const std::vector<char>& binary) override;
		void attachFromSource(ShaderType type, const std::string& source) override;
		void finalize() override;

		void setUniform1i(const GLchar* name, int value) const;
		void setUniform1iv(const GLchar* name, int* value, int count) const;
		void setUniform1f(const GLchar* name, float value) const;
		void setUniform2f(const GLchar* name, const Math::Vec2f& vector) const;
		void setUniform3f(const GLchar* name, const Math::Vec3f& vector) const;
		void setUniform4f(const GLchar* name, const Math::Vec4f& vector) const;
		void setUniformMat4(const GLchar* name, const Math::Mat4& matrix) const;

		void bindUBO(const GLchar* name, const unsigned int uboSlot) const;
	private:
		GLuint m_ID = -1;

		GLint getUniformLocation(const GLchar* name) const;
	};
} }

#endif