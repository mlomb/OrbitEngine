#ifndef PTRS_HPP
#define PTRS_HPP

#include "OE/Meta/NativeReflection.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine {
	namespace Engine {
		class MemoryDomain;
	}

	namespace internal {
		struct RefCount {
			// TODO atomic
			unsigned int strong = 0;
			unsigned int weak = 0;

			void Decr();
			void Incr();
			void WDecr();
			void WIncr();

			void Destroy();
		};
	}

	template<typename T>
	class Ptr {
	public:
		Ptr(T* ptr = 0, Engine::MemoryDomain* domain = 0);
		virtual ~Ptr();

		Meta::NativeType* GetType() const;
		virtual void Clear();

		T* Get() const;
		T& operator*() const;
		T* operator->() const;
		operator bool() const;

		bool operator==(const Ptr& rhs) const;
		bool operator!=(const Ptr& rhs) const;

	protected:
		internal::RefCount* m_RefCount;
		T* m_Ptr;
		Engine::MemoryDomain* m_Domain;

		template<class T> friend class StrongPtr;
		template<class T> friend class WeakPtr;
	};
	
	template<typename T>
	class StrongPtr : public Ptr<T> {
	public:
		StrongPtr(T* ptr = 0, Engine::MemoryDomain* domain = 0);
		StrongPtr(StrongPtr<T>&& mv);
		StrongPtr(const StrongPtr<T>& ptr);
		StrongPtr(const WeakPtr<T>& ptr);
		~StrongPtr();

		StrongPtr<T>& operator=(const StrongPtr<T>& ptr);
		StrongPtr<T>& operator=(const WeakPtr<T>& ptr);

		StrongPtr<T>& operator=(StrongPtr<T>&& other);

		void Clear() override;

	private:
		void BuildFromPtr(const Ptr<T>& ptr);
	};

	template<typename T>
	class WeakPtr : public Ptr<T> {
	public:
		WeakPtr(const StrongPtr<T>& ptr);
		WeakPtr(const WeakPtr<T>& ptr);
		WeakPtr();
		~WeakPtr();

		WeakPtr<T>& operator=(const StrongPtr<T>& ptr);
		WeakPtr<T>& operator=(const WeakPtr<T>& ptr);

	private:
		void BuildFromPtr(const Ptr<T>& ptr);
	};
}

#include "OE/Misc/Ptrs.inl"

#endif