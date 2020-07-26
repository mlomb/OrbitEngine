#ifndef CEF_ELEMENT_BUFFER_HPP
#define CEF_ELEMENT_BUFFER_HPP

#include <mutex>
#include <memory>

#include "Cef.hpp"

namespace OrbitEngine {
	namespace Graphics { class Texture; }

	namespace Editor {

	/// The buffer must be created and destroyed in the browser thread
	class CefElementBuffer {
	public:
		CefElementBuffer();
		~CefElementBuffer();

		/// Must be called from the render thread
		void update(const void* buffer, int width, int height);
		/// Must be called from the browser thread, with a proper context
		Graphics::Texture* getTexture();

	private:
		std::mutex m_Lock;
		std::unique_ptr<uint8_t[]> m_Buffer;
		int m_Width, m_Height;
		bool m_Dirty;
		Graphics::Texture* m_BlitTexture;
	};
} }

#endif