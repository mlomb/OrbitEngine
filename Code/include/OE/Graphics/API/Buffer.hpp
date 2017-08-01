#ifndef GRAPHICS_BUFFER_HPP
#define GRAPHICS_BUFFER_HPP

#include "OE/Graphics/API/MappedResource.hpp"
#include "OE/Graphics/API/VertexLayout.hpp"

namespace OrbitEngine {	namespace Graphics {
	enum class BufferType {
		VERTEX = 0,
		INDEX,
		CONSTANT
	};

	enum BufferMode {
		DEFAULT = 0,
		DYNAMIC
	};

	class VertexBuffer;

	class Buffer : public MappedResource {
	public:
		virtual void bind() const = 0;
		virtual void resize(const unsigned int size) = 0;
		virtual void setData(const unsigned int size, const void* data) = 0;

		static Buffer* CreateIndexBuffer(const unsigned int size, const void* data = 0);
		static VertexBuffer* CreateVertexBuffer(VertexLayout* layout, const unsigned int size, const void* data = 0);

	protected:
		unsigned int p_Size;

		Buffer();
	};

	class VertexBuffer : public virtual Buffer {
	public:
		VertexLayout* getLayout() { return p_Layout; };
		void setLayout(VertexLayout* layout) { p_Layout = layout; };
	protected:
		VertexLayout* p_Layout;
	};
} }

#endif