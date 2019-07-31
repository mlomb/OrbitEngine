#ifndef MISC_PIMPL_HPP
#define MISC_PIMPL_HPP

namespace OrbitEngine {	namespace Misc {
	template<typename T>
	class Pimpl {
	public:

		// Internal use only
		T* getPimpl();

	protected:
		Pimpl(T* pimpl = 0);
		virtual ~Pimpl();

		void setPimpl(T* pimpl);

		T* p_Pimpl;
	};

	template<typename T>
	inline Misc::Pimpl<T>::Pimpl(T* pimpl)
		: p_Pimpl(pimpl)
	{
	}

	template<typename T>
	inline Pimpl<T>::~Pimpl()
	{
		delete p_Pimpl;
	}

	template<typename T>
	inline T* Pimpl<T>::getPimpl()
	{
		return p_Pimpl;
	}

	template<typename T>
	inline void Pimpl<T>::setPimpl(T* pimpl)
	{
		p_Pimpl = pimpl;
	}
} }

#endif