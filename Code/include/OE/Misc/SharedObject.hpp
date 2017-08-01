#ifndef MISC_SHARED_OBJECT_HPP
#define MISC_SHARED_OBJECT_HPP

#include "OE/Misc/SharedReference.hpp"

namespace OrbitEngine {	namespace Misc {
	template<typename T>
	class SharedObject : public SharedReference {
	public:
		T* Get();
		T* GetAndReference();

	protected:
		virtual T* Create() = 0;
		virtual void Destroy(T* obj) = 0;

	private:
		void Init() override;
		void DeInit() override;

		T* m_SharedObject = 0;
	};

	template<typename T>
	inline void SharedObject<T>::Init()
	{
		m_SharedObject = Create();
	}
	
	template<typename T>
	inline void SharedObject<T>::DeInit()
	{
		Destroy(m_SharedObject);
		m_SharedObject = 0;
	}

	template<typename T>
	inline T* SharedObject<T>::Get()
	{
		return m_SharedObject;
	}

	template<typename T>
	inline T* SharedObject<T>::GetAndReference()
	{
		Reference();
		return m_SharedObject;
	}
} }

#endif