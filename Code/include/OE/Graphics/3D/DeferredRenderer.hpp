#ifndef GRAPHICS_DEFERRED_RENDERER_HPP
#define GRAPHICS_DEFERRED_RENDERER_HPP

#include "OE/Graphics/3D/Renderer3D.hpp"

#include "OE/Graphics/API/States.hpp"
#include "OE/Graphics/API/FrameBuffer.hpp"

namespace OrbitEngine { namespace Graphics {
	class DeferredRenderer : public Renderer3D {
	public:
		DeferredRenderer();

		void end() override;

		Graphics::FrameBuffer* getGBuffer() const { return m_GBuffer; };
	protected:
		void generateGBuffer();
		void geometryPass();
		void lightningPass();
		void skyboxPass();

		//void shadowsPass();

		Graphics::Shader* m_GBufferShader;
		Graphics::Shader* m_LighPassShader;
		Graphics::FrameBuffer* m_GBuffer = 0;

		PVMatrices m_MatricesUniforms;
		MMatrix m_MMatrixUniform;
		Graphics::UniformsPack<PVMatrices>* m_PVMatrices;
		Graphics::UniformsPack<MMatrix>* m_MMatrix;

		// PBR
		static Graphics::FrameBuffer* s_IntegratedBRDFLUT;

		/*
		//std::vector<Light*> lights;
		*/
	};
} }

#endif