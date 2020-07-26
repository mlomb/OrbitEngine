#ifndef CEF_CONTROLLER_HPP
#define CEF_CONTROLLER_HPP

#include "Cef.hpp"

namespace OrbitEngine { namespace Editor {

	/**
		Handles the **engine** object in JS, relays messages between the render and browser processes

		@code
		window.engine.register(function callback(data) {
			// engine -> browser
		});

		// browser -> engine
		window.engine.send("data");
		@endcode
	**/
	class CefController :
		public CefApp,
		public CefRenderProcessHandler,
		public CefV8Handler
	{
	public:
		void OnContextCreated(CefRefPtr<CefBrowser> browser,
							  CefRefPtr<CefFrame> frame,
							  CefRefPtr<CefV8Context> context) override;
		void OnContextReleased(CefRefPtr<CefBrowser> browser,
							   CefRefPtr<CefFrame> frame,
							   CefRefPtr<CefV8Context> context) override;

		bool Execute(const CefString& name,
					 CefRefPtr<CefV8Value> object,
					 const CefV8ValueList& arguments,
					 CefRefPtr<CefV8Value>& retval,
					 CefString& exception) override;

		bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
									  CefRefPtr<CefFrame> frame,
									  CefProcessId source_process,
									  CefRefPtr<CefProcessMessage> message) override;

		CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

	private:
		CefRefPtr<CefV8Value> m_CallbackFunc;
		CefRefPtr<CefV8Context> m_CallbackCtx;

		IMPLEMENT_REFCOUNTING(CefController);
	};

} }

#endif