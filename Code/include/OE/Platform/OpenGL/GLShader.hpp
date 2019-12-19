#ifndef GRAPHICS_GLSHADER_HPP
#define GRAPHICS_GLSHADER_HPP

#include <unordered_map>

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

		void attachFromBinary(ShaderType type, const std::vector<char>& binary) override;
		void attachFromSource(ShaderType type, const std::string& source) override;
		void finalize() override;

		void setUniform1i(const std::string& name, int value);
		void setUniform1iv(const std::string& name, int* value, int count);
		void setUniform1f(const std::string& name, float value);
		void setUniform2f(const std::string& name, const Math::Vec2f& vector);
		void setUniform3f(const std::string& name, const Math::Vec3f& vector);
		void setUniform4f(const std::string& name, const Math::Vec4f& vector);
		void setUniformMat4(const std::string& name, const Math::Mat4& matrix);

		void bindUBO(const GLuint block_index, const GLuint uboSlot) const;

		void matchLayout(Mesh* mesh) const override;
	private:
		GLuint m_ID = -1;
		std::unordered_map<std::string, GLint> m_UniformLocationCache;

		GLint getUniformLocation(const std::string& name);
		GLint getUniformBlockIndex(const GLchar* name) const;
	};
} }

#endif