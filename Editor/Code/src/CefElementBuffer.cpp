#include "CefElementBuffer.hpp"

#include <OE/Graphics/API/Texture.hpp>

namespace OrbitEngine { namespace Editor {

	CefElementBuffer::CefElementBuffer()
		: m_Buffer(nullptr), m_Width(0), m_Height(0), m_BlitTexture(nullptr)
	{
	}

	CefElementBuffer::~CefElementBuffer()
	{
		if (m_BlitTexture)
			delete m_BlitTexture;
	}

	void CefElementBuffer::update(const void* buffer, int width, int height)
	{
		int stride = width * 4;
		int size = stride * height;

		std::lock_guard<std::mutex> guard(m_Lock);
		if (!m_Buffer || width != m_Width || height != m_Height) {
			// TODO: reuse big buffer
			m_Buffer = std::make_unique<uint8_t[]>(size);
			m_Width = width;
			m_Height = height;
		}
		memcpy(m_Buffer.get(), buffer, size);
		m_Dirty = true;
	}

	Graphics::Texture* CefElementBuffer::getTexture()
	{
		using namespace OrbitEngine::Graphics;

		std::lock_guard<std::mutex> guard(m_Lock);

		if (!m_Buffer)
			return nullptr;
		if (!m_Dirty && m_BlitTexture)
			return m_BlitTexture;
		m_Dirty = false;

		// delete
		if (m_BlitTexture && (m_BlitTexture->getProperties().width != m_Width || m_BlitTexture->getProperties().height != m_Height)) {
			delete m_BlitTexture;
			m_BlitTexture = nullptr;
		}

		// create
		if (!m_BlitTexture) {
			TextureProperties props;
			props.sampleProperties.filter = NEAREST;
			props.formatProperties.format = BGRA;
			props.width = m_Width;
			props.height = m_Height;
			props.textureBufferMode = DYNAMIC;
			m_BlitTexture = Texture::Create(props, nullptr);
		}

		m_BlitTexture->setData(m_Buffer.get());

		return m_BlitTexture;
	}

} }