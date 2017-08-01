#include "OE/Platform/Direct3D/D3DStates.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine {	namespace Graphics {
	D3DStates::D3DStates()
	{
		memset(m_BlendStates, 0, sizeof(m_BlendStates));
		memset(m_RasterizerStates, 0, sizeof(m_RasterizerStates));
		memset(m_DepthStencilStates, 0, sizeof(m_DepthStencilStates));

		m_Cache = { };
	}

	D3DStates::~D3DStates()
	{
		for (int i = 0; i < 4; i++) {
			OE_D3D_RELEASE(m_BlendStates[i])
				OE_D3D_RELEASE(m_RasterizerStates[i])
		}

		for (int i = 0; i < 9 * 9 * 3; i++)
			OE_D3D_RELEASE(m_DepthStencilStates[i])
	}

	void D3DStates::setBlending(BlendState blendState)
	{
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->OMSetBlendState(getBlendState(blendState), NULL, 0xFFFFFFFF);
	}

	void D3DStates::setCullMode(CullMode cullMode)
	{
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->RSSetState(getRasterizerState(cullMode));
	}

	void D3DStates::setDepthTest(FunctionMode depthMode)
	{
		setDepthStencilTest(depthMode, m_Cache.stencilMode, m_Cache.stencilOperation);
	}

	void D3DStates::setStencil(FunctionMode stencilMode, StencilOperation operation)
	{
		setDepthStencilTest(m_Cache.depthMode, stencilMode, operation);
	}

	void D3DStates::setDepthStencilTest(FunctionMode depthMode, FunctionMode stencilMode, StencilOperation operation)
	{
		m_Cache.depthMode = depthMode;
		m_Cache.stencilMode = stencilMode;
		m_Cache.stencilOperation = operation;

		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->OMSetDepthStencilState(getDepthStencilState(depthMode, stencilMode, operation), 0);
	}

	void D3DStates::setScissor(Math::Scissor* scissor)
	{
		D3D11_RECT rects[1];
		if (scissor) {
			rects[0].left = (LONG)scissor->rect.x;
			rects[0].top = (LONG)scissor->rect.y;
			rects[0].right = (LONG)(scissor->rect.x + scissor->rect.z);
			rects[0].bottom = (LONG)(scissor->rect.y + scissor->rect.w);
		}
		else {
			rects[0].left = rects[0].top = 0;
			rects[0].right = rects[0].bottom = 32000;
		}
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->RSSetScissorRects(1, rects);
	}

	ID3D11BlendState* D3DStates::getBlendState(BlendState blendState)
	{
		ID3D11BlendState* bs = m_BlendStates[static_cast<int>(blendState)];
		if (bs != nullptr)
			return bs;

		D3D11_BLEND src;
		D3D11_BLEND dst;
		D3D11_COLOR_WRITE_ENABLE colorWrite = D3D11_COLOR_WRITE_ENABLE_ALL;

		switch (blendState)
		{
		case BlendState::DISABLED:
			src = D3D11_BLEND_ONE;
			dst = D3D11_BLEND_ZERO;
			break;
		case BlendState::NO_COLOR:
			colorWrite = (D3D11_COLOR_WRITE_ENABLE)0;
			src = dst = D3D11_BLEND_ZERO;
			break;
		case BlendState::SRC_ALPHA:
			src = D3D11_BLEND_SRC_ALPHA;
			dst = D3D11_BLEND_INV_SRC_ALPHA;
			break;
		case BlendState::ONE_ALPHA:
			src = D3D11_BLEND_ONE;
			dst = D3D11_BLEND_INV_SRC_ALPHA;
			break;
		default:
			return nullptr;
		}

		CreateBlendState(dst, src, colorWrite, &m_BlendStates[static_cast<int>(blendState)]);
		return m_BlendStates[static_cast<int>(blendState)];
	}

	ID3D11RasterizerState* D3DStates::getRasterizerState(CullMode cullMode)
	{
		ID3D11RasterizerState* rasterizer = m_RasterizerStates[static_cast<int>(cullMode)];
		if (rasterizer != nullptr)
			return rasterizer;

		D3D11_CULL_MODE cull;
		D3D11_FILL_MODE fill = D3D11_FILL_SOLID;

		switch (cullMode)
		{
		case CullMode::WIREFRAME:
			fill = D3D11_FILL_WIREFRAME;
			// fall
		case CullMode::NONE:
			cull = D3D11_CULL_NONE;
			break;
		case CullMode::FRONT:
			cull = D3D11_CULL_FRONT;
			break;
		case CullMode::BACK:
			cull = D3D11_CULL_BACK;
			break;
		default:
			return nullptr;
		}

		CreateRasterizerState(cull, fill, &m_RasterizerStates[static_cast<int>(cullMode)]);
		return m_RasterizerStates[static_cast<int>(cullMode)];
	}

	ID3D11DepthStencilState* D3DStates::getDepthStencilState(FunctionMode depthMode, FunctionMode stencilMode, StencilOperation operation)
	{
		int stateIndex = static_cast<int>(depthMode) * 9 * 3 + static_cast<int>(stencilMode) * 3 + static_cast<int>(operation);

		ID3D11DepthStencilState* depthStencil = m_DepthStencilStates[stateIndex];
		if (depthStencil != nullptr)
			return depthStencil;

		D3D11_DEPTH_STENCILOP_DESC frontOp, backOp;
		ZeroMemory(&frontOp, sizeof(D3D11_DEPTH_STENCILOP_DESC));
		ZeroMemory(&backOp, sizeof(D3D11_DEPTH_STENCILOP_DESC));
		frontOp = backOp = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };

		bool stencilEnabled = stencilMode != FunctionMode::DISABLED;
		if (stencilEnabled) {
			switch (operation)
			{
			case StencilOperation::SEPARATE_INCR_DECR:
				frontOp = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR, D3D11_COMPARISON_ALWAYS };
				backOp = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_DECR, D3D11_COMPARISON_ALWAYS };
				break;
			case StencilOperation::ZERO:
				frontOp = backOp = { D3D11_STENCIL_OP_ZERO, D3D11_STENCIL_OP_ZERO, D3D11_STENCIL_OP_ZERO, (D3D11_COMPARISON_FUNC)((int)stencilMode + 1) };
				break;
			default:
				break;
			}
		}

		CreateDepthStencilState(depthMode, stencilEnabled, frontOp, backOp, &m_DepthStencilStates[stateIndex]);
		return m_DepthStencilStates[stateIndex];
	}

	void D3DStates::CreateBlendState(D3D11_BLEND dst, D3D11_BLEND src, D3D11_COLOR_WRITE_ENABLE colorWrite, ID3D11BlendState** pBlendState)
	{
		D3D11_BLEND_DESC blendStateDescription;
		ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

		blendStateDescription.RenderTarget[0].BlendEnable = colorWrite == D3D11_COLOR_WRITE_ENABLE_ALL && (dst != D3D11_BLEND_ZERO || src != D3D11_BLEND_ONE);
		blendStateDescription.RenderTarget[0].RenderTargetWriteMask = colorWrite;

		blendStateDescription.RenderTarget[0].SrcBlend = src;
		blendStateDescription.RenderTarget[0].DestBlend = dst;
		blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		blendStateDescription.RenderTarget[0].SrcBlendAlpha = blendStateDescription.RenderTarget[0].SrcBlend;
		blendStateDescription.RenderTarget[0].DestBlendAlpha = blendStateDescription.RenderTarget[0].DestBlend;
		blendStateDescription.RenderTarget[0].BlendOpAlpha = blendStateDescription.RenderTarget[0].BlendOp;

		blendStateDescription.IndependentBlendEnable = false;

		HRESULT hr = Application::priv::D3DContext::GetCurrent()->getDevice()->CreateBlendState(&blendStateDescription, pBlendState);
		if (FAILED(hr))
			OE_LOG_FATAL("Can't create blend state!");
	}

	void D3DStates::CreateRasterizerState(D3D11_CULL_MODE cull, D3D11_FILL_MODE fill, ID3D11RasterizerState** pRasterizer)
	{
		D3D11_RASTERIZER_DESC rasterDesc;
		ZeroMemory(&rasterDesc, sizeof(rasterDesc));

		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FrontCounterClockwise = true;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = true;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.SlopeScaledDepthBias = 0.0f;
		rasterDesc.CullMode = cull;
		rasterDesc.FillMode = fill;

		HRESULT hr = Application::priv::D3DContext::GetCurrent()->getDevice()->CreateRasterizerState(&rasterDesc, pRasterizer);
		if (FAILED(hr))
			OE_LOG_FATAL("Can't create rasterizer state!");
	}

	void D3DStates::CreateDepthStencilState(FunctionMode depthFunction, bool stencil, D3D11_DEPTH_STENCILOP_DESC frontFace, D3D11_DEPTH_STENCILOP_DESC backFace, ID3D11DepthStencilState** pDepthStencil)
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		bool depthEnabled = depthFunction != FunctionMode::DISABLED;
		depthStencilDesc.DepthEnable = depthEnabled;
		depthStencilDesc.DepthWriteMask = stencil ? D3D11_DEPTH_WRITE_MASK_ZERO : D3D11_DEPTH_WRITE_MASK_ALL;
		if (depthEnabled)
			depthStencilDesc.DepthFunc = (D3D11_COMPARISON_FUNC)((int)depthFunction + 1);

		depthStencilDesc.StencilEnable = stencil;
		depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		depthStencilDesc.FrontFace = frontFace;
		depthStencilDesc.BackFace = backFace;

		HRESULT hr = Application::priv::D3DContext::GetCurrent()->getDevice()->CreateDepthStencilState(&depthStencilDesc, pDepthStencil);
		if (FAILED(hr))
			OE_LOG_FATAL("Can't create depth stencil view!");
	}
} }