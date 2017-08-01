#ifndef GRAPHICS_MAPPED_RESOURCE_HPP
#define GRAPHICS_MAPPED_RESOURCE_HPP

namespace OrbitEngine {	namespace Graphics {
	enum MappedPointerMode {
		READ,
		WRITE
	};

	class MappedResource {
	public:
		virtual void* mapPointer(MappedPointerMode mode) { return 0; };
		virtual void unmapPointer() {};
		virtual void setData(const unsigned int count, const void* data) {};
	protected:
		MappedResource() = default;
	};
} }

#endif