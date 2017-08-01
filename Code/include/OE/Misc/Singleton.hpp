#ifndef MISC_SINGLETON_HPP
#define MISC_SINGLETON_HPP

namespace OrbitEngine {	namespace Misc {
	template<typename T>
	class Singleton {
	public:
		static T* Instance();

	protected:
		Singleton() {};
		virtual ~Singleton() {};

	private:
		static T* s_Instance;
	};

	template<typename T>
	inline T* Singleton<T>::Instance()
	{
		if (s_Instance == 0)
			s_Instance = new T;
		return s_Instance;
	}

	template <typename T> T* Singleton<T>::s_Instance = 0;
} }

#endif