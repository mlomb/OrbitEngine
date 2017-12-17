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
#include "OE/Engine/Component.hpp"

namespace OrbitEngine {	namespace Engine {

	class REFLECT SceneObject : public Misc::OEObject {
		OEOBJECT(SceneObject)

	public:
		SceneObject(const std::string& name);
		~SceneObject();

		void addChildren(SceneObject* sceneObject);
		void addComponent(Component* component);

		SceneObject* parent();
		SceneObject* childAt(int i);
		int childCount() const;
		std::vector<SceneObject*>& getChildrens();
		std::vector<Component*>& getComponents();
		std::string getName();

		Misc::Property<std::string> m_Name;
	private:

		SceneObject* m_Parent;
		std::vector<SceneObject*> m_Childs;
		std::vector<Component*> m_Components;
		bool test;
		double k;

#if OE_EDITOR
	public:
		void* internal_EditorIndex = 0;
		std::function<void(int, SceneObject*)> internal_EditorBeginInsert;
		std::function<void()> internal_EditorEndInsert;
#endif
	};
} }

#endif