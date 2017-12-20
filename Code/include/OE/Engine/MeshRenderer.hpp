#ifndef ENGINE_MESHRENDERER_HPP
#define ENGINE_MESHRENDERER_HPP

#include "OE/Engine/Component.hpp"

namespace OrbitEngine {	namespace Engine {

	class REFLECT MeshRenderer : public Component {
		OEOBJECT(MeshRenderer)

	public:
		MeshRenderer();
		~MeshRenderer();
	};
} }

#endif