
#include <string>
#include "cef_ui_app.h"
#include "cef_ui_handler.h"
#include "cef_headers/include/wrapper/cef_helpers.h"
#include "cef_headers/include/base/cef_logging.h"
#include "cef_headers/include/views/cef_browser_view.h"
#include "cef_headers/include/views/cef_window.h"
#include "cef_headers/include/base/cef_bind.h"
#include "cef_headers/include/wrapper/cef_closure_task.h"

namespace cef_ui
{
	CefRefPtr<cef_ui_handler> g_instance = NULL;

	cef_ui_handler::cef_ui_handler(CefRefPtr<cef_ui_app> app)
		: m_app(app), is_closing_(false)
	{
		DCHECK(!g_instance.get());
		g_instance = this;
	}

	cef_ui_handler::~cef_ui_handler()
	{
		//g_instance = NULL;
	}

	// static
	cef_ui_handler* cef_ui_handler::GetInstance() {
		return g_instance;
	}

	void cef_ui_handler::OnTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title) {
		CEF_REQUIRE_UI_THREAD();

		//if (use_views_) {
		//	// Set the title of the window using the Views framework.
		//	CefRefPtr<CefBrowserView> browser_view =
		//		CefBrowserView::GetForBrowser(browser);
		//	if (browser_view) {
		//		CefRefPtr<CefWindow> window = browser_view->GetWindow();
		//		if (window)
		//			window->SetTitle(title);
		//	}
		//}
		//else {
		//	// Set the title of the window using platform APIs.
		//	PlatformTitleChange(browser, title);
		//}

		// Set the title of the window using the Views framework.
		CefRefPtr<CefBrowserView> browser_view =
			CefBrowserView::GetForBrowser(browser);
		if (browser_view) {
			CefRefPtr<CefWindow> window = browser_view->GetWindow();
			if (window)
				window->SetTitle(title);
		}
	}

	void cef_ui_handler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();

		// Add to the list of existing browsers.
		browser_list_.push_back(browser);
	}

	bool cef_ui_handler::DoClose(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();

		// Closing the main window requires special handling. See the DoClose()
		// documentation in the CEF header for a detailed destription of this
		// process.
		if (browser_list_.size() == 1) {
			// Set a flag to indicate that the window close should be allowed.
			is_closing_ = true;
		}

		// Allow the close. For windowed browsers this will result in the OS close
		// event being sent.
		return false;
	}

	void cef_ui_handler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();

		// Remove from the list of existing browsers.
		BrowserList::iterator bit = browser_list_.begin();
		for (; bit != browser_list_.end(); ++bit) {
			if ((*bit)->IsSame(browser)) {
				browser_list_.erase(bit);
				break;
			}
		}

		if (browser_list_.empty()) {
			// All browser windows have closed. Quit the application message loop.
			CefQuitMessageLoop();
		}
	}

	void cef_ui_handler::OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) {
		CEF_REQUIRE_UI_THREAD();

		// Don't display an error for downloaded files.
		if (errorCode == ERR_ABORTED)
			return;

		// Display a load error message.
		std::stringstream ss;
		ss << "<html><body bgcolor=\"white\">"
			"<h2>Failed to load URL "
			<< std::string(failedUrl) << " with error " << std::string(errorText)
			<< " (" << errorCode << ").</h2></body></html>";
		frame->LoadString(ss.str(), failedUrl);
	}

	bool cef_ui_handler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		WindowOpenDisposition target_disposition,
		bool user_gesture,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		bool* no_javascript_access)
	{
		switch (target_disposition)
		{
		case WOD_NEW_FOREGROUND_TAB:
		case WOD_NEW_BACKGROUND_TAB:
		case WOD_NEW_POPUP:
		case WOD_NEW_WINDOW:
			browser->GetMainFrame()->LoadURL(target_url);
			return true; //cancel create
		}

		return false;
	}

	void cef_ui_handler::CloseAllBrowsers(bool force_close) {
		if (!CefCurrentlyOn(TID_UI)) {
			// Execute on the UI thread.
			CefPostTask(TID_UI, base::Bind(&cef_ui_handler::CloseAllBrowsers, this,
				force_close));
			return;
		}

		if (browser_list_.empty())
			return;

		BrowserList::const_iterator it = browser_list_.begin();
		for (; it != browser_list_.end(); ++it)
			(*it)->GetHost()->CloseBrowser(force_close);
	}


	void cef_ui_handler::PlatformTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title) {
		//CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
		//SetWindowText(hwnd, std::string(title).c_str());
	}

	bool cef_ui_handler::GetViewRect(CefRefPtr<CefBrowser> /*browser*/, CefRect& rect)
	{
		// if (this->canvas)
		// {
		// 	rect.Set(0, 0, this->canvas->get_width(), this->canvas->get_height());
		// }
		// else
		// {
		// 	rect.Set(0, 0, 640, 480);
		// }

		rect.Set(10, 10, 300, 400);
		return true;
	}

	void cef_ui_handler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType /*type*/, const RectList& /*dirtyRects*/, const void* buffer, int width, int height)
	{
		//Components::XUI::UpdateTexture(buffer, width, height);
		// if (!this->canvas) return;
		// if (this->canvas->get_width() == uint32_t(width) && this->canvas->get_height() == uint32_t(height))
		// {
		// 	this->canvas->paint(buffer);
		// }
		// else
		// {
		// 	browser->GetHost()->WasResized();
		// }

		if (this->m_fun)
		{
			this->m_fun(buffer, width, height);
		}
	}

	void cef_ui_handler::trigger_resize()
	{
		for (auto& browser : this->browser_list_)
		{
			//browser->GetHost()->WasResized();

			browser->GetHost()->Invalidate(PET_VIEW);
			//browser->GetHost()->SendKeyEvent();
		}
	}

	void cef_ui_handler::SetFun(std::function<void(const void*, size_t, size_t)> fun)
	{
		this->m_fun = fun;
	}
}
