#ifndef GRAPHICS_DYNAMIC_ATLAS
#define GRAPHICS_DYNAMIC_ATLAS

#include <unordered_map>

#include "OE/Math/Rect.hpp"

namespace OrbitEngine { namespace Graphics {

	class FrameBuffer;
	class Texture;
	class DynamicAtlasAllocator;
	class TextureBlitter;

	class DynamicAtlas {
	public:
		typedef uint32_t Index;

		DynamicAtlas();
		~DynamicAtlas();

		bool tryGet(Index index, Math::Recti& rect) const;
		Texture* getTexture() const;
		unsigned int getSize() const;

		bool add(Index index, Math::Recti& rect, Texture* texture);

		void commit();

	private:
		DynamicAtlasAllocator* m_Allocator;
		TextureBlitter* m_TextureBlitter;

		std::unordered_map<Index, Math::Recti> m_Entries;

		FrameBuffer* m_AtlasRT;
	};

} }

#endif