#include "OE/Graphics/3D/ForwardRenderer.hpp"

#include "OE/Graphics/3D/Skybox.hpp"
#include "OE/Graphics/API/FrameBuffer.hpp"
#include "OE/Graphics/API/States.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLShader.hpp"
#endif

#include "OE/Application/ContextImpl.hpp"

namespace OrbitEngine { namespace Graphics {
	
	ForwardRenderer::ForwardRenderer()
	{

	}

	ForwardRenderer::~ForwardRenderer()
	{

	}

	void ForwardRenderer::end() {
		FrameBuffer::Prepare();

		States* states = Application::priv::ContextImpl::GetCurrent()->getGlobalStates();

		states->setBlending(BlendState::ONE_ZERO);
		states->setDepthTest(FunctionMode::LESS);
		states->setCullMode(CullMode::NONE);

		Material* currentMaterial = 0;
		Shader* currentShader = 0;

		for (Light* l : p_Lights) {
			ShaderDefinitions defs = l->getRequiredDefinitions();
			defs.emplace("FORWARD");

			for (auto &command : p_Commands) {
				if (command.material != currentMaterial) {
					currentMaterial = command.material;

					Shader* shader = currentMaterial->use(defs);
					shader->bind();

					if (shader != currentShader) {
						currentShader = shader;
						// bind non-material related stuff
						fillCameraBuffer(currentShader);

						if (p_Skybox) {
							if (p_Skybox->getPrefilteredEnviromentMap())
								p_Skybox->getPrefilteredEnviromentMap()->bind(9);
							else
								p_Skybox->getEnviromentMap()->bind(9);
						}

#if OE_OPENGL_ANY
						if (Application::Context::GetCurrentAPI() == OPENGL
#if OE_OPENGL_ES
							|| Application::Context::GetCurrentAPI() == OPENGL_ES
#endif
							) {
							GLShader* glGBufferShader = (GLShader*)currentShader;
							glGBufferShader->setUniform1i("enviroment", 9);
							glGBufferShader->setUniform1i("preintegratedBRDFLUT", 10);
						}
#endif
					}
					// bind material related stuff
					currentMaterial->use(); 

					// bind light related stuff
					l->fillBuffer(currentShader);
				}
				// bind object related stuff
				fillObjectBuffer(currentShader, command);

				command.mesh->drawIndexed(command.count);
			}

			// After the first pass..
			states->setBlending(BlendState::ONE_ONE);
			states->setDepthTest(FunctionMode::EQUAL);
		}
	}
} }