#include "OE/Platform/Direct3D/D3DContext.hpp"

#include "OE/Platform/Windows/WindowWindows.hpp"
#include "OE/Platform/Direct3D/D3DFrameBuffer.hpp"
#include "OE/Platform/Direct3D/D3DRenderTexture.hpp"
#include "OE/Graphics/API/FrameBuffer.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Application { namespace priv {

	D3DContext::D3DContext(WindowWindows* window, D3DContext* sharedContext)
		: ContextImpl(window), m_Factory(NULL), m_DXDevice(NULL), m_DXDeviceContext(NULL), m_DXSwapChain(NULL)
	{
		HRESULT hr;

		if (sharedContext == NULL) {
			m_IsShared = false;

			// Create a new DXGIFactory
			hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_Factory);
			if (FAILED(hr)) {
				OE_LOG_FATAL("Can't create DXGIFactory!");
				return;
			}

			UINT flags = 0;
#ifdef OE_DEBUG
			//flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			// Create a new Device
			hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, nullptr, 0, D3D11_SDK_VERSION, &m_DXDevice, NULL, &m_DXDeviceContext);
			if (FAILED(hr)) {
				OE_LOG_FATAL("Can't create D3D device!");
			}
		}
		else {
			// copy factory and device from the other context
			m_IsShared = true;
			m_Factory = sharedContext->m_Factory;
			m_DXDevice = sharedContext->m_DXDevice;
			m_DXDeviceContext = sharedContext->m_DXDeviceContext;
		}

		makeCurrent();

		//////////////////////////
		// Initialize SwapChain //
		//////////////////////////
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(scd));

		scd.BufferCount = 1;
		scd.BufferDesc.Width = window->getSize().x;
		scd.BufferDesc.Height = window->getSize().y;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = 0;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = window->getWindowNativeHandle();
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.Windowed = 1;
		//scd.Windowed = !(m_Window->getProperties().displayMode == DisplayMode::FULLSCREEN);
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = 0;

		/*
		if (m_Window->getProperties().vsync)
		{
		scd.BufferDesc.RefreshRate.Numerator = 0; // TODO CHANGE
		scd.BufferDesc.RefreshRate.Denominator = 1;
		}
		*/

		hr = m_Factory->CreateSwapChain(m_DXDevice, &scd, &m_DXSwapChain);
		if (FAILED(hr))
			OE_LOG_FATAL("Can't create SwapChain.");

		resizeContext(Math::Vec2i(100, 100));

		contextInitialized();

		return;

		/*
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		DXGI_ADAPTER_DESC adapterDesc;

		unsigned int numModes, numerator = 0, denominator = 0;
		DXGI_MODE_DESC* displayModeList;

		hr = factory->EnumAdapters(0, &adapter);
		if (FAILED(hr)) {
			OE_LOG_FATAL("Can't EnumAdapters!");
			return;
		}

		hr = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(hr)) {
			OE_LOG_FATAL("Can't EnumOutputs!");
			return;
		}

		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, 0);
		if (FAILED(hr)) {
			OE_LOG_FATAL("Can't retrive the number of display modes!");
			return;
		}

		displayModeList = new DXGI_MODE_DESC[numModes];
		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(hr)) {
			OE_LOG_FATAL("Can't retrive the display mode list!");
			return;
		}

		for (unsigned int i = 0; i < numModes; ++i) {
			if (displayModeList[i].Width == m_View->getWidth() && displayModeList[i].Height == m_View->getHeight()) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}

		delete[] displayModeList;
		displayModeList = 0;

		//if (numerator == 0 && denominator == 0)
		//	OE_LOG_WARNING("Can't find a situable display mode!");

		hr = adapter->GetDesc(&adapterDesc);
		if (FAILED(hr)) {
			OE_LOG_FATAL("Can't retrive the description of the adapter!");
			return;
		}

		unsigned int videoCardMemory = adapterDesc.DedicatedVideoMemory / 1024 / 1024;
		std::wstring description = std::wstring(adapterDesc.Description);
		std::string videoCardDescription = std::string(description.begin(), description.end());
		
		OE_LOG_INFO("Direct3D 11");
		OE_LOG_INFO("GPU " + videoCardDescription);
		OE_LOG_INFO("GPU Memory " + std::to_string(videoCardMemory) + "MB");

		OE_D3D_RELEASE(adapterOutput);
		OE_D3D_RELEASE(adapter);
		*/
	}

	void D3DContext::initializeDefaultFramebuffer()
	{
		m_DXDeviceContext->OMSetRenderTargets(0, 0, 0);

		if (p_DefaultFramebuffer) {
			delete p_DefaultFramebuffer;
			p_DefaultFramebuffer = 0;
		}

		HRESULT hr;
		ID3D11Texture2D* backBuffer;

		hr = m_DXSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& backBuffer);
		if (FAILED(hr)) {
			OE_LOG_FATAL("Can't get the pointer to the backbuffer!");
			return;
		}

		p_DefaultFramebuffer = Graphics::FrameBuffer::Create(p_Size.x, p_Size.y);

		Graphics::D3DFrameBuffer* fb = static_cast<Graphics::D3DFrameBuffer*>(p_DefaultFramebuffer);
		Graphics::TextureProperties properties;
		properties.width = p_Size.x;
		properties.height = p_Size.y;
		fb->attachColorTexture(new Graphics::D3DRenderTexture(properties, backBuffer));

		p_DefaultFramebuffer->attachDepthStencilTexture(false);
		p_DefaultFramebuffer->finalize();
		p_DefaultFramebuffer->setViewport();
	}

	void D3DContext::resizeContext(Math::Vec2i size)
	{
		if (p_Size == size)
			return;

		ContextImpl::resizeContext(size);

		if (p_DefaultFramebuffer) {
			m_DXDeviceContext->OMSetRenderTargets(0, 0, 0);
			delete p_DefaultFramebuffer;
			p_DefaultFramebuffer = 0;
		}

		HRESULT hr;
		hr = m_DXSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		if (FAILED(hr)) {
			OE_LOG_FATAL("Can't resize the swap chain!" << hr);
			return;
		}

		initializeDefaultFramebuffer();
	}


	D3DContext::~D3DContext()
	{
		if (m_DXSwapChain) {
			m_DXSwapChain->SetFullscreenState(false, NULL);
			OE_D3D_RELEASE(m_DXSwapChain);
		}
		m_DXDeviceContext->OMSetRenderTargets(0, 0, 0);
		OE_D3D_RELEASE(m_Rasterizer);
		if (p_DefaultFramebuffer) {
			delete p_DefaultFramebuffer;
			p_DefaultFramebuffer = 0;
		}
		if (!m_IsShared) {
			OE_D3D_RELEASE(m_DXDevice);
			OE_D3D_RELEASE(m_DXDeviceContext);
			OE_D3D_RELEASE(m_Factory);
		}
	}

	void D3DContext::present()
	{
		m_DXSwapChain->Present(/* m_Window->getProperties().vsync */ false, 0);
		p_DefaultFramebuffer->clear();
	}

	void D3DContext::makeCurrent(bool active)
	{
		ContextImpl::makeCurrent(active);
	}

	ID3D11Device* D3DContext::getDevice()
	{
		return m_DXDevice;
	}

	ID3D11DeviceContext* D3DContext::getDeviceContext()
	{
		return m_DXDeviceContext;
	}
} } }