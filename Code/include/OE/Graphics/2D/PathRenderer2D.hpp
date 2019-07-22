#ifndef GRAPHICS_PATH_RENDERER2D_HPP
#define GRAPHICS_PATH_RENDERER2D_HPP

#include "OE/Graphics/2D/BatchRenderer2D.hpp"
#include "OE/Graphics/2D/TextureArrayHandler.hpp"
#include "OE/Graphics/2D/PathRenderer.hpp"

#include "OE/Graphics/API/Mesh.hpp"
#include "OE/Graphics/API/States.hpp"
#include "OE/Graphics/API/Shader.hpp"
#include "OE/Graphics/API/UniformsPack.hpp"

#include "OE/Misc/ScissorStack.hpp"

namespace OrbitEngine { namespace Graphics {
	struct PathRenderer2DVertex {
		Math::Vec2f position;
		Math::Vec2f uv;
	};

	class PathRenderer2D : public BatchRenderer2D<PathRenderer2DVertex>, public TextureArrayHandler, public PathRenderer {
	public:
		PathRenderer2D(unsigned int batchSize = 50000 * 4);
		~PathRenderer2D();

		void begin() override;
		void end() override;

		Misc::ScissorStack* getScissorStack() const;
	private:
		void pushCall(PathRendererCall* call) override;
		void pushVertex(const Math::Vec2f& position, const Math::Vec2f& uv) override;
		unsigned int getVertexOffset() override;

		Misc::ScissorStack* m_ScissorStack;
		Graphics::UniformsPack<PathRendererUniformData>* m_UniformDataPack;

		std::vector<PathRendererCall*> m_RenderCalls;
		PathRendererCall* m_TrianglesCall = nullptr;
	};
} }

#endif