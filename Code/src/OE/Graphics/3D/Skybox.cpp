#include "OE/Graphics/3D/Skybox.hpp"

#include "OE/Application/Context.hpp"

#include "OE/Platform/OpenGL/GLShader.hpp"
#include "OE/Platform/OpenGL/GLFrameBuffer.hpp"

namespace OrbitEngine { namespace Graphics {

	Skybox::Skybox(Texture* texture)
		: EnviromentMap(texture)
	{
		m_Shader = Shader::Skybox();

		if (Application::Context::GetCurrentAPI() == RenderAPI::OPENGL) {
			GLShader* glShader = (GLShader*)m_Shader;
			glShader->bind();
			glShader->setUniform1i("cubeMap", 0);
		}
	}

	void Skybox::render(Misc::Camera* camera) {
		render(0, camera);
	}

	void Skybox::render(FrameBuffer* buffer, Misc::Camera* camera)
	{
		if (buffer != nullptr) {
			glViewport(0, 0, buffer->getWidth(), buffer->getHeight());

			//glBindFramebuffer(GL_READ_FRAMEBUFFER, ((GLFrameBuffer*)buffer)->getID());
			//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			//glBlitFramebuffer(0, 0, buffer->getWidth(), buffer->getHeight(), 0, 0, buffer->getWidth(), buffer->getHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		}
		GLFrameBuffer::Unbind();

		m_Shader->bind();
		getPrefilteredEnviromentMap()->bind(0);
		//m_EnviromentMap->bind(0);

		auto glshader = (GLShader*)m_Shader;

		glshader->setUniformMat4("pr_matrix", camera->getProjectionMatrix());
		glshader->setUniformMat4("vw_matrix", camera->getViewMatrix());

		//Renderer::SetDepthTest(FunctionMode::LESS_EQUAL);
		//Renderer2D::RenderQuadScreen();
		//Renderer::SetDepthTest(FunctionMode::DISABLED);
	}
} }