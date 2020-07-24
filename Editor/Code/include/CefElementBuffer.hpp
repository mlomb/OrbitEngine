#ifndef CEF_ELEMENT_BUFFER_HPP
#define CEF_ELEMENT_BUFFER_HPP

#include <mutex>
#include <memory>

#include "Cef.hpp"

namespace OrbitEngine {
	namespace Graphics { class Texture; }

	namespace Editor {

	class CefElementBuffer {
	public:
		CefElementBuffer();
		~CefElementBuffer();

		void update(const void* buffer, int width, int height);
		void upload(Graphics::Texture*& tex);

	private:
		std::mutex m_Lock;
		std::unique_ptr<uint8_t[]> m_Buffer;
		int m_Width, m_Height;
	};
} }

#endif