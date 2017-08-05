#ifndef GRAPHICS_D3DCONTEXT_HPP
#define GRAPHICS_D3DCONTEXT_HPP

#include "OE/Platform/Direct3D/D3DStates.hpp"
#include "OE/Platform/Direct3D/D3DDepthTexture.hpp"
#include "OE/Platform/Direct3D/Direct3D.hpp"
#include "OE/Application/ContextImpl.hpp"

namespace OrbitEngine { namespace Application { namespace priv {
	class WindowWindows;

	class D3DContext : public ContextImpl {
	public:
		D3DContext(WindowWindows* window = 0, D3DContext* sharedContext = 0);
		~D3DContext();

		void present() override;
		void makeCurrent(bool active = true) override;
		void resizeContext(Math::Vec2i size) override;
		void setDefaultBackbuffer() override;

		const std::string getName() override { return "Direct3D 11"; }
		const RenderAPI getAPI() override { return RenderAPI::DIRECT3D; }

		ID3D11Device* getDevice();
		ID3D11DeviceContext* getDeviceContext();

		static D3DContext* GetCurrent() { return (D3DContext*)ContextImpl::GetCurrent(); };
	protected:

	private:
		void initializeDeviceAndSwapChain(WindowWindows* window);
		void initializeRenderTargetView();
		void initializeViewport();

		void setViewport();

		IDXGISwapChain* m_DXSwapChain;
		ID3D11Device* m_DXDevice;
		ID3D11DeviceContext* m_DXDeviceContext;

		ID3D11RasterizerState* m_Rasterizer = 0;
		ID3D11RenderTargetView* m_DXRenderTargetView = 0;

		D3D11_VIEWPORT m_Viewport;
		Graphics::D3DDepthTexture* m_DepthStencil;
	};
} } }

#endif