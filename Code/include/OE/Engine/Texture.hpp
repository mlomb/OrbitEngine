#ifndef ENGINE_TEXTURE_HPP
#define ENGINE_TEXTURE_HPP

#include "OE/Config.hpp"

#include "OE/Engine/Asset.hpp"

namespace OrbitEngine {	namespace Engine {

	class REFLECT Texture : public Resource<Texture> {
		OEOBJECT(Texture)

	public:
		Texture();
		~Texture();
	};
} }

#endif
