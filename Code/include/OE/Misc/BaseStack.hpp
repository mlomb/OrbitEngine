#ifndef MISC_BASE_STACK_HPP
#define MISC_BASE_STACK_HPP

namespace OrbitEngine { namespace Misc {
	template <class T>
	class BaseStack {
	public:
		BaseStack();

		void pushStack(const T& obj, const bool override = false);
		void popStack();
		void clearStack();
		unsigned int countStack();
		T& get();
	protected:
		virtual T nextStack(const T& old, const T& added) = 0;
	private:
		std::vector<T> m_Stack;
		unsigned int m_Index = 0;
	};

	template<class T>
	inline BaseStack<T>::BaseStack()
	{
		pushStack(T());
	}

	template<class T>
	inline void BaseStack<T>::pushStack(const T& obj, const bool override)
	{
		if (override || m_Stack.size() == 0)
			m_Stack.push_back(obj);
		else
			m_Stack.push_back(nextStack(m_Stack[m_Stack.size() - 1], obj));
		m_Index = m_Stack.size() - 1;
	}

	template<class T>
	inline void BaseStack<T>::popStack()
	{
		if (m_Stack.size() > 0) {
			m_Stack.pop_back();
			m_Index = m_Stack.size() - 1;
		}
	}

	template<class T>
	inline void BaseStack<T>::clearStack()
	{
		m_Stack.clear();
		m_Index = 0;
	}

	template<class T>
	inline unsigned int BaseStack<T>::countStack()
	{
		return m_Stack.size();
	}

	template<class T>
	inline T& BaseStack<T>::get()
	{
		return m_Stack[m_Index];
	}
} }

#endif