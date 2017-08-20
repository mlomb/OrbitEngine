#include "OE/Graphics/2D/BatchRenderer.hpp"

#include "OE/Misc/Log.hpp"

#include "OE/Application/Context.hpp"

#include "OE/Graphics/API/VertexLayout.hpp"
#include "OE/Graphics/ShaderLoader.hpp"
#include "OE/Graphics/API/Mesh.hpp"
#include "OE/Graphics/API/States.hpp"

// TODO Get rid of this
#if OE_D3D
	#include "OE/Platform/Direct3D/D3DContext.hpp"
#endif
#if OE_OPENGL_ANY
#include "OE/Platform/OpenGL/GLShader.hpp"
#endif
#include "OE/Application/ContextImpl.hpp"
#include "OE/Application/WindowImpl.hpp"

namespace OrbitEngine { namespace Graphics {

	BatchRenderer::BatchRenderer(int batchSize)
	{
		VertexLayout* layout = new VertexLayout();
		layout->push<Math::Vec2f>("POSITION"); // Vertex position
		layout->push<Math::Color>("COLOR");    // Color
		layout->push<Math::Vec2f>("TEXCOORD"); // UV's
		layout->push<float>("TEXID");   // TextureID

		std::vector<Batch2DVertex> vertices = std::vector<Batch2DVertex>(batchSize);
		std::vector<unsigned short> indices = Mesh::GenerateIndices(Topology::TRIANGLES, batchSize);

		m_BatchMesh = Mesh::Create(vertices, layout, indices);
		m_BatchMesh->setTopology(Topology::TRIANGLES);

		m_BatchShader = ShaderLoader::Batch2D();

#if OE_OPENGL_ANY
		if (Application::Context::GetCurrentAPI() == OPENGL
#if OE_OPENGL_ES
			|| Application::Context::GetCurrentAPI() == OPENGL_ES
#endif
			) {
			Graphics::GLShader* glShader = (Graphics::GLShader*)m_BatchShader;
			glShader->bind();

			GLint texIDs[OE_2D_MAX_TEXTURES];
			for (int i = 0; i < OE_2D_MAX_TEXTURES; i++){
				texIDs[i] = i;
			}
			glShader->setUniform1iv("textures", texIDs, (sizeof(texIDs) / sizeof(*texIDs)));

			glShader->unbind();
		}
#endif

		m_UProjection = Graphics::UniformsPack<Math::Mat4>::Create();
	}

	BatchRenderer::~BatchRenderer()
	{
		delete m_BatchMesh;
		delete m_BatchShader;
	}

	void BatchRenderer::begin()
	{
		Renderer2D::begin();

		// Map the pointer
		m_pVertices = (Batch2DVertex*)m_BatchMesh->getVBO()->mapPointer(MappedPointerMode::WRITE);
		OE_ASSERT_MSG(m_pVertices, "Can't map pointer!");
		m_Count = 0;
	}

	void BatchRenderer::bindColor(const Math::Color& color)
	{
		m_BindColor = color;
	}

	void BatchRenderer::pushVertex(const Math::Vec2f& position, const Math::Color& color, const unsigned short tid, const Math::Vec2f& uv)
	{
		m_pVertices->position = position;
		m_pVertices->color = color;
		m_pVertices->uv = uv;
		m_pVertices->tid = tid;
		m_pVertices++;
	}

	void BatchRenderer::submitRect(const Math::Vec2f(&positions)[4], const Math::Vec2f(&uvs)[4])
	{
		pushVertex(positions[0], m_BindColor, p_BindTexture, uvs[0]);
		pushVertex(positions[1], m_BindColor, p_BindTexture, uvs[1]);
		pushVertex(positions[2], m_BindColor, p_BindTexture, uvs[2]);
		pushVertex(positions[3], m_BindColor, p_BindTexture, uvs[3]);

		m_Count += 6;
	}

	void BatchRenderer::end()
	{
		Renderer2D::end();

		States* states = Application::priv::ContextImpl::GetCurrent()->getGlobalStates();

		states->setBlending(BlendState::SRC_ALPHA);
		states->setDepthTest(FunctionMode::DISABLED);
		states->setStencil(FunctionMode::DISABLED);
		states->setCullMode(CullMode::FRONT);

		m_BatchMesh->getVBO()->unmapPointer();
		m_BatchShader->bind();

		/* TODO Move or Remove */
		Application::WindowProperties& wprops = Application::priv::ContextImpl::GetCurrent()->getWindow()->getProperties();
		Math::Mat4 proj = Math::Mat4::Orthographic(0.0f, (float)wprops.resolution.x, (float)wprops.resolution.y, 0.0f, -1.0f, 1.0f);
		/* -- */

		m_UProjection->setData(proj);
		m_UProjection->bind(0, Graphics::ShaderType::VERTEX);

		m_BatchMesh->drawIndexed(m_Count);
	}
} }