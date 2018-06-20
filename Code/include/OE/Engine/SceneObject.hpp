#ifndef ENGINE_SCENE_OBJECT_HPP
#define ENGINE_SCENE_OBJECT_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

#include "OE/Meta/NativeReflection.hpp"

#include "OE/Config.hpp"
#include "OE/Misc/Log.hpp"
#include "OE/Memory/Ptrs.hpp"
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
		std::string GetName() const;
		WeakPtr<SceneObject> GetChild(int i) const;
		int GetChildCount() const;
		WeakPtr<Component> GetComponent(int i) const;
		int GetComponentCount() const;
		
		WeakPtr<SceneObject> AddChildren();
		template<typename T>
		WeakPtr<T> AddComponent();
		void SetName(const std::string& name);
		void SetParent(WeakPtr<SceneObject> parent, int position = 99999999);


	private:
		friend class Scene;
		
		WeakPtr<SceneObject> m_Self;
		WeakPtr<SceneObject> m_Parent;
		WeakPtr<Scene> m_Scene;

		std::string m_Name;
		std::vector<StrongPtr<SceneObject>> m_Childs;
		std::vector<StrongPtr<Component>> m_Components;
	};

	template<typename T>
	inline WeakPtr<T> SceneObject::AddComponent()
	{
		StrongPtr<T> ptr = MemoryDomain::Get()->Create<T>();
		StrongPtr<Component> comp = ptr.template AsStrong<Component>();
		comp->m_SceneObject = m_Self;
		m_Components.push_back(comp);
		return ptr;
	}
} }

#endif