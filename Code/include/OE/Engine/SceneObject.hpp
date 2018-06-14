#ifndef ENGINE_SCENE_OBJECT_HPP
#define ENGINE_SCENE_OBJECT_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

#include "OE/Meta/NativeReflection.hpp"

#include "OE/Config.hpp"
#include "OE/Misc/Log.hpp"
#include "OE/Misc/Ptrs.hpp"
#include "OE/Engine/Object.hpp"
#include "OE/Engine/Scene.hpp"
#include "OE/Engine/Component.hpp"

namespace OrbitEngine {	namespace Engine {

	class SceneObject : public Object {
		NATIVE_REFLECTION

	public:
		SceneObject();
		~SceneObject();

		WeakPtr<Scene> GetScene() const;
		WeakPtr<SceneObject> GetParent() const;
		WeakPtr<SceneObject> GetChild(int i) const;
		int GetChildCount() const;
		std::string GetName() const;

		WeakPtr<SceneObject> AddChildren();
		void SetName(const std::string& name);
		void SetParent(WeakPtr<SceneObject> parent, int position = 99999999);

	private:
		friend class Scene;
		
		WeakPtr<SceneObject> m_Self;
		WeakPtr<SceneObject> m_Parent;
		WeakPtr<Scene> m_Scene;

		std::string m_Name;
		std::vector<StrongPtr<SceneObject>> m_Childs;
	};
} }

#endif