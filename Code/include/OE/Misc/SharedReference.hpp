#ifndef MISC_SHARED_REFERENCE_HPP
#define MISC_SHARED_REFERENCE_HPP

namespace OrbitEngine {	namespace Misc {
	class SharedReference {
	public:
		inline void Reference();
		inline void DeReference();

		inline bool IsInitialized();

	protected:
		virtual void Init() = 0;
		virtual void DeInit() = 0;

	private:
		int m_ReferenceCount = 0;
	};

	inline void SharedReference::Reference()
	{
		if (m_ReferenceCount == 0)
			Init();
		m_ReferenceCount++;
	}

	inline void SharedReference::DeReference()
	{
		m_ReferenceCount--;
		if (m_ReferenceCount == 0)
			DeInit();
	}

	inline bool SharedReference::IsInitialized()
	{
		return m_ReferenceCount != 0;
	}
} }

#endif