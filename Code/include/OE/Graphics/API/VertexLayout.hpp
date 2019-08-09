#ifndef GRAPHICS_VERTEX_LAYOUT_HPP
#define GRAPHICS_VERTEX_LAYOUT_HPP

#include <string>
#include <vector>

namespace OrbitEngine { namespace Graphics {
	struct VertexElement {
		std::string name;
		unsigned int type;
		unsigned int size;
		unsigned int count;
		bool normalized;
		unsigned int offset;
	};

	class VertexLayout {
	public:
		VertexLayout();
		~VertexLayout();

		void push(VertexElement attrib);
		template<typename T> void push(const std::string& name, const unsigned int size = 1, const bool normalized = false);

		inline unsigned int getStride() { return m_Stride; };
		inline std::vector<VertexElement>& getElements() { return m_Childrens; };
	protected:
		std::vector<VertexElement> m_Childrens;
		unsigned int m_Stride;
	};
} }

#endif