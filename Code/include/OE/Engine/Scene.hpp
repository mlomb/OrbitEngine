#ifndef ENGINE_SCENE_HPP
#define ENGINE_SCENE_HPP

#include <vector>

#include "OE/Misc/OEObject.hpp"
#include "OE/Misc/ObjectPool.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine {	namespace Engine {
	class SceneObject;

	class REFLECT Scene : public Misc::OEObject {
		OEOBJECT(Scene)

	public:
		Scene();
		~Scene();

		SceneObject* getRoot();

		template<typename T>
		T* allocateObject();
		template<typename T>
		void deallocateObject(T* obj);

		template<typename T>
		std::pair<typename std::vector<void*>::const_iterator, typename std::vector<void*>::const_iterator> getObjects();

		std::unordered_map<metacpp::TypeID, Misc::TrackedMemoryPool*>& getAllObjects();

	private:
		SceneObject* m_Root;
		std::unordered_map<metacpp::TypeID, Misc::TrackedMemoryPool*> m_Objects;
	};

	template<typename T>
	inline T* Scene::allocateObject()
	{
		metacpp::TypeID id = metacpp::TypeInfo<T>::ID;
		if (id != 0) {
			Misc::ObjectPool<T>* pool;

			auto it = m_Objects.find(metacpp::TypeInfo<T>::ID);
			if (it != m_Objects.end())
				pool = static_cast<Misc::ObjectPool<T>*>((*it).second);
			else {
				pool = new Misc::ObjectPool<T>(80);
				m_Objects.insert(std::make_pair(id, pool));
			}

			return pool->acquire();
		}
		else {
			OE_LOG_WARNING("Trying to allocate an invalid object");
			return NULL;
		}
	}

	template<typename T>
	inline void Scene::deallocateObject(T* obj)
	{
		auto it = m_Objects.find(metacpp::TypeInfo<T>::ID);
		if (it != m_Objects.end()) {
			Misc::ObjectPool<T>* pool = static_cast<Misc::ObjectPool<T>*>(*it);
			return pool->release(obj);
		}
		else {
			OE_LOG_WARNING("Trying to deallocate an invalid object");
		}
	}
	template<typename T>
	inline std::pair<typename std::vector<void*>::const_iterator, typename std::vector<void*>::const_iterator> Scene::getObjects()
	{
		auto it = m_Objects.find(metacpp::TypeInfo<T>::ID);
		if (it != m_Objects.end()) {
			Misc::TrackedMemoryPool* pool = (*it).second;
			return std::make_pair(pool->begin(), pool->end());
		}
		return std::pair<typename std::vector<void*>::const_iterator, typename std::vector<void*>::const_iterator>();
	}
} }

#endif