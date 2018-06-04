#ifndef ENGINE_SCENE_HPP
#define ENGINE_SCENE_HPP

#include <vector>
#include <unordered_map>

#include "OE/Misc/ObjectPool.hpp"
#include "OE/Misc/Log.hpp"

#include "OE/Engine/Object.hpp"

namespace OrbitEngine {	namespace Engine {
	class SceneObject;

	class Scene : public Object {

	public:
		Scene();
		~Scene();

		SceneObject* GetRoot();

	private:
		SceneObject* m_Root;
	};
} }

#endif