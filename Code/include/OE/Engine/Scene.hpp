#ifndef ENGINE_SCENE_HPP
#define ENGINE_SCENE_HPP

#include <vector>
#include <unordered_map>

#include "OE/Memory/Ptrs.hpp"
#include "OE/Misc/Log.hpp"

#include "OE/Engine/Object.hpp"

namespace OrbitEngine {	namespace Engine {
	class SceneObject;

	class Scene : public Object {
		NATIVE_REFLECTION

	public:
		Scene();
		~Scene();

		WeakPtr<SceneObject> GetRoot();

	private:
		StrongPtr<SceneObject> m_Root;
	};
} }

#endif