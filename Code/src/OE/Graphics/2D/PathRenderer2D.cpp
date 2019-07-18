#include "OE/Graphics/2D/PathRenderer2D.hpp"

#include "OE/Graphics/API/FrameBuffer.hpp"
#include "OE/Graphics/ShaderLoader.hpp"

// TODO Get rid of this
#include "OE/Application/ContextImpl.hpp"
#include "OE/Application/WindowImpl.hpp"
#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLShader.hpp"
#endif

namespace OrbitEngine { namespace Graphics {
	PathRenderer2D::PathRenderer2D()
		: m_Verts(NULL), m_NVerts(0)
	{
		m_Shader = ShaderLoader::Vector2D();

		VertexLayout* layout = new VertexLayout();
		layout->push<Math::Vec2f>("POSITION"); // Vertex position
		layout->push<Math::Vec2f>("TEXCOORD"); // UV's

		std::vector<PathRendererVertex> vertices = std::vector<PathRendererVertex>(MAX_VERTEXS);
		std::vector<unsigned short> indices = Mesh::GenerateIndices(Topology::TRIANGLES_FAN, MAX_VERTEXS * 3);

		m_Mesh = Mesh::Create(vertices, layout, indices);

		m_UniformDataPack = UniformsPack<PathRendererUniformData>::Create();

#if OE_OPENGL_ANY
		if (Application::Context::GetCurrentAPI() == OPENGL
#if OE_OPENGL_ES
			|| Application::Context::GetCurrentAPI() == OPENGL_ES
#endif
			) {
			GLShader* glShader = (GLShader*)m_Shader;
			glShader->bind();

			GLint texIDs[OE_2D_MAX_TEXTURES];
			for (int i = 0; i < OE_2D_MAX_TEXTURES; i++) {
				texIDs[i] = i;
			}
			glShader->setUniform1iv("textures", texIDs, (sizeof(texIDs) / sizeof(*texIDs)));
			// TODO: Remove
			glShader->bindUBO("PVMatrix", 0);
			glShader->bindUBO("Data", 1);
			
			glShader->unbind();
		}
#endif
	}

	PathRenderer2D::~PathRenderer2D()
	{
		delete m_Shader;
		delete m_Mesh;
	}

	void PathRenderer2D::begin()
	{
		Renderer2D::begin();

		m_Verts = (PathRendererVertex*)m_Mesh->getVBO()->mapPointer(MappedPointerMode::WRITE);
		m_NVerts = 0;

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

		Renderer2D::end();
		FrameBuffer::Prepare();

		m_Mesh->getVBO()->unmapPointer();

		States* states = Application::priv::ContextImpl::GetCurrent()->getGlobalStates();
		states->setCullMode(CullMode::NONE); // BACK
		states->setDepthTest(FunctionMode::DISABLED);

		m_Shader->bind();
		m_UPVMatrix->bind(0, ShaderType::VERTEX);
		
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

	void PathRenderer2D::submitRect(const Math::Vec2f(&positions)[4], const Math::Vec2f(&uvs)[4])
	{
		// If we don't have a texture binded, just use the normal PathRenderer
		// The problem with this is that we need to call fill()
		if (p_BindTexture == 0) {
			float commands[] = {
				MOVETO, positions[0].x, positions[0].y,
				LINETO, positions[1].x, positions[1].y,
				LINETO, positions[2].x, positions[2].y,
				LINETO, positions[3].x, positions[3].y,
				CLOSE
			};

			pushCommands(commands, ARRAY_COUNT(commands));
			return;
		}

		/* Find a better way, this is ugly */
		if (m_TrianglesCall == nullptr) {
			m_TrianglesCall = new PathRendererCall();
			m_TrianglesCall->trianglesOffset = getVertexOffset();
			m_TrianglesCall->data.paint = fillStyle();
			m_TrianglesCall->data.texid = p_BindTexture;
			m_TrianglesCall->data.strokeMul = 1;
		}
		else {
			if (p_DirtyState || m_TrianglesCall->data.texid != p_BindTexture /* DO SOMETHING WITH THIS */ /* || m_TrianglesCall->data.scissor != p_ScissorStack->get().rect */) {
				PathRendererCall* temp = m_TrianglesCall;
				m_TrianglesCall = nullptr;
				pushCall(temp);

				submitRect(positions, uvs);
				return;
			}
		}
		p_DirtyState = false;

		pushVertex(positions[0], uvs[0]);
		pushVertex(positions[1], uvs[1]);
		pushVertex(positions[2], uvs[2]);

		pushVertex(positions[2], uvs[2]);
		pushVertex(positions[3], uvs[3]);
		pushVertex(positions[0], uvs[0]);

		m_TrianglesCall->trianglesCount += 6;
	}

	void PathRenderer2D::pushCall(PathRendererCall* call)
	{
		if (m_TrianglesCall != nullptr) {
			PathRendererCall* temp = m_TrianglesCall;
			m_TrianglesCall = nullptr;
			pushCall(temp);
		}
		call->data.scissor = Math::Vec4f((float)p_ScissorStack->get().rect.x, (float)p_ScissorStack->get().rect.y, (float)p_ScissorStack->get().rect.z, (float)p_ScissorStack->get().rect.w);
		m_RenderCalls.push_back(call);
	}

	void PathRenderer2D::pushVertex(const Math::Vec2f& position, const Math::Vec2f& uv)
	{
		if (m_NVerts + 1 >= MAX_VERTEXS) {
			if (m_NVerts + 1 == MAX_VERTEXS) {
				OE_LOG_WARNING("Increase the max of vertexs in PathRenderer2D.");
				m_NVerts++; // Prevent a lot of messages
			}
			return;
		}

		m_Verts->position = p_Transform * position;
		m_Verts->uv = uv;
		m_Verts++;
		m_NVerts++;
	}

	unsigned int PathRenderer2D::getVertexOffset() {
		return m_NVerts;
	};
} }