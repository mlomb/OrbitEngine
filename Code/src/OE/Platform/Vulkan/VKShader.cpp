#include "OE/Platform/Vulkan/VKShader.hpp"

#include "OE/Misc/Log.hpp"
#include "OE/Platform/Vulkan/VKContext.hpp"

namespace OrbitEngine {	namespace Graphics {
	VKShader::VKShader()
	{
	}

	VKShader::~VKShader()
	{
		// Destroy shader modules
		for (auto& module : m_ShaderModules)
			vkDestroyShaderModule(Application::priv::VKContext::GetCurrent()->getDevice(), module, nullptr);
		m_ShaderModules.clear();
	}

	void VKShader::attachFromBinary(ShaderType type, const std::vector<char>& binary)
	{
		VkShaderModule module = CreateShaderModule(binary);
		if (!module)
			return;
		m_ShaderModules.push_back(module);

		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = ShaderTypeToVKShaderStage(type);
		shaderStageInfo.module = module;
		shaderStageInfo.pName = "main";
		m_ShaderStages.push_back(shaderStageInfo);
	}

	void VKShader::attachFromSource(ShaderType type, const std::string& source)
	{
		shaderc_shader_kind shader_kind = ShaderTypeToShadercKind(type);
		
		/*
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		options.SetOptimizationLevel(shaderc_optimization_level_size);

		shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, shader_kind, "VKShader", options);
		if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
			OE_LOG_ERROR("Couldn't compile GLSL to SPV: " << result.GetErrorMessage())
			return;
		}

		std::vector<char> binary = { result.cbegin(), result.cend() };
		*/
	}

	VkShaderModule VKShader::CreateShaderModule(const std::vector<char>& binary)
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = binary.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(binary.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(Application::priv::VKContext::GetCurrent()->getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			OE_LOG_ERROR("Failed to create the shader module!")
			return 0;
		}

		return shaderModule;
	}

	void VKShader::finalize()
	{
		// Nothing
	}

	void VKShader::bind() const
	{

	}

	void VKShader::unbind() const
	{

	}

	shaderc_shader_kind VKShader::ShaderTypeToShadercKind(ShaderType type)
	{
		switch (type) {
		case ShaderType::VERTEX:
			return shaderc_shader_kind::shaderc_vertex_shader;
		default:
			return shaderc_shader_kind();
		}
	}

	VkShaderStageFlagBits VKShader::ShaderTypeToVKShaderStage(ShaderType type)
	{
		switch (type) {
		case ShaderType::VERTEX:
			return VK_SHADER_STAGE_VERTEX_BIT;
		case ShaderType::FRAGMENT:
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		default:
			return VkShaderStageFlagBits();
		}
	}
} }