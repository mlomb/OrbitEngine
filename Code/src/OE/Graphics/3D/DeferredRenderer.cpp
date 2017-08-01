#include "OE/Graphics/3D/DeferredRenderer.hpp"

#include "OE/Graphics/3D/Skybox.hpp"
#include "OE/Graphics/2D/Renderer2D.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLShader.hpp"
#endif

#include "OE/Application/ContextImpl.hpp"
#include "OE/Application/WindowImpl.hpp"

namespace OrbitEngine { namespace Graphics {

	Graphics::FrameBuffer* DeferredRenderer::s_IntegratedBRDFLUT = nullptr;

	DeferredRenderer::DeferredRenderer()
		: m_GBuffer(0)
	{
		m_GBufferShader = Graphics::Shader::GBuffer();
		m_LighPassShader = Graphics::Shader::DeferredPBR();
		m_PVMatrices = Graphics::UniformsPack<PVMatrices>::Create();
		m_MMatrix = Graphics::UniformsPack<MMatrix>::Create();

		// Generate a look-up texture to cache the second sum of the split sum approximation used by Unreal (see notes)
		if (s_IntegratedBRDFLUT == nullptr) {
			/*
			unsigned short integratedBRDFSize = 128;
			Graphics::TextureFormatProperties props;
			props.format = Graphics::TextureFormat::RG8;
			props.dataType = Graphics::TextureDataType::UNSIGNED_BYTE;
			props.width = props.height = integratedBRDFSize;
			props.mipmapping = false;

			s_IntegratedBRDFLUT = Graphics::FrameBuffer::Create(integratedBRDFSize, integratedBRDFSize);
			s_IntegratedBRDFLUT->attachColorTexture(props);
			s_IntegratedBRDFLUT->finalize();

			Graphics::Shader* integrateBRDFShader = Graphics::Shader::IntegrateBRDF();

			FrameBuffer::Push(s_IntegratedBRDFLUT);
			FrameBuffer::Prepare();
			integrateBRDFShader->bind();

			Renderer2D::RenderQuadScreen();

			integrateBRDFShader->unbind();
			FrameBuffer::Pop();

			delete integrateBRDFShader;
			*/
		}
	}

	void DeferredRenderer::end() {
		Math::Vec2i size = Graphics::FrameBuffer::GetCurrentSize();
		if (!m_GBuffer || Math::Vec2i(m_GBuffer->getWidth(), m_GBuffer->getHeight()) != size) {
			generateGBuffer();
		}

		geometryPass();
		lightningPass();
		skyboxPass();
	}

	void DeferredRenderer::generateGBuffer()
	{
		if (m_GBuffer)
			delete m_GBuffer;

		Math::Vec2i size = Graphics::FrameBuffer::GetCurrentSize();
		m_GBuffer = Graphics::FrameBuffer::Create(size.x, size.y);
		m_GBuffer->setClearColor(Math::Vec4f(0, 0, 0, 1));

		Graphics::TextureFormatProperties formatProperties;
		formatProperties.mipmapping = false;

		formatProperties.format = Graphics::TextureFormat::RGB8;
		formatProperties.dataType = Graphics::TextureDataType::UNSIGNED_BYTE;
		formatProperties.bpp = 32;
		m_GBuffer->attachColorTexture(formatProperties, 2); // Positions, Normals

		formatProperties.format = Graphics::TextureFormat::RGBA8;
		formatProperties.dataType = Graphics::TextureDataType::UNSIGNED_BYTE;
		formatProperties.bpp = 32;
		m_GBuffer->attachColorTexture(formatProperties); // Color + Specular

		formatProperties.format = Graphics::TextureFormat::DEPTH;
		m_GBuffer->attachDepthTexture(formatProperties); // Depth

		m_GBuffer->finalize();
	}

