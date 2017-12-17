#include "OE/Misc/OEObject.hpp"

namespace OrbitEngine {	namespace Misc {
	metacpp::TypeID OEObject::getTypeID() const {
		return metacpp::TypeInfo<OEObject>::ID;
	}

	metacpp::Type* OEObject::getType() const
	{
		return metacpp::TypeInfo<OEObject>::TYPE;
	}
} }