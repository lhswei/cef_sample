
#include "cef_ui_app.h"
#include "cef_ui_handler.h"
#include "cef_headers/include/cef_browser.h"
#include "cef_headers/include/cef_command_line.h"
#include "cef_headers/include/views/cef_browser_view.h"
#include "cef_headers/include/views/cef_window.h"
#include "cef_headers/include/wrapper/cef_helpers.h"

namespace cef_ui
{
	// When using the Views framework this object provides the delegate
	// implementation for the CefWindow that hosts the Views-based browser.
	class SimpleWindowDelegate : public CefWindowDelegate
	{
	public:
		explicit SimpleWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
			: browser_view_(browser_view) {}

		void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE {
			// Add the browser view and show the window.
			window->AddChildView(browser_view_);
			window->Show();

			// Give keyboard focus to the browser view.
			browser_view_->RequestFocus();
		}

		void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE {
			browser_view_ = NULL;
		}

		bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE {
			// Allow the window to close if the browser says it's OK.
			CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
			if (browser)
				return browser->GetHost()->TryCloseBrowser();
			return true;
		}

	private:
		CefRefPtr<CefBrowserView> browser_view_;

		IMPLEMENT_REFCOUNTING(SimpleWindowDelegate);
		DISALLOW_COPY_AND_ASSIGN(SimpleWindowDelegate);
	};

	cef_ui_app::cef_ui_app()
	{
	
	}

	void cef_ui_app::OnContextInitialized()
	{
		CEF_REQUIRE_UI_THREAD();

		//CefRefPtr<CefCommandLine> command_line =
		//	CefCommandLine::GetGlobalCommandLine();

	}

	void cef_ui_app::OnBeforeCommandLineProcessing(const CefString& /*process_type*/, CefRefPtr<CefCommandLine> command_line)
	{
	//	//command_line->AppendSwitch("enable-experimental-web-platform-features");
	//	//command_line->AppendSwitch("in-process-gpu");
	//	//command_line->AppendSwitch("enable-media-stream");
	//	//command_line->AppendSwitch("use-fake-ui-for-media-stream");
	//	//command_line->AppendSwitch("enable-speech-input");
	//	//command_line->AppendSwitch("ignore-gpu-blacklist");
	//	//command_line->AppendSwitch("enable-usermedia-screen-capture");
		command_line->AppendSwitchWithValue("default-encoding", "utf-8");
		command_line->AppendSwitch("enable-rgba-4444-textures");

	//	/*if (IsWindows10OrGreater())
	//	{
	//		command_line->AppendSwitch("force-gpu-rasterization");
	//	}*/

	//	//command_line->AppendSwitch("disable-gpu");
	//	//command_line->AppendSwitch("disable-gpu-compositing");
	//	//command_line->AppendSwitch("enable-begin-frame-scheduling");
		//command_line->AppendSwitch("enable-surface-synchronization");
	}

}
