#include "OE/Graphics/3D/EnviromentMap.hpp"

#include "OE/Config.hpp"
#include "OE/Application/Context.hpp"
#include "OE/Graphics/MeshGenerator.hpp"
#include "OE/Graphics/ShaderLoader.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLShader.hpp"
#endif

namespace OrbitEngine { namespace Graphics {
	Mesh* EnviromentMap::s_CaptureCube = nullptr;
	Shader* EnviromentMap::s_PrefilterCapture = nullptr;
	Shader* EnviromentMap::s_HDRToCubemap = nullptr;
	static const Math::Mat4 fixedCubeProj = Math::Mat4::Perspective(90.0f, 1.0f, 0.01f, 100.0f);
	Math::Mat4 EnviromentMap::s_CubeFaces[6] = {
		Math::Mat4::LookAt(Math::Vec3f(), Math::Vec3f(1.0, 0.0, 0.0), Math::Vec3f(0.0, -1.0, 0.0)),
		Math::Mat4::LookAt(Math::Vec3f(), Math::Vec3f(-1.0, 0.0, 0.0), Math::Vec3f(0.0, -1.0, 0.0)),
		Math::Mat4::LookAt(Math::Vec3f(), Math::Vec3f(0.0, 1.0, 0.0), Math::Vec3f(0.0, 0.0, 1.0)),
		Math::Mat4::LookAt(Math::Vec3f(), Math::Vec3f(0.0, -1.0, 0.0), Math::Vec3f(0.0, 0.0, -1.0)),
		Math::Mat4::LookAt(Math::Vec3f(), Math::Vec3f(0.0, 0.0, 1.0), Math::Vec3f(0.0, -1.0, 0.0)),
		Math::Mat4::LookAt(Math::Vec3f(), Math::Vec3f(0.0, 0.0, -1.0), Math::Vec3f(0.0, -1.0, 0.0)),
	};

	EnviromentMap::EnviromentMap(Texture* texture)
		: m_EnviromentMap(texture),
		m_PrefilteredEnvMap(0)
	{
		if (s_CaptureCube == nullptr)
			s_CaptureCube = MeshGenerator::Box(10, 10, 10);

		if (texture->getProperties().dimension != TextureDimension::CUBEMAP) {
			// It's an HDR texture, convert it to a cubemap
			/*
			if (s_HDRToCubemap == nullptr) {
				s_HDRToCubemap = Shader::HDRToCubemap();
				setViewMatrices(s_HDRToCubemap);
				((GLShader*)s_HDRToCubemap)->setUniform1i("environment", 0);
			}


			TextureFormatProperties formatProps;
			formatProps.format = TextureFormat::RGB;
			formatProps.dataType = TextureDataType::FLOAT;
			formatProps.width = formatProps.height = 128;
			formatProps.mipmapping = false;

			FrameBuffer* fb = FrameBuffer::Create(formatProps.width, formatProps.height);
			fb->attachColorCubemap(formatProps);
			fb->finalize();
			fb->setClearColor(Math::Vec4f(1, 0, 1, 1)); // TODO Set clear color


			FrameBuffer::Push(fb);
			FrameBuffer::Prepare();
			s_HDRToCubemap->bind();

			auto gls = (GLShader*)s_HDRToCubemap;
			gls->setUniformMat4("pr_matrix", fixedCubeProj);

			fb->clear();
			s_CaptureCube->drawIndexed(36);

			s_HDRToCubemap->unbind();
			FrameBuffer::Pop();

			m_EnviromentMap = fb->getColorTextures()[0];
			*/
		}
		precomputeEnvPBR();
	}

	void EnviromentMap::precomputeEnvPBR()
	{
		if (s_PrefilterCapture == nullptr) {
			s_PrefilterCapture = ShaderLoader::PrefilterEnvMap();
			setViewMatrices(s_PrefilterCapture);
			if (Application::Context::GetCurrentAPI() == RenderAPI::OPENGL)
				((GLShader*)s_PrefilterCapture)->setUniform1i("environment", 0);
		}

		if (m_PrefilteredEnvMap == nullptr) {
			m_PrefilteredEnvMap = FrameBuffer::Create(128, 128);

			TextureFormatProperties formatProperties;
			formatProperties.format = TextureFormat::RGBA32F;
			formatProperties.dataType = TextureDataType::FLOAT;

			TextureSampleProperties sampleProperties;
			sampleProperties.mipmapping = true;

			m_PrefilteredEnvMap->attachColorTextures(1, formatProperties, sampleProperties, CUBEMAP);
			m_PrefilteredEnvMap->finalize();
			m_PrefilteredEnvMap->setClearColor(Math::Vec4f(1, 0, 1, 1)); // TODO Set clear color
		}

		FrameBuffer::Push(m_PrefilteredEnvMap);
		FrameBuffer::Prepare();
		s_PrefilterCapture->bind();
		//Renderer::SetCullMode(CullMode::NONE);

		auto gls = (GLShader*)s_PrefilterCapture;
		gls->setUniformMat4("pr_matrix", fixedCubeProj);

		m_EnviromentMap->bind(0);

		unsigned int mipLevels = Texture::CalculateMipLevelsCount(128, 128);
		mipLevels = 5;
		for (unsigned int i = 0; i < mipLevels; i++) {
			m_PrefilteredEnvMap->useMip(i);
			m_PrefilteredEnvMap->clear();

			gls->setUniform1f("roughness", (float)i / (float)(mipLevels - 1));

			s_CaptureCube->drawIndexed(36);
		}

		FrameBuffer::Pop();
	}

	void EnviromentMap::setViewMatrices(Shader* shader)
	{
		if (Application::Context::GetCurrentAPI() == RenderAPI::OPENGL) {
			GLShader* glShader = (GLShader*)shader;
			glShader->bind();
			//for (int i = 0; i < 6; ++i)
			//	glShader->setUniformMat4(("viewMatrices[" + std::to_string(i) + "]").c_str(), s_CubeFaces[i] * Math::Mat4::Scale(Math::Vec3f(10, 10, 10)));
		}
	}
} }
