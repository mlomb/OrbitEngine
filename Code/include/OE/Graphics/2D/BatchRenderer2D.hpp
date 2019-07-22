#ifndef GRAPHICS_BATCH_RENDERER2D_HPP
#define GRAPHICS_BATCH_RENDERER2D_HPP

#include "OE/Graphics/2D/Renderer2D.hpp"
#include "OE/Graphics/API/UniformsPack.hpp"

#include "OE/Math/Color.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace Graphics {
	class Shader;
	class Mesh;

	template<typename V>
	class BatchRenderer2D : virtual public Renderer2D {
	public:
		BatchRenderer2D(VertexLayout* layout, Shader* shader, unsigned int batchSize, Topology batchTopology = Topology::TRIANGLES);
		virtual ~BatchRenderer2D();

		void begin() override;
		void end() override;

		void pushVertex(const V& v);

	protected:
		VertexLayout* m_VertexLayout;
		Shader* m_Shader;
		Mesh* m_Mesh;

		V* m_pVertices;
		unsigned int m_Count;
	};


	template<typename V> inline BatchRenderer2D<V>::BatchRenderer2D(VertexLayout* layout, Shader* shader, unsigned int batchSize, Topology batchTopology)
		: m_VertexLayout(layout), m_Shader(shader), m_pVertices(NULL), m_Count(0)
	{
		std::vector<V> vertices = std::vector<V>(batchSize);
		std::vector<unsigned short> indices = Mesh::GenerateIndices(batchTopology, batchSize);

		m_Mesh = Mesh::Create(vertices, layout, indices);
		m_Mesh->setTopology(batchTopology);
	}

	template<typename V> inline BatchRenderer2D<V>::~BatchRenderer2D()
	{
		delete m_VertexLayout;
		delete m_Shader;
		delete m_Mesh;
	}

	template<typename V> inline void BatchRenderer2D<V>::pushVertex(const V& v)
	{
		*m_pVertices = v;
		m_pVertices++;
	}

	template<typename V> inline void BatchRenderer2D<V>::begin()
	{
		Renderer2D::begin();

		// Map the pointer
		m_pVertices = (V*)m_Mesh->getVBO()->mapPointer(MappedPointerMode::WRITE);
		OE_ASSERT_MSG(m_pVertices, "Can't map batch renderer's pointer!");
		m_Count = 0;
	}

	template<typename V> inline void BatchRenderer2D<V>::end()
	{
		Renderer2D::end();

		m_Mesh->getVBO()->unmapPointer();
		m_Shader->bind();

		m_UPVMatrix->bind(0, Graphics::ShaderType::VERTEX);
	}
} }

#endif