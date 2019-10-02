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
			case 2:
				return Xsc::OutputShaderVersion::ESSL100;
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
	
	void resolveFields(const std::string& prefix, size_t offset, const Xsc::Reflection::ReflectionData& xsc_reflection, const std::vector<Xsc::Reflection::Field>& fields, std::vector<ShaderUniform>& uniforms) {
		for (const Xsc::Reflection::Field& f : fields) {
			ShaderUniformType type = ShaderUniformType::UNKNOWN;
			switch (f.type) {
			case Xsc::Reflection::FieldType::Bool:   type = ShaderUniformType::BOOL;   break;
			case Xsc::Reflection::FieldType::Int:    type = ShaderUniformType::INT;    break;
			case Xsc::Reflection::FieldType::UInt:   type = ShaderUniformType::UINT;   break;
			case Xsc::Reflection::FieldType::Half:   type = ShaderUniformType::HALF;   break;
			case Xsc::Reflection::FieldType::Float:  type = ShaderUniformType::FLOAT;  break;
			case Xsc::Reflection::FieldType::Double: type = ShaderUniformType::DOUBLE; break;
			case Xsc::Reflection::FieldType::Record:
				const Xsc::Reflection::Record& record = xsc_reflection.records[f.typeRecordIndex];
				resolveFields(f.name, offset + f.offset, xsc_reflection, record.fields, uniforms);
				continue;
			}

			uniforms.push_back({ (prefix.size() > 0 ? prefix + "." : "") + f.name, type, f.size, offset + f.offset, { f.dimensions[0], f.dimensions[1] } });
		}
	}

	ShaderCompilationResult ShaderCompiler::CrossCompileHLSL(const std::string hlslSource, ShaderType shaderType, std::string entryPoint)
	{
		Application::priv::ContextImpl* context = Application::priv::ContextImpl::GetCurrent();

		bool isD3D = false;
#if OE_D3D
		isD3D = context->getAPI() == RenderAPI::DIRECT3D;
#endif
		ShaderReflection reflection;

		auto inputStream = std::make_shared<std::istringstream>(hlslSource);
		std::ostringstream outputStream;

		Xsc::ShaderInput inputDesc;
		inputDesc.sourceCode = inputStream;
		inputDesc.shaderVersion = Xsc::InputShaderVersion::HLSL5;
		inputDesc.entryPoint = entryPoint;
		inputDesc.shaderTarget = ShaderToXSCShader(shaderType);

		Xsc::ShaderOutput outputDesc;
		outputDesc.shaderVersion = OutputShaderVersionFromCurrentContext();
		outputDesc.sourceCode = &outputStream;
		//outputDesc.options.autoBinding = true;
		outputDesc.options.allowExtensions = true;
		outputDesc.options.preserveComments = false;
		outputDesc.options.optimize = true;
		outputDesc.options.writeGeneratorHeader = false; // may cause problems with glsl-optimizer

		/* If we are in Direct3D we only need to preprocess the shader since it's already in HLSL */
		if (isD3D)
			outputDesc.options.preprocessOnly = true;

		try
		{
			// TODO Replace the default StdLog -> OE_LOG_*
			Xsc::StdLog xsc_log;
			Xsc::Reflection::ReflectionData xsc_reflection;
			bool result = Xsc::CompileShader(inputDesc, outputDesc, &xsc_log, &xsc_reflection);
			if (!result) {
				OE_LOG_WARNING("Couldn't cross compile the shader!");
				xsc_log.PrintAll();
			}
			else {
				// parse reflection
				for (Xsc::Reflection::ConstantBuffer cb : xsc_reflection.constantBuffers) {
					ShaderBuffer buff;
					buff.name = cb.name;
					buff.slot = 0;
					buff.size = cb.size == 0xFFFFFFFF ? -1 : 0;
					buff.padding = cb.padding;

					resolveFields("", 0, xsc_reflection, cb.fields, buff.uniforms);

					reflection.buffers.push_back(buff);
				}
			}
		}
		catch (const std::exception& e)
		{
			OE_LOG_ERROR(e.what());
			return { "" };
		}

		//////////////////////////
		/// Post process shader //
		//////////////////////////
		/*
		// Convert this
		layout(std140, row_major) uniform Block
		{
			highp float a;
			highp float b;
			highp float c;
		};

		// To this
		uniform highp float a;
		uniform highp float b;
		uniform highp float c;
		*/
		bool dismantled_ubos = false;

#if OE_OPENGL_ANY
		if (context->getAPI() == OPENGL
#if OE_OPENGL_ES
			|| context->getAPI() == OPENGL_ES
#endif
			) {
			Application::priv::GLContext* glcontext = reinterpret_cast<Application::priv::GLContext*>(context);
			dismantled_ubos = !glcontext->getInfo().ubo_support;
		}
#endif

		std::stringstream ss(outputStream.str());
		std::ostringstream oo;
		std::string line;

		bool inside_ubo_definition = false;

		while (std::getline(ss, line, '\n')) {
			if (line.find("layout(std140) uniform") != std::string::npos ||
				line.find("layout(std140, row_major) uniform") != std::string::npos) {
				inside_ubo_definition = true;
				if (dismantled_ubos) {
					std::getline(ss, line, '{'); // crop until {
					continue;
				}
			}
			if (line.size() > 0) {
				if (inside_ubo_definition) {
					if (line == "};") {
						inside_ubo_definition = false;
						if(dismantled_ubos)
							continue;
					}
					if(dismantled_ubos)
						oo << "uniform ";
				}
			}
			oo << line << '\n';
		}
		std::string source = oo.str();

		//OE_LOG_DEBUG("------------------------");
		//OE_LOG_DEBUG(" ---- HLSL5 -> " << (int)outputDesc.shaderVersion << " ---- ");
		//OE_LOG_DEBUG("Shader Input:");
		//OE_LOG_DEBUG(hlslSource);
		//OE_LOG_DEBUG("Shader Output:");
		//OE_LOG_DEBUG(source);
		//OE_LOG_DEBUG("------------------------");

#if OE_OPENGL_ANY
		if (context->getAPI() == OPENGL
#if OE_OPENGL_ES
			|| context->getAPI() == OPENGL_ES
#endif
			) {
			static bool glsl_optimizer_initialized = false;
			static glslopt_ctx* ctx = NULL;

			glslopt_shader_type type;
			switch (shaderType)
			{
			case VERTEX: type = glslopt_shader_type::kGlslOptShaderVertex; break;
			case FRAGMENT: type = glslopt_shader_type::kGlslOptShaderFragment; break;
			default:
				return { source, reflection }; // not supported by glsl-optimize
			}

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
					return { source, reflection }; // not supported by glsl-optimize
				}
				ctx = glslopt_initialize(targetVersion);
				// call glslopt_cleanup?
			}

			glslopt_shader* shader = glslopt_optimize(ctx, type, source.c_str(), kGlslOptionSkipPreprocessor);
			if (glslopt_get_status(shader)) {
				source = glslopt_get_output(shader);

				//OE_LOG_DEBUG("Shader optimized output:");
				//OE_LOG_DEBUG(source);
				//OE_LOG_DEBUG("------------------------");
			}
			else {
				OE_LOG_WARNING("Couldn't run glsl-optimizer on shader! " << glslopt_get_log(shader));
			}
			glslopt_shader_delete(shader);
		}
#endif

		return { source, reflection };
	}
} }