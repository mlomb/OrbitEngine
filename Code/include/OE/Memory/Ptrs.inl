#ifndef PTRS_INL
#define PTRS_INL

#include "OE/Memory/Ptrs.hpp"

#include "OE/Meta/NativeReflection.hpp"
#include "OE/Engine/MemoryDomain.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine {

	inline void RefCount::Decr()
	{
		strong--;
		if (strong + weak == 0) Destroy();
	}

	inline void RefCount::Incr()
	{
		strong++;
	}

	inline void RefCount::WDecr()
	{
		weak--;
		if (strong + weak == 0) Destroy();
	}

	inline void RefCount::WIncr()
	{
		weak++;
	}

	inline void RefCount::Destroy()
	{
		delete this;
	}


	template<typename T>
	inline Ptr<T>::Ptr(T* ptr, Meta::Type* type, RefCount* ref_count)
		: m_Ptr(ptr), m_Type(type), m_RefCount(ref_count)
	{
	}

	template<typename T>
	inline Ptr<T>::~Ptr()
	{
	}

	template<typename T>
	inline Meta::Type* Ptr<T>::GetType() const
	{
		return m_Type;
	}

	template<typename T>
	inline void Ptr<T>::Clear()
	{
		m_RefCount = 0;
		m_Type = 0;
		m_Ptr = 0;
	}

	template<typename T>
	inline void Ptr<T>::BuildFromPtr(const Ptr<T>& ptr)
	{
		Clear();

		m_Ptr = ptr.m_Ptr;
		m_Type = ptr.m_Type;
		m_RefCount = ptr.m_RefCount;
	}

	template<typename T>
	inline T* Ptr<T>::Get() const
	{
		if (m_RefCount && m_RefCount->strong)
			return m_Ptr;
		return nullptr;
	}

	template<typename T>
	inline T* Ptr<T>::operator->() const
	{
		return Get();
	}

	template<typename T>
	inline Ptr<T>::operator bool() const
	{
		return Get() != nullptr;
	}

	template<typename T>
	inline bool Ptr<T>::operator==(const Ptr& rhs) const
	{
		return m_Ptr == rhs.m_Ptr && m_RefCount == rhs.m_RefCount;
	}

	template<typename T>
	inline bool Ptr<T>::operator!=(const Ptr& rhs) const
	{
		return !operator==(rhs);
	}

	template<typename T>
	template<typename V>
	inline WeakPtr<V> Ptr<T>::AsWeak()
	{
		return WeakPtr<V>(Ptr<V>(static_cast<V*>(m_Ptr), m_Type, m_RefCount));
	}

	template<typename T>
	template<typename V>
	inline StrongPtr<V> Ptr<T>::AsStrong()
	{
		return StrongPtr<V>(Ptr<V>(static_cast<V*>(m_Ptr), m_Type, m_RefCount));
	}


	template<typename T>
	inline StrongPtr<T>::StrongPtr()
		: Ptr<T>(0, 0)
	{
	}

	template<typename T>
	inline StrongPtr<T>::StrongPtr(T* ptr)
		: Ptr<T>(ptr, Meta::NativeTypeResolver<T>::Get())
	{
		if (this->m_Ptr) {
			if (!this->m_RefCount)
				this->m_RefCount = new RefCount();
			this->m_RefCount->Incr();
		}
	}

	template<typename T>
	inline StrongPtr<T>::StrongPtr(const Ptr<T>& ptr)
	{
		BuildFromPtr(ptr);
	}

	template<typename T>
	inline StrongPtr<T>::StrongPtr(const StrongPtr<T>& ptr)
	{
		BuildFromPtr(ptr);
	}

	template<typename T>
	inline StrongPtr<T>::StrongPtr(const WeakPtr<T>& ptr)
	{
		BuildFromPtr(ptr);
	}

	template<typename T>
	inline StrongPtr<T>::StrongPtr(StrongPtr<T>&& mv)
	{
		operator=(mv);
	}

	template<typename T>
	inline StrongPtr<T>::~StrongPtr()
	{
		Clear();
	}

	template<typename T>
	inline StrongPtr<T>& StrongPtr<T>::operator=(const StrongPtr<T>& ptr)
	{
		BuildFromPtr(ptr);
		return *this;
	}

	template<typename T>
	inline StrongPtr<T>& StrongPtr<T>::operator=(const WeakPtr<T>& ptr)
	{
		BuildFromPtr(ptr);
		return *this;
	}

	template<typename T>
	inline StrongPtr<T>& StrongPtr<T>::operator=(StrongPtr<T>&& other)
	{
		if (this == &other)
			return *this;

		Ptr<T>::BuildFromPtr(other);
		other.Ptr<T>::Clear();

		return *this;
	}

	template<typename T>
	inline void StrongPtr<T>::Clear()
	{
		if (this->m_RefCount) {
			if (this->m_RefCount->strong == 1) {
				Engine::MemoryDomain* md = Engine::MemoryDomain::Get();
				if (md && m_Type)
					md->Destroy(*this);
				else
					delete this->m_Ptr;
			}
			this->m_RefCount->Decr();
		}
		Ptr<T>::Clear();
	}

	template<typename T>
	inline void StrongPtr<T>::BuildFromPtr(const Ptr<T>& ptr)
	{
		Ptr<T>::BuildFromPtr(ptr);

		if (this->m_RefCount && this->m_RefCount->strong > 0)
			this->m_RefCount->Incr();
	}


	template<typename T>
	inline WeakPtr<T>::WeakPtr()
		: Ptr<T>()
	{
	}

	template<typename T>
	inline WeakPtr<T>::WeakPtr(const Ptr<T>& ptr)
	{
		BuildFromPtr(ptr);
	}

	template<typename T>
	inline WeakPtr<T>::WeakPtr(const StrongPtr<T>& ptr)
	{
		BuildFromPtr(ptr);
	}

	template<typename T>
	inline WeakPtr<T>::WeakPtr(const WeakPtr<T>& ptr)
	{
		BuildFromPtr(ptr);
	}

	template<typename T>
	inline WeakPtr<T>::WeakPtr(WeakPtr<T>&& mv)
	{
		operator=(mv);
	}

	template<typename T>
	inline WeakPtr<T>::~WeakPtr()
	{
		Clear();
	}

	template<typename T>
	inline WeakPtr<T>& WeakPtr<T>::operator=(const StrongPtr<T>& ptr)
	{
		BuildFromPtr(ptr);
		return *this;
	}

	template<typename T>
	inline WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& ptr)
	{
		BuildFromPtr(ptr);
		return *this;
	}

	template<typename T>
	inline WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>&& other)
	{
		if (this == &other)
			return *this;

		Ptr<T>::BuildFromPtr(other);
		other.Ptr<T>::Clear();

		return *this;
	}

	template<typename T>
	inline void WeakPtr<T>::Clear()
	{
		if (this->m_RefCount)
			this->m_RefCount->WDecr();
		Ptr<T>::Clear();
	}

	template<typename T>
	inline void WeakPtr<T>::BuildFromPtr(const Ptr<T>& ptr)
	{
		Ptr<T>::BuildFromPtr(ptr);

		if (this->m_RefCount)
			this->m_RefCount->WIncr();
	}
}

#endif