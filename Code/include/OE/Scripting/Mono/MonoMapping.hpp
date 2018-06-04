#pragma once

#include "OE/Scripting/Mono/MonoHelpers.hpp"

// Be careful with your eyes.

namespace OrbitEngine {
	class MonoAssembly;
	namespace Engine {
		class Object;
	};

	class MonoMapping {
	public:
		/* SHOULD ONLY BE CALLED ONCE AFTER LOADING OrbitEngine.dll */
		static void Map(MonoAssembly* assembly);

		/* Declarations */
		MONO_DEC_CLASS(Object)
			MONO_DEC_FIELD(Object, pointer)
			
		// Scene
		MONO_DEC_CLASS(Scene)

		// SceneObject
		MONO_DEC_CLASS(SceneObject)

		// EngineDomain
		MONO_DEC_CLASS(EngineDomain)
	private:
		MonoMapping() { };
	};
}