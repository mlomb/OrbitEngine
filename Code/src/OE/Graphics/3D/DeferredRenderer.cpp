#include "OE/Graphics/3D/DeferredRenderer.hpp"

#include "OE/Graphics/3D/Skybox.hpp"
#include "OE/Graphics/3D/BRDFLUT.hpp"
#include "OE/Graphics/2D/Renderer2D.hpp"
#include "OE/Graphics/Shaders/ShaderLoader.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLShader.hpp"
	#include "OE/Platform/OpenGL/GLFrameBuffer.hpp"
#endif

#include "OE/Application/ContextImpl.hpp"
#include "OE/Application/WindowImpl.hpp"

namespace OrbitEngine { namespace Graphics {

	DeferredRenderer::DeferredRenderer()
		: m_GBuffer(0)
	{
		m_DeferredLightShader = new ManagedShader("Resources/Shaders/DeferredLighting.oeshader");
	}

	DeferredRenderer::~DeferredRenderer()
	{
		delete m_DeferredLightShader;
	}

	void DeferredRenderer::end() {
		Math::Vec2i size = FrameBuffer::GetCurrentSize();
		if (!m_GBuffer || Math::Vec2i(m_GBuffer->getWidth(), m_GBuffer->getHeight()) != size) {
			if (m_GBuffer)
				delete m_GBuffer;
			m_GBuffer = generateGBuffer(size);
			if (!m_GBuffer) {
				OE_LOG_WARNING("Can't create GBuffer!");
				return;
			}
		}

		geometryPass();
		lightningPass();
		skyboxPass();
	}

	FrameBuffer* DeferredRenderer::generateGBuffer(const Math::Vec2i size)
	{
		FrameBuffer* gBuffer = FrameBuffer::Create(size.x, size.y);
		gBuffer->setClearColor(Math::Vec4f(0, 0, 0, 1));

		TextureFormatProperties formatProperties;
		formatProperties.dataType = Graphics::TextureDataType::FLOAT;
		formatProperties.format = Graphics::TextureFormat::RGBA32F;

		gBuffer->attachColorTextures(3, formatProperties); // Positions, Normals, Color + Specular
		gBuffer->attachDepthStencilTexture();

		gBuffer->finalize();

		return gBuffer;
	}

	void DeferredRenderer::geometryPass()
	{
		States* states = Application::priv::ContextImpl::GetCurrent()->getGlobalStates();

		states->setBlending(BlendState::DISABLED);
		states->setDepthTest(FunctionMode::LESS);
		states->setCullMode(CullMode::NONE);

		/* TESTING TODO REMOVE */
		bool wf = false;
		if (Application::priv::ContextImpl::GetCurrent()->getWindowImpl()->getInputManager()->isButtonDown(Application::Button::RIGHT)) {
			wf = true;
			states->setCullMode(CullMode::WIREFRAME);
		}
		/* -- */
		
		FrameBuffer::Push(m_GBuffer);
		FrameBuffer::Prepare();
		m_GBuffer->clear();

		Material* currentMaterial = 0;
		Shader* currentShader = 0;

		for (auto &command : p_Commands) {
			if (command.material != currentMaterial) {
				currentMaterial = command.material;

				Shader* shader = command.material->use({
					"DEFERRED"
				});
				shader->bind();

				if (shader != currentShader) {
					currentShader = shader;
					// bind non-material related stuff
					fillCameraBuffer(currentShader);
				}
				// bind material related stuff
				currentMaterial->use();
			}
			// bind object related stuff
			fillObjectBuffer(currentShader, command);

			command.mesh->drawIndexed(command.count);
		}

		FrameBuffer::Pop();
	}

	void DeferredRenderer::lightningPass() {
		FrameBuffer::Prepare();

		States* states = Application::priv::ContextImpl::GetCurrent()->getGlobalStates();
		states->setCullMode(CullMode::NONE);
		states->setBlending(BlendState::ONE_ONE);
		states->setDepthTest(FunctionMode::DISABLED);

		// Foreach type of light
		for (Light* l : p_Lights) {
			Shader* shader = m_DeferredLightShader->requestShader(l->getRequiredDefinitions());
			shader->bind();

			/* Bind the stuff */
			fillCameraBuffer(shader);
#if OE_OPENGL_ANY
			if (Application::Context::GetCurrentAPI() == OPENGL
#if OE_OPENGL_ES
				|| Application::Context::GetCurrentAPI() == OPENGL_ES
#endif
				) {
				GLShader* glLighPassShader = (GLShader*)shader;
				glLighPassShader->setUniform1i("GPosition", 0);
				glLighPassShader->setUniform1i("GNormalRoughness", 1);
				glLighPassShader->setUniform1i("GAlbedoMetallic", 2);
				glLighPassShader->setUniform1i("enviroment", 9);
				glLighPassShader->setUniform1i("preintegratedBRDFLUT", 10);
			}
#endif
			std::vector<Texture*> colorBuffers = m_GBuffer->getColorTextures();
			for (unsigned int i = 0; i < colorBuffers.size(); i++)
				colorBuffers[int(i)]->bind(i);

			if (p_Skybox) {
				if (p_Skybox->getPrefilteredEnviromentMap())
					p_Skybox->getPrefilteredEnviromentMap()->bind(9);
				else
					p_Skybox->getEnviromentMap()->bind(9);
			}
			p_BRDFLUT->bind(10);
			/* --- */

			l->fillBuffer(shader);
			Renderer2D::RenderQuadScreen();
		}

		/* Dirty AND TEMPORARY method to unbind the GBuffer */
		for (int slot = 0; slot < 5; slot++)
			p_BRDFLUT->bind(slot);
	}

	void DeferredRenderer::skyboxPass()
	{
		if (p_Skybox == nullptr)
			return;

		// Copy the GBuffer's depth texture into the output framebuffer
		FrameBuffer::GetCurrent()->blit(m_GBuffer, BlitOperation::DEPTH);

		//p_Skybox->render(p_Camera);
	}
} }