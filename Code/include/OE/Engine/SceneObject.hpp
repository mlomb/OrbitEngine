#ifndef ENGINE_SCENE_OBJECT_HPP
#define ENGINE_SCENE_OBJECT_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

#include <MetaCPP/TypeID.hpp>
#include <MetaCPP/TypeInfo.hpp>
#include <MetaCPP/Runtime.hpp>

#include "OE/Config.hpp"
#include "OE/Misc/Log.hpp"
#include "OE/Misc/Property.hpp"
#include "OE/Misc/OEObject.hpp"
#include "OE/Engine/Scene.hpp"
#include "OE/Engine/Component.hpp"

namespace OrbitEngine {	namespace Engine {

	class REFLECT SceneObject : public Misc::OEObject {
		OEOBJECT(SceneObject)

	public:
		SceneObject();
		~SceneObject();

		SceneObject* addChildren(const std::string& name);

		template<typename T>
		T* addComponent();

		Scene* getScene();
		SceneObject* getParent();
		SceneObject* childAt(int i);
		int childCount() const;
		std::vector<SceneObject*>& getChildrens();
		std::vector<Component*>& getComponents();
		std::string getName();
		void setName(const std::string& name);

		template<typename T>
		T* getComponent();

		Misc::Property<std::string> m_Name;
	private:
		friend class Scene;

		Scene* m_Scene;
		SceneObject* m_Parent;
		std::vector<SceneObject*> m_Childs;
		std::vector<Component*> m_Components;

#if OE_EDITOR
	public:
		void* internal_EditorIndex = 0;
		std::function<void(int, SceneObject*)> internal_EditorBeginInsert;
		std::function<void()> internal_EditorEndInsert;
#endif
	};

	template<typename T>
	inline T* SceneObject::addComponent()
	{
		T* component = m_Scene->allocateObject<T>();
		component->p_SceneObject = this;
		m_Components.push_back(component);
		return component;
	}

	template<typename T>
	inline T* SceneObject::getComponent()
	{
		if (m_Components.size() > 0) {
			return static_cast<T*>(m_Components[0]); // for now
		}
		return 0;
	}
} }

#endif