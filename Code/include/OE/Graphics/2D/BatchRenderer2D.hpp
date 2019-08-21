#ifndef GRAPHICS_BATCH_RENDERER2D_HPP
#define GRAPHICS_BATCH_RENDERER2D_HPP

#include "OE/Graphics/2D/Renderer2D.hpp"
#include "OE/Graphics/API/UniformsPack.hpp"
#include "OE/Graphics/API/VertexLayout.hpp"

#include "OE/Math/Color.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace Graphics {
	class Shader;
	class Mesh;

	template<typename V>
	class BatchRenderer2D : virtual public Renderer2D {
	public:
		BatchRenderer2D(VertexLayout* layout, unsigned int batchSize, Topology batchTopology = Topology::TRIANGLES);
		virtual ~BatchRenderer2D();

		void begin() override;
		void end() override;

		void pushVertex(const V& v);
		unsigned int getVertexCount() const;

	protected:
		VertexLayout* m_VertexLayout;
		Mesh* m_Mesh;

	private:
		V* m_pVertices;
		unsigned int m_Count;
		unsigned int m_BatchSize;
	};


	template<typename V> inline BatchRenderer2D<V>::BatchRenderer2D(VertexLayout* layout, unsigned int batchSize, Topology batchTopology)
		: m_VertexLayout(layout), m_pVertices(NULL), m_Count(0), m_BatchSize(batchSize)
	{
		std::vector<V> vertices = std::vector<V>(batchSize);
		std::vector<unsigned short> indices = Mesh::GenerateIndices(batchTopology, batchSize);

		m_Mesh = Mesh::Create(vertices, layout, indices);
		m_Mesh->setTopology(batchTopology);
	}

	template<typename V> inline BatchRenderer2D<V>::~BatchRenderer2D()
	{
		delete m_VertexLayout;
		delete m_Mesh;
	}

	template<typename V> inline void BatchRenderer2D<V>::pushVertex(const V& v)
	{
		OE_ASSERT(m_Count < m_BatchSize);

		*m_pVertices = v;
		m_pVertices++;
		m_Count++;
	}

	template<typename V> unsigned int BatchRenderer2D<V>::getVertexCount() const
	{
		return m_Count;
	}

	template<typename V> inline void BatchRenderer2D<V>::begin()
	{
		Renderer2D::begin();

		// Map the pointer
		m_pVertices = (V*)m_Mesh->getVBO()->mapPointer(MappedPointerMode::WRITE);
		m_Count = 0;
		OE_ASSERT_MSG(m_pVertices, "Can't map batch renderer's vertex pointer!");
	}

	template<typename V> inline void BatchRenderer2D<V>::end()
	{
		Renderer2D::end();

		m_Mesh->getVBO()->unmapPointer();
	}
} }

#endif