#include "OE/Graphics/3D/Skybox.hpp"

#include "OE/Graphics/API/States.hpp"
#include "OE/Graphics/API/UniformsPack.hpp"
#include "OE/Graphics/ShaderLoader.hpp"
#include "OE/Graphics/2D/Renderer2D.hpp"

#include "OE/Application/Context.hpp"
#include "OE/Application/ContextImpl.hpp"

#include "OE/Platform/OpenGL/GLShader.hpp"
#include "OE/Platform/OpenGL/GLFrameBuffer.hpp"

namespace OrbitEngine { namespace Graphics {

	Skybox::Skybox(Texture* texture)
		: EnviromentMap(texture)
	{
		m_Shader = ShaderLoader::Skybox();

		if (Application::Context::GetCurrentAPI() == RenderAPI::OPENGL) {
			GLShader* glShader = (GLShader*)m_Shader;
			glShader->bind();
			glShader->setUniform1i("cubeMap", 0);
		}
	}

	void Skybox::render(Misc::Camera* camera)
	{
		/*
		if (buffer != nullptr) {
			glViewport(0, 0, buffer->getWidth(), buffer->getHeight());

			glBindFramebuffer(GL_READ_FRAMEBUFFER, ((GLFrameBuffer*)buffer)->getID());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, 0, buffer->getWidth(), buffer->getHeight(), 0, 0, buffer->getWidth(), buffer->getHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		}
		*/
		FrameBuffer::Prepare();

		m_Shader->bind();
		//getPrefilteredEnviromentMap()->bind(0);
		m_EnviromentMap->bind(0);

		Graphics::UniformsPack<PVMatrices>* m_PVMatrices = Graphics::UniformsPack<PVMatrices>::Create();
		
		PVMatrices m_MatricesUniforms;
		m_MatricesUniforms.pmatrix.pr_matrix = camera->getProjectionMatrix();
		m_MatricesUniforms.vmatrix.vw_matrix = camera->getViewMatrix();
		m_PVMatrices->setData(m_MatricesUniforms);
		m_PVMatrices->bind(0, Graphics::ShaderType::VERTEX);

		States* states = Application::priv::ContextImpl::GetCurrent()->getGlobalStates();

		states->setDepthTest(FunctionMode::LESS_EQUAL);
		Renderer2D::RenderQuadScreen();
		states->setDepthTest(FunctionMode::DISABLED);
	}
} }