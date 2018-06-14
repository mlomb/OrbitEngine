#ifndef PTRS_INL
#define PTRS_INL

#include "OE/Misc/Ptrs.hpp"

#include "OE/Engine/MemoryDomain.hpp"

namespace OrbitEngine {

	// Ptr

	template<typename T>
	inline Ptr<T>::Ptr(T* ptr, Engine::MemoryDomain* domain)
		: m_Ptr(ptr), m_RefCount(0), m_Domain(domain)
	{
	}

	template<typename T>
	inline Ptr<T>::~Ptr()
	{
	}

	template<typename T>
	inline Meta::NativeType* Ptr<T>::GetType() const
	{
		return Meta::NativeTypeResolver<T>::Get();
	}

	template<typename T>
	inline void Ptr<T>::Clear()
	{
		m_RefCount = 0;
		m_Ptr = 0;
		m_Domain = 0;
	}

	template<typename T>
	inline T* Ptr<T>::Get() const
	{
		if (m_RefCount && m_RefCount->strong)
			return m_Ptr;
		return nullptr;
	}

	template<typename T>
	inline T& Ptr<T>::operator*() const
	{
		return *Get();
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

	// Strong

	template<typename T>
	inline StrongPtr<T>::StrongPtr(T* ptr, Engine::MemoryDomain* domain)
		: Ptr<T>(ptr, domain)
	{
		if (ptr) {
			m_RefCount = new internal::RefCount();
			m_RefCount->Incr();
		}
	}

	template<typename T>
	inline StrongPtr<T>::StrongPtr(StrongPtr<T>&& mv)
	{
		operator=(mv);
	}

	template<typename T>
	inline StrongPtr<T>::StrongPtr(const StrongPtr<T>& ptr)
	{
		BuildFromPtr(ptr);
	}

	template<typename T>
	inline StrongPtr<T>::StrongPtr(const WeakPtr<T>& ptr)
	{
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

		Clear();

		m_Ptr = other.m_Ptr;
		m_RefCount = other.m_RefCount;
		m_Domain = other.m_Domain;

		other.Ptr<T>::Clear();

		return *this;
	}

	template<typename T>
	inline void StrongPtr<T>::Clear()
	{
		if (m_RefCount) {
			if (m_RefCount->strong == 1) {
				// DELETE
				OE_LOG_DEBUG("DELETE");

				if (m_Domain)
					m_Domain->Deallocate(*this);
				else
					delete m_Ptr;
			}
			m_RefCount->Decr();
		}
		Ptr<T>::Clear();
	}

	template<typename T>
	inline void StrongPtr<T>::BuildFromPtr(const Ptr<T>& ptr)
	{
		Clear();

		m_Ptr = ptr.m_Ptr;
		m_RefCount = ptr.m_RefCount;
		m_Domain = ptr.m_Domain;

		if (m_RefCount && m_RefCount->strong > 0)
			m_RefCount->Incr();
	}

	// Weak

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
	inline WeakPtr<T>::WeakPtr()
		: Ptr()
	{
	}

	template<typename T>
	inline WeakPtr<T>::~WeakPtr()
	{
		if (m_RefCount) {
			m_RefCount->WDecr();
		}
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
	inline void WeakPtr<T>::BuildFromPtr(const Ptr<T>& ptr)
	{
		if (m_RefCount)
			m_RefCount->WDecr();

		m_Ptr = ptr.m_Ptr;
		m_RefCount = ptr.m_RefCount;
		m_Domain = ptr.m_Domain;

		if (m_RefCount)
			m_RefCount->WIncr();
	}
}

#endif