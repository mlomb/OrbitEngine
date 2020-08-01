#ifndef UI_PAINTER_HPP
#define UI_PAINTER_HPP

#include "OE/Math/Color.hpp"
#include "OE/Math/Mat4.hpp"

#include "OE/Graphics/API/Mesh.hpp"
#include "OE/Graphics/API/Shader.hpp"
#include "OE/Graphics/API/UniformsPack.hpp"

namespace OrbitEngine { namespace UI {

	struct Vertex {
		Math::Vec2f pos;
		Math::Color4f col;
	};

	struct ConstantBuffer {
		Math::Mat4 pr_matrix;
	};

	// TODO: move from immediate to retained eventually
	class Painter {
	public:
		Painter();
		~Painter();

		void drawRectangle(const Math::Vec2f& position, const Math::Vec2f& size, const Math::Color4f& color);

	private:
		Graphics::Shader* m_Shader;
		Graphics::Mesh* m_Mesh;
		Graphics::UniformsPack<ConstantBuffer>* m_ConstantBuffer;

		Vertex* m_pVertex;
		uint16_t* m_pIndex;
		unsigned int m_IndexCount;
		unsigned int m_VertexCount;
		ConstantBuffer m_ConstantBufferData;

	private:
		friend class Composer;

		void setProjection(const Math::Mat4& proj);
		void begin();
		void end();
	};

} }

#endif