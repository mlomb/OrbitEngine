#ifndef OE_OEOBJECT_HPP
#define OE_OEOBJECT_HPP

#include <MetaCPP/TypeInfo.hpp>
#include <MetaCPP/TypeID.hpp>
#include <MetaCPP/Type.hpp>

#include "OE/Config.hpp"

#define OEOBJECT(className) \
public: \
	metacpp::TypeID getTypeID() const { \
		return metacpp::TypeInfo< className >::ID; \
	} \
	metacpp::Type* getType() const { \
		return metacpp::TypeInfo< className >::TYPE; \
	} \
private: \


namespace OrbitEngine {	namespace Misc {
	
	class REFLECT OEObject {
	public:
		virtual metacpp::TypeID getTypeID() const;
		virtual metacpp::Type* getType() const;
	};
} }


#endif