#ifndef GRAPHICS_DEFERRED_RENDERER_HPP
#define GRAPHICS_DEFERRED_RENDERER_HPP

#include "OE/Graphics/3D/Renderer3D.hpp"

#include "OE/Graphics/API/States.hpp"
#include "OE/Graphics/API/FrameBuffer.hpp"

namespace OrbitEngine { namespace Graphics {
	struct DeferredLightUniforms {
		Math::Vec3f camPosition;
		float pad0;
	};

	class DeferredRenderer : public Renderer3D {
	public:
		DeferredRenderer();

		void end() override;

		FrameBuffer* getGBuffer() const { return m_GBuffer; };
		static Texture* GetIntegratedBRDFLUT() { return s_IntegratedBRDFLUT->getColorTextures()[0]; }
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
		DeferredLightUniforms m_DeferredLightUniform;
		Graphics::UniformsPack<PVMatrices>* m_PVMatrices;
		Graphics::UniformsPack<MMatrix>* m_MMatrix;
		Graphics::UniformsPack<DeferredLightUniforms>* m_DeferredLightUniforms;

		// PBR
		static Graphics::FrameBuffer* s_IntegratedBRDFLUT;

		/*
		//std::vector<Light*> lights;
		*/
	};
} }

#endif