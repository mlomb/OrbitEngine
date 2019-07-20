#include "OE/Graphics/ShaderCompiler.hpp"

#include "OE/Application/ContextImpl.hpp"
#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLContext.hpp"
#endif

#include "OE/Misc/Log.hpp"

/* In X.h, some macros causes problems with Xsc */
#ifdef Always
#undef Always
#endif
#include <Xsc/Xsc.h>
#include <glsl_optimizer.h>

namespace OrbitEngine { namespace Graphics {
	
	static Xsc::ShaderTarget ShaderToXSCShader(ShaderType shader)
	{
		switch (shader)
		{
		case VERTEX: return Xsc::ShaderTarget::VertexShader;
		case FRAGMENT: return Xsc::ShaderTarget::FragmentShader;
		case GEOMETRY: return Xsc::ShaderTarget::GeometryShader;
		case TESS_CONTROL: return Xsc::ShaderTarget::TessellationControlShader;
		case TESS_EVAL: return Xsc::ShaderTarget::TessellationEvaluationShader;
		default: case UNKNOWN: return Xsc::ShaderTarget::Undefined;
		}
	}

	static Xsc::OutputShaderVersion OutputShaderVersionFromCurrentContext()
	{
		Application::priv::ContextImpl* context = Application::priv::ContextImpl::GetCurrent();
		switch (Application::priv::ContextImpl::GetCurrentAPI()) {
#if OE_OPENGL_ES
		case OPENGL_ES:
		{
			Application::priv::GLContext* glcontext = reinterpret_cast<Application::priv::GLContext*>(context);
			switch (glcontext->getInfo().major) {
			case 1:
				return Xsc::OutputShaderVersion::ESSL100;
			case 2:
				return Xsc::OutputShaderVersion::ESSL300;
			case 3:
				switch (glcontext->getInfo().minor) {
				case 0: return Xsc::OutputShaderVersion::ESSL300;
				case 1: return Xsc::OutputShaderVersion::ESSL310;
				case 2: return Xsc::OutputShaderVersion::ESSL320;
				}
				break;
			}

			return Xsc::OutputShaderVersion::ESSL;
		}
#endif
#if OE_OPENGL
		case OPENGL:
		{
			Application::priv::GLContext* glcontext = reinterpret_cast<Application::priv::GLContext*>(context);
			switch (glcontext->getInfo().major) {
			case 2:
				switch (glcontext->getInfo().minor) {
				case 0: return Xsc::OutputShaderVersion::GLSL110;
				case 1: return Xsc::OutputShaderVersion::GLSL120;
				}
				break;
			case 3:
				switch (glcontext->getInfo().minor) {
				case 0: return Xsc::OutputShaderVersion::GLSL130;
				case 1: return Xsc::OutputShaderVersion::GLSL140;
				case 2: return Xsc::OutputShaderVersion::GLSL150;
				case 3: return Xsc::OutputShaderVersion::GLSL330;
				}
				break;
			case 4:
				switch (glcontext->getInfo().minor) {
				case 0: return Xsc::OutputShaderVersion::GLSL400;
				case 1: return Xsc::OutputShaderVersion::GLSL410;
				case 2: return Xsc::OutputShaderVersion::GLSL420;
				case 3: return Xsc::OutputShaderVersion::GLSL430;
				case 4: return Xsc::OutputShaderVersion::GLSL440;
				case 5: return Xsc::OutputShaderVersion::GLSL450;
				case 6: return Xsc::OutputShaderVersion::GLSL460;
				}
				break;
			}
			return Xsc::OutputShaderVersion::GLSL;
		}
#endif
#if OE_VULKAN
		case VULKAN:
			return Xsc::OutputShaderVersion::VKSL;
#endif
		}
		return Xsc::OutputShaderVersion::GLSL;
	}
	
