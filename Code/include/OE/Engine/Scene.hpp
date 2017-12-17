#ifndef ENGINE_SCENE_HPP
#define ENGINE_SCENE_HPP

#include "OE/Engine/SceneObject.hpp"

namespace OrbitEngine {	namespace Engine {

	class REFLECT Scene : public Misc::OEObject {
		OEOBJECT(Scene)

	public:
		Scene();
		~Scene();

		SceneObject* getRoot();

	private:
		SceneObject* m_Root;
	};
} }

#endif