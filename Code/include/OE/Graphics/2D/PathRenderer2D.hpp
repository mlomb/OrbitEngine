#ifndef GRAPHICS_PATH_RENDERER2D_HPP
#define GRAPHICS_PATH_RENDERER2D_HPP

#include "OE/Graphics/2D/Renderer2D.hpp"
#include "OE/Graphics/2D/PathRenderer.hpp"

#include "OE/Graphics/API/Mesh.hpp"
#include "OE/Graphics/API/States.hpp"
#include "OE/Graphics/API/Shader.hpp"
#include "OE/Graphics/API/UniformsPack.hpp"

#define MAX_VERTEXS 50000

namespace OrbitEngine { namespace Graphics {
	struct PathRendererVertex {
		Math::Vec2f position;
		Math::Vec2f uv;
	};

	class PathRenderer2D : public Renderer2D, public PathRenderer {
	public:
		using Renderer2D::rect;

		PathRenderer2D();
		~PathRenderer2D();

		void begin() override;
		void end() override;

		void bindColor(const Math::Color& color) override { fillColor(color); strokeColor(color); };
		void submitRect(const Math::Vec2f(&positions)[4], const Math::Vec2f(&uvs)[4]) override;
	private:
		void pushVertex(const Math::Vec2f& position, const Math::Vec2f& uv) override;
		void pushCall(PathRendererCall* call) override;
		unsigned int getVertexOffset() override;

		Graphics::Shader* m_Shader;
		Graphics::Mesh* m_Mesh;

		Graphics::UniformsPack<PathRendererUniformData>* m_UniformDataPack;

		std::vector<PathRendererCall*> m_RenderCalls;
		PathRendererVertex* m_Verts;
		unsigned int m_NVerts;

		PathRendererCall* m_TrianglesCall = nullptr;
	};
} }

#endif