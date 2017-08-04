#ifndef APPLICATION_X11DISPLAY_HPP
#define APPLICATION_X11DISPLAY_HPP

#include "OE/Platform/X11/X11.hpp"

#include "OE/Misc/SharedObject.hpp"
#include "OE/Misc/Singleton.hpp"

namespace OrbitEngine {	namespace Application { namespace priv {
	class X11Display : public Misc::SharedObject<::Display>, public Misc::Singleton<X11Display> {
	private:
		friend class Misc::Singleton<X11Display>;

		X11Display();

		::Display* Create() override;
		void Destroy(::Display* object) override;
	};
} } }

#endif