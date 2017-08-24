#ifndef GRAPHICS_D3DSTATES_HPP
#define GRAPHICS_D3DSTATES_HPP

#include "OE/Graphics/API/States.hpp"

#include "OE/Platform/Direct3D/Direct3D.hpp"

namespace OrbitEngine { namespace Graphics {
	struct D3DRendererCache {
		// Depth
		FunctionMode depthMode;

		// Stencil
		FunctionMode stencilMode;
		StencilOperation stencilOperation;
	};

	class D3DStates : public States {
	public:
		D3DStates();
		~D3DStates();

		void setBlending(BlendState enabled) override;
		void setCullMode(CullMode cullMode) override;
		void setDepthTest(FunctionMode depthMode) override;
		void setStencil(FunctionMode stencilMode, StencilOperation operation = (StencilOperation)0) override;
		void setScissor(Math::Scissor* scissor) override;

		ID3D11BlendState* getBlendState(BlendState blendState);
		ID3D11RasterizerState* getRasterizerState(CullMode cullMode);
		ID3D11DepthStencilState* getDepthStencilState(FunctionMode depthMode, FunctionMode stencilMode, StencilOperation operation);

	private:
		void setDepthStencilTest(FunctionMode depthMode, FunctionMode stencilMode, StencilOperation operation);

		void CreateBlendState(D3D11_BLEND dst, D3D11_BLEND src, D3D11_COLOR_WRITE_ENABLE colorWrite, ID3D11BlendState** pBlendState);
		void CreateRasterizerState(D3D11_CULL_MODE cull, D3D11_FILL_MODE fill, ID3D11RasterizerState** pRasterizer);
		void CreateDepthStencilState(FunctionMode depthFunction, bool stencil, D3D11_DEPTH_STENCILOP_DESC frontFace, D3D11_DEPTH_STENCILOP_DESC backFace, ID3D11DepthStencilState** pDepthStencil);

	private:
		ID3D11BlendState* m_BlendStates[6]; // BlendState enum size 
		ID3D11RasterizerState* m_RasterizerStates[4]; // CullMode enum size
		ID3D11DepthStencilState* m_DepthStencilStates[9 * 9 * 3]; // depthMode * stencilMode * operation

		D3DRendererCache m_Cache;
	};
} }

#endif