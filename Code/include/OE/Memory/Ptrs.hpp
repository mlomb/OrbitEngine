#ifndef PTRS_HPP
#define PTRS_HPP

namespace OrbitEngine {
	namespace Meta {
		class NativeType;
	}

	template<typename T>
	class StrongPtr;
	template<typename T>
	class WeakPtr;

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

	template<typename T>
	class Ptr {
	public:

		Ptr(T* ptr = 0, RefCount* ref_count = 0);
		virtual ~Ptr();

		Meta::NativeType* GetType() const;
		virtual void Clear();

		T* Get() const;
		T& operator*() const;
		T* operator->() const;
		operator bool() const;
		bool operator==(const Ptr& rhs) const;
		bool operator!=(const Ptr& rhs) const;

		template<typename V>
		WeakPtr<V> AsWeak();
		template<typename V>
		StrongPtr<V> AsStrong();

	protected:
		RefCount* m_RefCount;
		T* m_Ptr;

		virtual void BuildFromPtr(const Ptr<T>& ptr);

		friend class StrongPtr<T>;
		friend class WeakPtr<T>;
	};

	template<typename T>
	class StrongPtr : public Ptr<T> {
	public:
		StrongPtr(T* ptr = 0);
		StrongPtr(const Ptr<T>& ptr);
		StrongPtr(const StrongPtr<T>& ptr);
		StrongPtr(const WeakPtr<T>& ptr);
		StrongPtr(StrongPtr<T>&& mv);
		~StrongPtr();

		StrongPtr<T>& operator=(const StrongPtr<T>& ptr);
		StrongPtr<T>& operator=(const WeakPtr<T>& ptr);
		StrongPtr<T>& operator=(StrongPtr<T>&& other);

		void Clear() override;

		void BuildFromPtr(const Ptr<T>& ptr) override;
	};

	template<typename T>
	class WeakPtr : public Ptr<T> {
	public:
		WeakPtr();
		WeakPtr(const Ptr<T>& ptr);
		WeakPtr(const StrongPtr<T>& ptr);
		WeakPtr(const WeakPtr<T>& ptr);
		WeakPtr(WeakPtr<T>&& mv);
		~WeakPtr();

		WeakPtr<T>& operator=(const StrongPtr<T>& ptr);
		WeakPtr<T>& operator=(const WeakPtr<T>& ptr);
		WeakPtr<T>& operator=(WeakPtr<T>&& other);

		void Clear() override;

		void BuildFromPtr(const Ptr<T>& ptr) override;
	};
}

#include "OE/Memory/Ptrs.inl"

#endif