	std::string ShaderCompiler::CrossCompileHLSL(const std::string hlslSource, ShaderType shaderType, std::string entryPoint)
	{
		bool isD3D = false;
#if OE_D3D
		isD3D = Application::Context::GetCurrentAPI() == RenderAPI::DIRECT3D;
#endif

		auto inputStream = std::make_shared<std::istringstream>(hlslSource);
		std::ostringstream outputStream;
		std::string output;

		Xsc::ShaderInput inputDesc;
		inputDesc.sourceCode = inputStream;
		inputDesc.shaderVersion = Xsc::InputShaderVersion::HLSL5;
		inputDesc.entryPoint = entryPoint;
		inputDesc.shaderTarget = ShaderToXSCShader(shaderType);

		Xsc::ShaderOutput outputDesc;
		outputDesc.shaderVersion = OutputShaderVersionFromCurrentContext();
		outputDesc.sourceCode = &outputStream;
		outputDesc.options.autoBinding = true;
		outputDesc.options.allowExtensions = true;
		outputDesc.options.preserveComments = false;
		outputDesc.options.writeGeneratorHeader = false; // may cause problems with glsl-optimizer

		/* If we are in Direct3D we only need to preprocess the shader since it's already in HLSL */
		if (isD3D)
			outputDesc.options.preprocessOnly = true;

		try
		{
			// TODO Replace the default StdLog -> OE_LOG_*
			Xsc::StdLog log;
			bool result = Xsc::CompileShader(inputDesc, outputDesc, &log, 0);
			if (result) {
				output = outputStream.str();
			}
			else {
				OE_LOG_WARNING("Couldn't cross compile the shader!");
				log.PrintAll();
			}
		}
		catch (const std::exception& e)
		{
			OE_LOG_ERROR(e.what());
		}

		/*
		OE_LOG_DEBUG("------------------------");
		OE_LOG_DEBUG("Shader Input:");
		OE_LOG_DEBUG(hlslSource);
		OE_LOG_DEBUG("Shader Output:");
		OE_LOG_DEBUG(output);
		OE_LOG_DEBUG("------------------------");
		*/

#if OE_OPENGL_ANY
		if (Application::Context::GetCurrentAPI() == OPENGL
#if OE_OPENGL_ES
			|| Application::Context::GetCurrentAPI() == OPENGL_ES
#endif
			) {
			static bool glsl_optimizer_initialized = false;
			static glslopt_ctx* ctx = NULL;

			if (!glsl_optimizer_initialized) {
				glslopt_target targetVersion;
				switch (outputDesc.shaderVersion) {
				case Xsc::OutputShaderVersion::ESSL:
				case Xsc::OutputShaderVersion::ESSL100:
					targetVersion = glslopt_target::kGlslTargetOpenGLES20;
					break;
				case Xsc::OutputShaderVersion::ESSL300:
				case Xsc::OutputShaderVersion::ESSL310:
				case Xsc::OutputShaderVersion::ESSL320:
					targetVersion = glslopt_target::kGlslTargetOpenGLES30;
					break;
				case Xsc::OutputShaderVersion::GLSL110:
				case Xsc::OutputShaderVersion::GLSL120:
				case Xsc::OutputShaderVersion::GLSL130:
				case Xsc::OutputShaderVersion::GLSL140:
				case Xsc::OutputShaderVersion::GLSL150:
					targetVersion = glslopt_target::kGlslTargetOpenGL;
					break;
				default:
					return output; // not supported by glsl-optimize
				}
				ctx = glslopt_initialize(targetVersion);
				// call glslopt_cleanup?
			}

			glslopt_shader_type type;
			switch (shaderType)
			{
			case VERTEX: type = glslopt_shader_type::kGlslOptShaderVertex; break;
			case FRAGMENT: type = glslopt_shader_type::kGlslOptShaderFragment; break;
			default:
				return output; // not supported by glsl-optimize
			}

			glslopt_shader* shader = glslopt_optimize(ctx, type, output.c_str(), kGlslOptionSkipPreprocessor);
			if (glslopt_get_status(shader)) {
				output = glslopt_get_output(shader);
				/*
				OE_LOG_DEBUG("Shader optimized output:");
				OE_LOG_DEBUG(output);
				OE_LOG_DEBUG("------------------------");
				*/
			}
			else {
				OE_LOG_WARNING("Couldn't run glsl-optimizer on shader! " << glslopt_get_log(shader));
			}
			glslopt_shader_delete(shader);
		}
#endif

		return output;
	}
} }