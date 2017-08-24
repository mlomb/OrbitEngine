#ifndef GRAPHICS_DEFERRED_RENDERER_HPP
#define GRAPHICS_DEFERRED_RENDERER_HPP

#include "OE/Graphics/3D/Renderer3D.hpp"

#include "OE/Graphics/API/States.hpp"
#include "OE/Graphics/API/FrameBuffer.hpp"

namespace OrbitEngine { namespace Graphics {
	class DeferredRenderer : public Renderer3D {
	public:
		DeferredRenderer();
		virtual ~DeferredRenderer();

		void end() override;

		FrameBuffer* getGBuffer() const { return m_GBuffer; };
	protected:
		FrameBuffer* generateGBuffer(const Math::Vec2i size);

		void geometryPass();
		void lightningPass();
		void skyboxPass();
		//void shadowsPass();

		ManagedShader* m_DeferredLightShader;
		FrameBuffer* m_GBuffer = 0;
	};
} }

#endif