#include "OE/Graphics/2D/PathRenderer2D.hpp"

#include "OE/Application/Context.hpp"
#include "OE/Application/ContextImpl.hpp"
#include "OE/Graphics/ShaderLoader.hpp"
#include "OE/Graphics/API/States.hpp"
#if OE_OPENGL_ANY
#include "OE/Platform/OpenGL/GLShader.hpp"
#endif

namespace OrbitEngine { namespace Graphics {
	PathRenderer2D::PathRenderer2D(unsigned int batchSize)
		: BatchRenderer2D<PathRenderer2DVertex>(
		([]() {
		VertexLayout* layout = new VertexLayout();
		layout->push<Math::Vec2f>("POSITION"); // Vertex position
		layout->push<Math::Vec2f>("TEXCOORD"); // UV's
		return layout;
	})(),
		batchSize,
		Topology::TRIANGLES_FAN
		)
	{
		m_ScissorStack = new Misc::ScissorStack();
		m_UniformDataPack = UniformsPack<PathRendererUniformData>::Create();
		m_Shader = ShaderLoader::Vector2D();

		TextureArrayHandler::init(m_Shader);

#if OE_OPENGL_ANY
		if (Application::Context::GetCurrentAPI() == OPENGL
#if OE_OPENGL_ES
			|| Application::Context::GetCurrentAPI() == OPENGL_ES
#endif
			) {
			GLShader* glShader = (GLShader*)m_Shader;
			glShader->bind();

			// TODO: Remove
			glShader->bindUBO("PVMatrix", 0);
			glShader->bindUBO("Data", 1);
			
			glShader->unbind();
		}
#endif
	}

	PathRenderer2D::~PathRenderer2D()
	{
		delete m_UniformDataPack;
		delete m_Shader;
	}

	void PathRenderer2D::begin()
	{
		m_Shader->bind();
		BatchRenderer2D::begin();
		TextureArrayHandler::begin();

		for (size_t i = 0; i < m_RenderCalls.size(); ++i) {
			delete m_RenderCalls[i];
			m_RenderCalls[i] = nullptr;
		}
		m_TrianglesCall = nullptr;
		m_RenderCalls.clear();

		PathRenderer::beginFrame();
	}

	void PathRenderer2D::end()
	{
		if (m_TrianglesCall != nullptr) {
			PathRendererCall* temp = m_TrianglesCall;
			m_TrianglesCall = nullptr;
			pushCall(temp);
		}

		BatchRenderer2D::end();
		TextureArrayHandler::end();

		States* states = Application::priv::ContextImpl::GetCurrent()->getGlobalStates();
		states->setCullMode(CullMode::NONE); // BACK
		states->setDepthTest(FunctionMode::DISABLED);

		unsigned int i = 0;
		for (PathRendererCall* call : m_RenderCalls) {
			m_UniformDataPack->setData(call->data);
			m_UniformDataPack->bind(1, ShaderType::FRAGMENT);

			if (call->pathsCount > 0) {
				/* Fill */
				states->setBlending(BlendState::NO_COLOR);
				states->setStencil(FunctionMode::ALWAYS, StencilOperation::SEPARATE_INCR_DECR);
				for (i = 0; i < call->pathsCount; i++) {
					Path& p = call->paths[i];
					if (p.fillCount == 0)
						continue;
					
					switch (Application::Context::GetCurrentAPI())
					{
#if OE_OPENGL_ANY
					case RenderAPI::OPENGL:
#if OE_OPENGL_ES
					case RenderAPI::OPENGL_ES:
#endif
						m_Mesh->setTopology(Topology::TRIANGLES_FAN);
						m_Mesh->draw(p.fillCount, p.fillOffset);
						break;
#endif
#if OE_D3D
					case RenderAPI::DIRECT3D:
						m_Mesh->setTopology(Topology::TRIANGLES);
						m_Mesh->drawIndexed((p.fillCount - 2) * 3, p.fillOffset);
						break;
#endif
					}
				}

				/* Stroke/Fringe */
				states->setBlending(BlendState::ONE_ALPHA);
				states->setStencil(FunctionMode::EQUAL, StencilOperation::KEEP); // EQUAL
				for (i = 0; i < call->pathsCount; i++) {
					Path& p = call->paths[i];
					if (p.strokeCount == 0)
						continue;

					m_Mesh->setTopology(Topology::TRIANGLES_STRIP);
					m_Mesh->draw(p.strokeCount, p.strokeOffset);
				}

				states->setStencil(FunctionMode::NOT_EQUAL, StencilOperation::ZERO);
			}

			/* Paint the fill || triangles */
			states->setBlending(BlendState::SRC_ALPHA);
			if (call->trianglesCount > 0) {
				m_Mesh->setTopology(Topology::TRIANGLES);
				m_Mesh->draw(call->trianglesCount, call->trianglesOffset);
			}

			states->setStencil(FunctionMode::DISABLED);
		}

		m_Shader->unbind();
	}

	Misc::ScissorStack* PathRenderer2D::getScissorStack() const
	{
		return m_ScissorStack;
	}

	void PathRenderer2D::pushCall(PathRendererCall* call)
	{
		if (m_TrianglesCall != nullptr) {
			PathRendererCall* temp = m_TrianglesCall;
			m_TrianglesCall = nullptr;
			pushCall(temp);
		}
		call->data.scissor = Math::Vec4f((float)m_ScissorStack->get().rect.x, (float)m_ScissorStack->get().rect.y, (float)m_ScissorStack->get().rect.z, (float)m_ScissorStack->get().rect.w);
		m_RenderCalls.push_back(call);
	}

	void PathRenderer2D::pushVertex(const Math::Vec2f& position, const Math::Vec2f& uv)
	{
		BatchRenderer2D::pushVertex({ p_Transform * position, uv });
		m_Count++;
	}

	unsigned int PathRenderer2D::getVertexOffset() {
		return m_Count;
	};
} }