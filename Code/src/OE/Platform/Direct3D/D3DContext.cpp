#include "OE/Platform/Direct3D/D3DContext.hpp"

#include "OE/Platform/Windows/WindowWindows.hpp"
#include "OE/Platform/Direct3D/D3DFrameBuffer.hpp"
#include "OE/Platform/Direct3D/D3DRenderTexture.hpp"
#include "OE/Graphics/API/FrameBuffer.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Application { namespace priv {

	D3DContext::D3DContext(WindowWindows* window, D3DContext* sharedContext)
		: ContextImpl(window)
	{
		HRESULT hr;

		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		unsigned int numModes, numerator = 0, denominator = 0;
		DXGI_MODE_DESC* displayModeList;
		DXGI_ADAPTER_DESC adapterDesc;

		hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(hr)) {
			OE_LOG_FATAL("Can't create DXGIFactory!");
			return;
		}

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

		/*
		for (unsigned int i = 0; i < numModes; ++i) {
			if (displayModeList[i].Width == m_View->getWidth() && displayModeList[i].Height == m_View->getHeight()) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
		*/

		//if (numerator == 0 && denominator == 0)
		//	OE_LOG_WARNING("Can't find a situable display mode!");

		hr = adapter->GetDesc(&adapterDesc);
		if (FAILED(hr)) {
			OE_LOG_FATAL("Can't retrive the description of the adapter!");
			return;
		}

		/*
		unsigned int videoCardMemory = adapterDesc.DedicatedVideoMemory / 1024 / 1024;
		std::wstring description = std::wstring(adapterDesc.Description);
		std::string videoCardDescription = std::string(description.begin(), description.end());
		
		OE_LOG_INFO("Direct3D 11");
		OE_LOG_INFO("GPU " + videoCardDescription);
		OE_LOG_INFO("GPU Memory " + std::to_string(videoCardMemory) + "MB");
		*/
		
		delete[] displayModeList;
		displayModeList = 0;

		OE_D3D_RELEASE(adapterOutput);
		OE_D3D_RELEASE(adapter);
		OE_D3D_RELEASE(factory);

		makeCurrent();
		initializeDeviceAndSwapChain(window);
		resizeContext(Math::Vec2i(100, 100));

		contextInitialized();
	}

	void D3DContext::initializeDeviceAndSwapChain(WindowWindows* window)
	{
		HRESULT hr;
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(scd));

		scd.BufferCount = 1;
		scd.BufferDesc.Width = window->getProperties().resolution.x;
		scd.BufferDesc.Height = window->getProperties().resolution.y;
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

		UINT flags = 0;
#ifdef OE_DEBUG
		//flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, nullptr, 0, D3D11_SDK_VERSION, &scd, &m_DXSwapChain, &m_DXDevice, NULL, &m_DXDeviceContext);
		if (FAILED(hr))
			OE_LOG_FATAL("Can't create device and swap chain.");
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
		properties.formatProperties.width = p_Size.x;
		properties.formatProperties.height = p_Size.y;

		fb->attachColorTexture(new Graphics::D3DRenderTexture(properties, backBuffer));

		properties.formatProperties.format = Graphics::TextureFormat::DEPTH;
		properties.formatProperties.width = p_Size.x;
		properties.formatProperties.height = p_Size.y;

		fb->attachDepthTexture(properties.formatProperties);
		fb->finalize();

		p_DefaultFramebuffer->setViewport();
	}

	/*
	void D3DContext::initializeRenderTargetView()
	{
		m_DXDeviceContext->OMSetRenderTargets(0, 0, 0);
		OE_D3D_RELEASE(m_DXRenderTargetView);
		delete m_DepthStencil;

		HRESULT hr;
		ID3D11Texture2D* backBuffer;

		hr = m_DXSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		if (FAILED(hr)) {
			OE_LOG_FATAL("Can't get the pointer to the backbuffer!");
			return;
		}
		hr = m_DXDevice->CreateRenderTargetView(backBuffer, NULL, &m_DXRenderTargetView);
		if (FAILED(hr)) {
			OE_LOG_FATAL("Can't create the render target view!");
			return;
		}

		Graphics::TextureProperties properties;
		properties.textureBufferMode = Graphics::BufferMode::DEFAULT;
		properties.formatProperties.format = Graphics::TextureFormat::DEPTH;
		properties.formatProperties.width = p_Size.x;
		properties.formatProperties.height = p_Size.y;
		properties.dimension = Graphics::TEXTURE2D;
		m_DepthStencil = new Graphics::D3DDepthTexture(properties);

		OE_D3D_RELEASE(backBuffer);
	}
	*/

	/*
	void D3DContext::setDefaultBackbuffer()
	{
		if (!m_DXRenderTargetView || !m_DepthStencil) {
			resizeContext(p_Size);
		}

		m_DXDeviceContext->OMSetRenderTargets(1, &m_DXRenderTargetView, m_DepthStencil->getDSV());
		setViewport();
	}
	*/

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
		OE_D3D_RELEASE(m_DXDevice);
		OE_D3D_RELEASE(m_DXDeviceContext);
		if (p_DefaultFramebuffer) {
			delete p_DefaultFramebuffer;
			p_DefaultFramebuffer = 0;
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