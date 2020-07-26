#include "CefController.hpp"

#include <OE/Misc/Log.hpp>

namespace OrbitEngine { namespace Editor {

	void CefController::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
	{
		CefRefPtr<CefV8Value> window = context->GetGlobal();

		CefRefPtr<CefV8Value> engine_obj = CefV8Value::CreateObject(NULL, NULL);
		engine_obj->SetValue("send", CefV8Value::CreateFunction("send", this), V8_PROPERTY_ATTRIBUTE_READONLY);
		engine_obj->SetValue("register", CefV8Value::CreateFunction("register", this), V8_PROPERTY_ATTRIBUTE_READONLY);

		window->SetValue("engine", engine_obj, V8_PROPERTY_ATTRIBUTE_READONLY);
	}

	void CefController::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
	{
		m_CallbackFunc = nullptr;
		m_CallbackCtx = nullptr;
	}

	bool CefController::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
	{
		if (name == "send") {
			if (arguments.size() == 1 && arguments[0]->IsString()) {
				CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
				CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("relay");
				message->GetArgumentList()->SetString(0, arguments[0]->GetStringValue());
				browser->GetMainFrame()->SendProcessMessage(PID_BROWSER, message);
			}
		} else if (name == "register") {
			if (arguments.size() == 1 && arguments[0]->IsFunction()) {
				m_CallbackFunc = arguments[0];
				m_CallbackCtx = CefV8Context::GetCurrentContext();
				return true;
			}
		}
		return false;
	}

	bool CefController::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
	{
		if (message->GetName() == "relay") {
			if (m_CallbackFunc.get() && m_CallbackCtx.get()) {
				CefV8ValueList args;
				args.push_back(CefV8Value::CreateString(message->GetArgumentList()->GetString(0)));
				m_CallbackFunc->ExecuteFunctionWithContext(m_CallbackCtx, NULL, args);
				return true;
			}
		}
		return false;
	}

    CefRefPtr<CefRenderProcessHandler> CefController::GetRenderProcessHandler() {
        return this;
    }

} }