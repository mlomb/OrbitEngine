#ifndef APPLICATION_WINDOW_CLASS_HPP
#define APPLICATION_WINDOW_CLASS_HPP

#include "OE/Platform/Windows/Windows.hpp"
#include "OE/Misc/SharedReference.hpp"

#define OE_WNDCLASSNAME "OrbitEngine-Window"

namespace OrbitEngine {	namespace Application { namespace priv {
	class WindowClass : public Misc::SharedReference {
	private:
		void Init() override;
		void DeInit() override;
	};
} } }

#endif