#ifndef _CEF_UI_HANDLER_H_
#define _CEF_UI_HANDLER_H_

#include <list>
#include "cef_client.h"


namespace cef_ui
{
	class cef_ui_app;

	class cef_ui_handler : public CefClient, public CefDisplayHandler, public CefLifeSpanHandler, public CefLoadHandler
	{
	public:
        explicit cef_ui_handler(bool use_views);
		~cef_ui_handler();

		// Provide access to the single global instance of this object.
		static cef_ui_handler* GetInstance();

		// CefClient methods:
		virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
			return this;
		}
		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
			return this;
		}
		virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }

		// CefDisplayHandler methods:
		virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
			const CefString& title) OVERRIDE;

		// CefLifeSpanHandler methods:
		virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
		virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

		// CefLoadHandler methods:
		virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			ErrorCode errorCode,
			const CefString& errorText,
			const CefString& failedUrl) OVERRIDE;

		// Request that all existing browser windows close.
		void CloseAllBrowsers(bool force_close);

		bool IsClosing() const { return is_closing_; }

	private:
		// Platform-specific implementation.
		void PlatformTitleChange(CefRefPtr<CefBrowser> browser,
			const CefString& title);

		// True if the application is using the Views framework.
		const bool use_views_;

		// List of existing browser windows. Only accessed on the CEF UI thread.
		typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
		BrowserList browser_list_;

		bool is_closing_;
		IMPLEMENT_REFCOUNTING(cef_ui_handler);
	};
}


#endif //_CEF_UI_HANDLER_H_