#ifndef APPLICATION_WINDOWIMPL_HPP
#define APPLICATION_WINDOWIMPL_HPP

#include "OE/Application/NativeHandlers.hpp"
#include "OE/Application/Window.hpp"
#include "OE/Application/View.hpp"
#include "OE/Application/InputManager.hpp"

namespace OrbitEngine {	namespace Application {
	namespace priv {
	class ContextImpl;

	class WindowImpl : public View {
	public:
		virtual ~WindowImpl();

		virtual void processEvents();

		virtual bool destroyRequested() const;

		Math::Vec2i getCursorPosition() const;
		virtual DisplayMode getDisplayMode() const;
		virtual std::string getTitle() const;
		virtual Math::Vec2i getPosition() const;
		virtual Math::Vec2i getSize() const;
		virtual bool isVisible() const;
		virtual bool isFocused() const;
		virtual bool isMinimized() const = 0;

		virtual bool setDisplayMode(DisplayMode mode) = 0;
		virtual bool setTitle(const std::string& title) = 0;
		virtual bool setPosition(const Math::Vec2i& position) = 0;
		virtual bool setSize(const Math::Vec2i& size) = 0;
		virtual bool setVisibility(bool visible) = 0;
		virtual bool requestFocus() = 0;
		virtual bool setAlpha(float alpha) = 0;

		virtual WindowNativeHandle getWindowNativeHandle() = 0;
		virtual DisplayNativeHandle getDisplayNativeHandle() = 0;

	protected:
		WindowImpl();
		
		void requestDestroy();

		friend class ContextImpl;
		friend class InputManager;

		DisplayMode p_DisplayMode;
		std::string p_Title;
		Math::Vec2i p_Position;
		Math::Vec2i p_Size;
		bool p_Focused;
		bool p_Visible;

		ContextImpl* p_Context = 0;
	private:
		bool m_DestroyRequested = false;
	};
} } }

#endif