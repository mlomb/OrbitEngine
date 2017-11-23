#ifndef ENGINE_SCENE_OBJECT_HPP
#define ENGINE_SCENE_OBJECT_HPP

#include <vector>
#include <string>
#include <functional>

#include "OE/Config.hpp"

namespace OrbitEngine {	namespace Engine {

	class SceneObject {
	public:
		SceneObject(const std::string& name);
		~SceneObject();

		void addChildren(SceneObject* sceneObject);

		SceneObject* parent();
		SceneObject* childAt(int i);
		int childCount() const;
		std::vector<SceneObject*>& getChildrens();
		std::string getName() const;

	private:

		SceneObject* m_Parent;
		std::vector<SceneObject*> m_Childs;
		std::string m_Name;

#if OE_EDITOR
	public:
		void* internal_EditorIndex = 0;
		std::function<void(int, SceneObject*)> internal_EditorBeginInsert;
		std::function<void()> internal_EditorEndInsert;
#endif
	};
} }

#endif