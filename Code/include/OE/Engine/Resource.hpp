#ifndef ENGINE_RESOURCE_HPP
#define ENGINE_RESOURCE_HPP

#include "OE/Config.hpp"

#include "OE/Misc/OEObject.hpp"
#include "OE/Misc/GUID.hpp"

namespace OrbitEngine {	namespace Engine {

	template<typename T>
	class REFLECT Resource : public Misc::OEObject {
		OEOBJECT(Resource)

	public:
		Resource();
		Resource(Misc::GUID guid);
		~Resource();

		Misc::GUID getGUID() const;
		bool isValid() const;

	private:
		Misc::GUID m_ID;
	};
} }

#endif