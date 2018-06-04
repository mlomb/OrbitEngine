#ifndef ENGINE_SCENE_OBJECT_HPP
#define ENGINE_SCENE_OBJECT_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

#include "OE/Meta/NativeReflection.hpp"

#include "OE/Config.hpp"
#include "OE/Misc/Log.hpp"
#include "OE/Engine/Object.hpp"
#include "OE/Engine/Scene.hpp"
#include "OE/Engine/Component.hpp"

namespace OrbitEngine {	namespace Engine {

	struct Test {
		NATIVE_REFLECTION()

	public:
		bool b;
		char c;
		int i;
		unsigned int ui;
		long l;
		long long ll;
		unsigned long ul;
		unsigned long long ull;
		double d;
		float f;
		Math::Vec2i v2i;
		Math::Vec3i v3i;
		Math::Vec4i v4i;
		Math::Vec2f v2f;
		Math::Vec3f v3f;
		Math::Vec4f v4f;
		std::string str;
	};

	class REFLECT SceneObject : public Object {
		NATIVE_REFLECTION()

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
		std::string getName() const;
		void setName(const std::string& name);
		void parent(SceneObject* parent, int position = 99999999);

		template<typename T>
		T* getComponent();

		Test m_Test;
		std::string m_Name;
	private:
		friend class Scene;

		Scene* m_Scene;
		SceneObject* m_Parent;
		std::vector<SceneObject*> m_Childs;
		std::vector<Component*> m_Components;
	};

	template<typename T>
	inline T* SceneObject::addComponent()
	{
		T* component = new T();
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