#ifndef _CEF_UI_APP_H_
#define _CEF_UI_APP_H_

#include "cef_app.h"

namespace cef_ui
{

	class cef_ui_app : public CefApp, public CefBrowserProcessHandler
	{
	public:

		cef_ui_app();

		// CefApp methods:
		virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override
		{
			return this;
		}

		// CefBrowserProcessHandler methods:
		virtual void OnContextInitialized() override;

	protected:
		// CefApp methods:
		virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) override;


	private:

		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(cef_ui_app);
	};
};

#endif   //_CEF_UI_APP_H_