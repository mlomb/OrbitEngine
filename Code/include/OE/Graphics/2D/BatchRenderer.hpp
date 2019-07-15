#ifndef GRAPHICS_BATCH_RENDERER_HPP
#define GRAPHICS_BATCH_RENDERER_HPP

#include "OE/Graphics/2D/Renderer2D.hpp"
#include "OE/Graphics/API/UniformsPack.hpp"

#include "OE/Math/Color.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace Graphics {
	class Shader;
	class Mesh;

	class BatchRenderer : public Renderer2D {
	public:
		struct Batch2DVertex {
			Math::Vec2f position;
			Math::Color color;
			Math::Vec2f uv;
			float tid;
		};

		BatchRenderer(int batchSize = 50000 * 4);
		~BatchRenderer();

		void begin() override;
		void end() override;

		void submitRect(const Math::Vec2f(&positions)[4], const Math::Vec2f(&uvs)[4]) override;
		void bindColor(const Math::Color& color) override;

	private:
		void pushVertex(const Math::Vec2f& position, const Math::Color& color, const unsigned short tid, const Math::Vec2f& uv);

		Shader* m_BatchShader;
		Mesh* m_BatchMesh;
		Math::Color m_BindColor;

		Batch2DVertex* m_pVertices;
		unsigned int m_Count;
	};
} }

#endif