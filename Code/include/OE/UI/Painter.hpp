#ifndef UI_PAINTER_HPP
#define UI_PAINTER_HPP

#include "OE/Math/Color.hpp"
#include "OE/Math/Mat4.hpp"

#include "OE/Graphics/API/Mesh.hpp"
#include "OE/Graphics/API/Shader.hpp"
#include "OE/Graphics/API/UniformsPack.hpp"
#include "OE/Graphics/Font.hpp"
#include "OE/Graphics/DynamicAtlas.hpp"

namespace OrbitEngine { namespace UI {

	struct Vertex {
		Math::Vec2f pos;
		Math::Color4f col;
		Math::Vec2f uv;
		float flags;
	};

	enum class Corner {
		TOP_RIGHT = 0,
		TOP_LEFT = 1,
		BOTTOM_LEFT = 2,
		BOTTOM_RIGHT = 3
	};

	struct RoundedRectParams {
		Math::Vec2f cornerRadii[4];
	};

	// TODO: move from immediate to retained eventually
	class Painter {
	public:
		Painter();
		~Painter();


		void drawRectangle(const Math::Rectf& rect, const Math::Color4f& color);
		void drawRoundedCorner(const Math::Vec2f& center, const Math::Vec2f& radii, const Corner corner, const Math::Color4f& color);
		void drawRoundedRectangle(const Math::Rectf& rect, const Math::Color4f& color, const RoundedRectParams& params);

		void drawText(const Graphics::TextLayout& textLayout, const Math::Vec2f& position);

	private:
		struct ConstantBuffer {
			Math::Mat4 pr_matrix;
		};

		Graphics::Shader* m_Shader;
		Graphics::Mesh* m_Mesh;
		Graphics::UniformsPack<ConstantBuffer>* m_ConstantBuffer;

		Vertex* m_pVertex;
		uint16_t* m_pIndex;
		unsigned int m_IndexCount;
		unsigned int m_VertexCount;
		ConstantBuffer m_ConstantBufferData;

		Graphics::DynamicAtlas* m_Atlas;
		std::vector<Graphics::Texture*> m_DestroyAfterCommitingAtlas;

	private:
		friend class Composer;

		void setProjection(const Math::Mat4& proj);
		void begin();
		void flush();
		void end();
	};

} }

#endif