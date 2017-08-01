#ifndef GRAPHICS_RENDERER_HPP
#define GRAPHICS_RENDERER_HPP

#include "OE/Math/Mat4.hpp"
#include "OE/Misc/Camera.hpp"

namespace OrbitEngine { namespace Graphics {

	struct PMatrix {
		Math::Mat4 pr_matrix;
	};

	struct VMatrix {
		Math::Mat4 vw_matrix;
	};

	struct MMatrix {
		Math::Mat4 ml_matrix;
	};

	struct PVMatrices {
		PMatrix pmatrix;
		VMatrix vmatrix;
	};


	class Renderer {
	public:
		~Renderer() {};

		virtual void begin() = 0;
		virtual void end() = 0;

		inline void useCamera(Misc::Camera* camera) { p_Camera = camera; };
		inline Misc::Camera* getCamera() { return p_Camera; };

	protected:
		Misc::Camera* p_Camera;

		Renderer() {};
	};
} }

#endif