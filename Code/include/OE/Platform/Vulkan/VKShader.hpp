#ifndef GRAPHICS_VKSHADER_HPP
#define GRAPHICS_VKSHADER_HPP

#include "OE/Platform/Vulkan/Vulkan.hpp"

#include <shaderc/shaderc.hpp>
#include <map>

#include "OE/Graphics/API/Shader.hpp"

namespace OrbitEngine {	namespace Graphics {
	class VKShader : public Shader {
	public:
		VKShader();
		~VKShader();

		void attachFromBinary(ShaderType type, const std::vector<char>& binary) override;
		void attachFromSource(ShaderType type, const std::string& source) override;
		void finalize() override;
		void bind() const override;
		void unbind() const override;

		std::vector<VkPipelineShaderStageCreateInfo> getShaderStages() { return m_ShaderStages; }

		static shaderc_shader_kind ShaderTypeToShadercKind(ShaderType type);
		static VkShaderStageFlagBits ShaderTypeToVKShaderStage(ShaderType type);
		static VkShaderModule CreateShaderModule(const std::vector<char>& binary);
	private:
		std::vector<VkShaderModule> m_ShaderModules;
		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
	};
} }

#endif