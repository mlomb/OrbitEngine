#ifndef GRAPHICS_FORWARD_RENDERER_HPP
#define GRAPHICS_FORWARD_RENDERER_HPP

#include "OE/Graphics/3D/Renderer3D.hpp"

namespace OrbitEngine { namespace Graphics {
	class ForwardRenderer : public Renderer3D {
	public:
		ForwardRenderer();
		virtual ~ForwardRenderer();

		void end() override;

	private:
	};
} }

#endif