#include "OE/Graphics/3D/Skybox.hpp"

#include "OE/Graphics/API/States.hpp"
#include "OE/Graphics/API/UniformsPack.hpp"
#include "OE/Graphics/ShaderLoader.hpp"
#include "OE/Graphics/2D/Renderer2D.hpp"

#include "OE/Application/Context.hpp"
#include "OE/Application/ContextImpl.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLShader.hpp"
	#include "OE/Platform/OpenGL/GLFrameBuffer.hpp"
#endif

namespace OrbitEngine { namespace Graphics {

	Skybox::Skybox(Texture* texture)
		: EnviromentMap(texture)
	{
		m_Shader = ShaderLoader::Skybox();

#if OE_OPENGL_ANY
		if (Application::Context::GetCurrentAPI() == RenderAPI::OPENGL) {
			GLShader* glShader = (GLShader*)m_Shader;
			glShader->bind();
			glShader->setUniform1i("cubeMap", 0);
		}
#endif
	}

	void Skybox::render(Misc::Camera* camera)
	{
		FrameBuffer::Prepare();

		m_Shader->bind();
		//getPrefilteredEnviromentMap()->bind(0);
		m_EnviromentMap->bind(0);

		Graphics::UniformsPack<PVMatrices>* m_PVMatrices = Graphics::UniformsPack<PVMatrices>::Create();
		
		PVMatrices m_MatricesUniforms;
		m_MatricesUniforms.pmatrix.pr_matrix = camera->getProjectionMatrix();
		Math::Mat4 view = camera->getViewMatrix();
		/*
		view._41 = 0;
		view._42 = 0;
		view._43 = 0;
		*/

		m_MatricesUniforms.vmatrix.vw_matrix = Math::Mat4::Inverse(Math::Mat4::Transpose(camera->getProjectionMatrix() * view));
		m_PVMatrices->setData(m_MatricesUniforms);
		//m_PVMatrices->bind(0, Graphics::ShaderType::VERTEX);

		States* states = Application::priv::ContextImpl::GetCurrent()->getGlobalStates();

		states->setDepthTest(FunctionMode::LESS_EQUAL);
		Renderer2D::RenderQuadScreen();
	}
} }