#include "OE/Graphics/2D/SpriteBatcher.hpp"

#include "OE/Application/Context.hpp"
#include "OE/Application/ContextImpl.hpp"
#include "OE/Graphics/ShaderLoader.hpp"
#include "OE/Graphics/API/States.hpp"
#if OE_OPENGL_ANY
#include "OE/Platform/OpenGL/GLShader.hpp"
#endif

namespace OrbitEngine { namespace Graphics {

	SpriteBatcher::SpriteBatcher(int numSprites)
		: BatchRenderer2D<Batch2DVertex>(
			([]() {
				VertexLayout* layout = new VertexLayout();
				layout->push<Math::Vec2f>("POSITION"); // Vertex position
				layout->push<Math::Color4f>("COLOR");  // Color
				layout->push<Math::Vec2f>("TEXCOORD"); // UV's
				layout->push<float>("TEXID");   // TextureID
				return layout;
			})(),
			numSprites * 4,
			Topology::TRIANGLES
			), m_IndexCount(0)
	{
		m_Shader = ShaderLoader::Batch2D();
		m_Shader->matchLayout(m_Mesh);
		TextureArrayHandler::init(m_Shader);
	}

	SpriteBatcher::~SpriteBatcher()
	{
		delete m_Shader;
	}

	void SpriteBatcher::begin()
	{
		BatchRenderer2D::begin();
		TextureArrayHandler::begin();
		SpriteRenderer::begin();
		m_IndexCount = 0;
	}

	void SpriteBatcher::end()
	{
		m_Shader->bind();
		m_UPVMatrix->bind("PVMatrix", m_Shader);

		BatchRenderer2D::end();
		TextureArrayHandler::end();
		SpriteRenderer::end();

		States* states = Application::priv::ContextImpl::GetCurrent()->getGlobalStates();

		states->setBlending(BlendState::SRC_ALPHA);
		states->setDepthTest(FunctionMode::DISABLED);
		states->setStencil(FunctionMode::DISABLED);
		states->setCullMode(CullMode::NONE);

		m_Mesh->drawIndexed(m_IndexCount);
	}

	void SpriteBatcher::submitRect(const Math::Vec2f(&positions)[4], const Math::Vec2f(&uvs)[4], const Math::Color4f& color, unsigned int texture)
	{
		pushVertex({ p_Transform * positions[0], color, uvs[0], (float)texture });
		pushVertex({ p_Transform * positions[1], color, uvs[1], (float)texture });
		pushVertex({ p_Transform * positions[2], color, uvs[2], (float)texture });
		pushVertex({ p_Transform * positions[3], color, uvs[3], (float)texture });
		
		m_IndexCount += 6;
	}
	
	int SpriteBatcher::handleTexture(Texture* tex)
	{
		return TextureArrayHandler::handleTexture(tex);
	}
} }