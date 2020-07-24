#include "CefElementBuffer.hpp"

#include <OE/Graphics/API/Texture.hpp>

namespace OrbitEngine { namespace Editor {

	CefElementBuffer::CefElementBuffer()
		: m_Buffer(nullptr), m_Width(0), m_Height(0)
	{

	}

	CefElementBuffer::~CefElementBuffer()
	{
	}

	void CefElementBuffer::update(const void* buffer, int width, int height)
	{
		int stride = width * 4;
		int size = stride * height;

		std::lock_guard<std::mutex> guard(m_Lock);
		if (!m_Buffer || width != m_Width || height != m_Height) {
			m_Buffer = std::make_unique<uint8_t[]>(size);
			m_Width = width;
			m_Height = height;
		}
		memcpy(m_Buffer.get(), buffer, size);
	}

	void CefElementBuffer::upload(Graphics::Texture*& tex)
	{
		using namespace OrbitEngine::Graphics;

		std::lock_guard<std::mutex> guard(m_Lock);

		if (!m_Buffer)
			return;

		// delete
		if (tex && (tex->getProperties().width != m_Width || tex->getProperties().height != m_Height)) {
			delete tex;
			tex = nullptr;
		}

		// create
		if (!tex) {
			TextureProperties props;
			props.sampleProperties.filter = NEAREST;
			props.formatProperties.format = BGRA;
			props.width = m_Width;
			props.height = m_Height;
			props.textureBufferMode = DYNAMIC;
			tex = Texture::Create(props, nullptr);
		}

		tex->setData(m_Buffer.get());
	}

} }