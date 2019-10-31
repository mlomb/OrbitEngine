#ifndef APPLICATION_WINDOW_EMSCRIPTEN_HPP
#define APPLICATION_WINDOW_EMSCRIPTEN_HPP

#include "OE/Platform/Emscripten/Emscripten.hpp"

#include "OE/Application/Window.hpp"
#include "OE/Application/WindowImpl.hpp"
#include "OE/Application/NativeHandlers.hpp"

#include "OE/Application/Context.hpp"

namespace OrbitEngine {	namespace Application { namespace priv {
	class WindowEmscripten : public WindowImpl {
	public:
		WindowEmscripten(const std::string& selector = "canvas");
		~WindowEmscripten();

		void processEvents() override;

		bool isMinimized() const override;
		bool setDisplayMode(DisplayMode mode) override;
		bool setCursor(Cursor cursor) override;
		bool setTitle(const std::string& title) override;
		bool setPosition(const Math::Vec2i& position) override;
		bool setSize(const Math::Vec2i& size) override;
		bool setVisibility(bool visible) override;
		bool requestFocus() override;
		bool setAlpha(float alpha) override;

		WindowNativeHandle getWindowNativeHandle() override;
		DisplayNativeHandle getDisplayNativeHandle() override;
		
		std::string getSelector() const;
		
		static std::string CursorToCSS(const Cursor cursor);
	private:
		std::string m_Selector;
		bool m_PointerLock;

		bool isPointerLockActive();
		void updateSizeFromHTML();

		static EM_BOOL EMS_Callback_FullscreenChange(int eventType, const EmscriptenFullscreenChangeEvent *fullscreenChangeEvent, void *userData);
		static EM_BOOL EMS_Callback_ResizeCanvas(int eventType, const EmscriptenUiEvent *uiEvent, void *userData);
		static EM_BOOL EMS_Callback_Focus(int eventType, const EmscriptenFocusEvent *focusEvent, void *userData);

		static EM_BOOL EMS_Callback_Key(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData);
		static EM_BOOL EMS_Callback_KeyPress(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData);
		static EM_BOOL EMS_Callback_Wheel(int eventType, const EmscriptenWheelEvent *wheelEvent, void *userData);
		static EM_BOOL EMS_Callback_MouseMove(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);
		static EM_BOOL EMS_Callback_MouseButton(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);
	};
} } }

#endif