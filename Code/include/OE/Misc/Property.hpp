#ifndef MISC_PROPERTY_HPP
#define MISC_PROPERTY_HPP

#include <mutex>

#include "OE/Config.hpp"

#include "OE/Application/EventHandler.hpp"
#include "OE/Misc/OEObject.hpp"

namespace OrbitEngine {	namespace Misc {

	class REFLECT PropertyBase : public OEObject {
		OEOBJECT(PropertyBase)

	public:
		Application::EventHandler<> onModification;

		virtual void* getPtr() = 0;

	protected:
		void fireModification();
	};

	template<typename T>
	class REFLECT Property : public PropertyBase {
		OEOBJECT(Property<T>)

	public:
		Property();
		Property(T obj);

		Property& operator=(const T& value);

		operator T() const;

		void* getPtr() override {
			return static_cast<void*>(&m_Obj);
		}

	private:
		T m_Obj;
	};
	
	template<typename T>
	inline Property<T>::Property()
		: m_Obj()
	{
	}

	template<typename T>
	inline Property<T>::Property(T obj)
		: m_Obj(obj)
	{
	}

	template<typename T>
	inline Property<T> &Property<T>::operator=(const T& value) {
		m_Obj = value;
		fireModification();
		return *this;
	}

	template<typename T>
	inline Property<T>::operator T() const
	{
		return m_Obj;
	}
} }

#endif