	void DeferredRenderer::geometryPass()
	{
		States* states = Application::priv::ContextImpl::GetCurrent()->getGlobalStates();

		states->setBlending(Graphics::BlendState::DISABLED);
		states->setDepthTest(Graphics::FunctionMode::LESS);
		states->setCullMode(Graphics::CullMode::NONE);

		/* TESTING TODO REMOVE */
		bool wf = false;
		if (Application::priv::ContextImpl::GetCurrent()->getWindow()->getInputManager()->isButtonDown(Application::Button::RIGHT)) {
			wf = true;
			states->setCullMode(Graphics::CullMode::WIREFRAME);
		}
		/* -- */
		
		FrameBuffer::Push(m_GBuffer);
		FrameBuffer::Prepare();
		m_GBuffer->clear();
		m_GBufferShader->bind();

#if OE_OPENGL_ANY
		if (Application::Context::GetCurrentAPI() == OPENGL
#if OE_OPENGL_ES
			|| Application::Context::GetCurrentAPI() == OPENGL_ES
#endif
			) {
			Graphics::GLShader* glGBufferShader = (Graphics::GLShader*)m_GBufferShader;
			glGBufferShader->bindUBO("PVMatrices", 0);
			glGBufferShader->bindUBO("MMatrix", 1);

			Graphics::GLShader* glLighPassShader = (Graphics::GLShader*)m_LighPassShader;
			glLighPassShader->bind();
			glLighPassShader->setUniform1i("GPosition", 0);
			glLighPassShader->setUniform1i("GNormalRoughness", 1);
			glLighPassShader->setUniform1i("GAlbedoMetallic", 2);
			glLighPassShader->setUniform1i("enviroment", 3);
			glLighPassShader->setUniform1i("preintegratedBRDFLUT", 4);
		}
#endif

		Material::Prepare(m_GBufferShader);

		m_MatricesUniforms.pmatrix.pr_matrix = p_Camera->getProjectionMatrix();
		m_MatricesUniforms.vmatrix.vw_matrix = p_Camera->getViewMatrix();
		m_PVMatrices->setData(m_MatricesUniforms);
		m_PVMatrices->bind(0, Graphics::ShaderType::VERTEX);

		for (auto &command : m_Commands) {
			m_MMatrixUniform.ml_matrix = command.transform;
			m_MMatrix->setData(m_MMatrixUniform);
			m_MMatrix->bind(1, Graphics::ShaderType::VERTEX);

			if (command.material != nullptr)
				command.material->use();

			if(m_Skybox)
				m_Skybox->getPrefilteredEnviromentMap()->bind(3);
			//m_Skybox->getEnviromentMap()->bind(3);
			//s_IntegratedBRDFLUT->getColorTextures()[0]->bind(4);

			command.mesh->drawIndexed(36);
		}

		m_GBufferShader->unbind();
		FrameBuffer::Pop();
	}

	int debug = 0;

	void DeferredRenderer::lightningPass() {
		m_LighPassShader->bind();

#if OE_OPENGL_ANY
		if (Application::Context::GetCurrentAPI() == OPENGL
#if OE_OPENGL_ES
			|| Application::Context::GetCurrentAPI() == OPENGL_ES
#endif
			) {
			/*
			if (Application::InputManager::GetCurrent()->isKeyPressed(Application::Key::KEYPAD_0)) debug = 0;
			else if (Application::InputManager::GetCurrent()->isKeyPressed(Application::Key::KEYPAD_1)) debug = 1;
			else if (Application::InputManager::GetCurrent()->isKeyPressed(Application::Key::KEYPAD_2)) debug = 2;
			else if (Application::InputManager::GetCurrent()->isKeyPressed(Application::Key::KEYPAD_3)) debug = 3;
			else if (Application::InputManager::GetCurrent()->isKeyPressed(Application::Key::KEYPAD_4)) debug = 4;
			else if (Application::InputManager::GetCurrent()->isKeyPressed(Application::Key::KEYPAD_5)) debug = 5;
			*/
			Graphics::GLShader* glLighPassShader = (Graphics::GLShader*)m_LighPassShader;
			glLighPassShader->setUniform1i("debug", debug);
			glLighPassShader->setUniform3f("camPosition", p_Camera->getPosition());
		}
#endif

		std::vector<Graphics::Texture*> colorBuffers = m_GBuffer->getColorTextures();
		for (size_t i = 0; i < colorBuffers.size(); i++)
			colorBuffers[i]->bind(i);

		States* states = Application::priv::ContextImpl::GetCurrent()->getGlobalStates();
		states->setCullMode(Graphics::CullMode::NONE);

		FrameBuffer::Prepare();
		Renderer2D::RenderQuadScreen();
	}

	void DeferredRenderer::skyboxPass()
	{
		if (m_Skybox == nullptr)
			return;

		m_Skybox->render(m_GBuffer, p_Camera);
	}
} }