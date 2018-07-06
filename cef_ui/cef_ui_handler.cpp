
#include "cef_headers/include/wrapper/cef_helpers.h"
#include "cef_headers/include/base/cef_logging.h"
#include "cef_headers/include/views/cef_browser_view.h"
#include "cef_headers/include/views/cef_window.h"
#include "cef_headers/include/base/cef_bind.h"
#include "cef_headers/include/wrapper/cef_closure_task.h"
#include <string>
#include <windowsx.h>
#include "cef_ui_app.h"
#include "cef_ui_handler.h"
#include "cef_util_win.h"

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

	void cef_ui_handler::OnPaint()
	{
		for (auto& browser : this->browser_list_)
		{
			//browser->GetHost()->WasResized();

			browser->GetHost()->Invalidate(PET_VIEW);
		}
	}

	void cef_ui_handler::SetFun(std::function<void(const void*, size_t, size_t)> fun)
	{
		this->m_fun = fun;
	}

	void cef_ui_handler::OnKeyEvent(UINT message, WPARAM wParam, LPARAM lParam)
	{
		for (auto& browser : this->browser_list_)
		{
			CefKeyEvent event;
			event.windows_key_code = wParam;
			event.native_key_code = lParam;
			event.is_system_key = message == WM_SYSCHAR || message == WM_SYSKEYDOWN ||
				message == WM_SYSKEYUP;

			if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN)
				event.type = KEYEVENT_RAWKEYDOWN;
			else if (message == WM_KEYUP || message == WM_SYSKEYUP)
				event.type = KEYEVENT_KEYUP;
			else
				event.type = KEYEVENT_CHAR;
			event.modifiers = GetCefKeyboardModifiers(wParam, lParam);
			browser->GetHost()->SendKeyEvent(event);
		}
	}

	void cef_ui_handler::OnFocus(bool setFocus) {
		for (auto& browser : this->browser_list_)
		{
			browser->GetHost()->SendFocusEvent(setFocus);
		}
	}

	void cef_ui_handler::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam) {
		for (auto& browser : this->browser_list_)
		{
			CefRefPtr<CefBrowserHost> browser_host;
			browser_host = browser->GetHost();
			HWND hwnd_ = browser_host->GetWindowHandle();
			LONG currentTime = 0;
			bool cancelPreviousClick = false;
			int x = GET_X_LPARAM(lParam) - 100;
			int y = GET_Y_LPARAM(lParam) - 100;
			if (message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN ||
				message == WM_MBUTTONDOWN || message == WM_MOUSEMOVE ||
				message == WM_MOUSELEAVE) {
				currentTime = GetMessageTime();

				cancelPreviousClick =
					(abs(last_click_x_ - x) > (GetSystemMetrics(SM_CXDOUBLECLK) / 2)) ||
					(abs(last_click_y_ - y) > (GetSystemMetrics(SM_CYDOUBLECLK) / 2)) ||
					((currentTime - last_click_time_) > GetDoubleClickTime());
				if (cancelPreviousClick &&
					(message == WM_MOUSEMOVE || message == WM_MOUSELEAVE)) {
					last_click_count_ = 0;
					last_click_x_ = 0;
					last_click_y_ = 0;
					last_click_time_ = 0;
				}
			}

			switch (message) {
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN: {
				::SetCapture(hwnd_);
				::SetFocus(hwnd_);
				//int x = GET_X_LPARAM(lParam);
				//int y = GET_Y_LPARAM(lParam);
				if (wParam & MK_SHIFT) {
					// Start rotation effect.
					last_mouse_pos_.x = current_mouse_pos_.x = x;
					last_mouse_pos_.y = current_mouse_pos_.y = y;
					mouse_rotation_ = true;
				}
				else {
					CefBrowserHost::MouseButtonType btnType =
						(message == WM_LBUTTONDOWN
							? MBT_LEFT
							: (message == WM_RBUTTONDOWN ? MBT_RIGHT : MBT_MIDDLE));
					if (!cancelPreviousClick && (btnType == last_click_button_)) {
						++last_click_count_;
					}
					else {
						last_click_count_ = 1;
						last_click_x_ = x;
						last_click_y_ = y;
					}
					last_click_time_ = currentTime;
					last_click_button_ = btnType;

					if (browser_host) {
						CefMouseEvent mouse_event;
						mouse_event.x = x;
						mouse_event.y = y;
						//last_mouse_down_on_view_ = !IsOverPopupWidget(x, y);
						//ApplyPopupOffset(mouse_event.x, mouse_event.y);
						//DeviceToLogical(mouse_event, device_scale_factor_);
						mouse_event.modifiers = GetCefMouseModifiers(wParam);
						browser_host->SendMouseClickEvent(mouse_event, btnType, false,
							last_click_count_);
					}
				}
			} break;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
				if (GetCapture() == hwnd_)
					ReleaseCapture();
				if (mouse_rotation_) {
					// End rotation effect.
					mouse_rotation_ = false;
					//renderer_.SetSpin(0, 0);
					//Invalidate();
				}
				else {
					//int x = GET_X_LPARAM(lParam);
					//int y = GET_Y_LPARAM(lParam);
					CefBrowserHost::MouseButtonType btnType =
						(message == WM_LBUTTONUP
							? MBT_LEFT
							: (message == WM_RBUTTONUP ? MBT_RIGHT : MBT_MIDDLE));
					if (browser_host) {
						CefMouseEvent mouse_event;
						mouse_event.x = x;
						mouse_event.y = y;
						//if (last_mouse_down_on_view_ && IsOverPopupWidget(x, y) &&
						//	(GetPopupXOffset() || GetPopupYOffset())) {
						//	break;
						//}
						//ApplyPopupOffset(mouse_event.x, mouse_event.y);
						//DeviceToLogical(mouse_event, device_scale_factor_);
						mouse_event.modifiers = GetCefMouseModifiers(wParam);
						browser_host->SendMouseClickEvent(mouse_event, btnType, true,
							last_click_count_);
					}
				}
				break;

			case WM_MOUSEMOVE: {
				//int x = GET_X_LPARAM(lParam);
				//int y = GET_Y_LPARAM(lParam);
				if (mouse_rotation_) {
					// Apply rotation effect.
					current_mouse_pos_.x = x;
					current_mouse_pos_.y = y;
					//renderer_.IncrementSpin(current_mouse_pos_.x - last_mouse_pos_.x,
					//	current_mouse_pos_.y - last_mouse_pos_.y);
					last_mouse_pos_.x = current_mouse_pos_.x;
					last_mouse_pos_.y = current_mouse_pos_.y;
					//Invalidate();
				}
				else {
					if (!mouse_tracking_) {
						// Start tracking mouse leave. Required for the WM_MOUSELEAVE event to
						// be generated.
						TRACKMOUSEEVENT tme;
						tme.cbSize = sizeof(TRACKMOUSEEVENT);
						tme.dwFlags = TME_LEAVE;
						tme.hwndTrack = hwnd_;
						TrackMouseEvent(&tme);
						mouse_tracking_ = true;
					}

					if (browser_host) {
						CefMouseEvent mouse_event;
						mouse_event.x = x;
						mouse_event.y = y;
						//ApplyPopupOffset(mouse_event.x, mouse_event.y);
						//DeviceToLogical(mouse_event, device_scale_factor_);
						mouse_event.modifiers = GetCefMouseModifiers(wParam);
						browser_host->SendMouseMoveEvent(mouse_event, false);
					}
				}
				break;
			}

			case WM_MOUSELEAVE: {
				if (mouse_tracking_) {
					// Stop tracking mouse leave.
					TRACKMOUSEEVENT tme;
					tme.cbSize = sizeof(TRACKMOUSEEVENT);
					tme.dwFlags = TME_LEAVE & TME_CANCEL;
					tme.hwndTrack = hwnd_;
					TrackMouseEvent(&tme);
					mouse_tracking_ = false;
				}

				if (browser_host) {
					// Determine the cursor position in screen coordinates.
					POINT p;
					::GetCursorPos(&p);
					::ScreenToClient(hwnd_, &p);

					CefMouseEvent mouse_event;
					mouse_event.x = p.x;
					mouse_event.y = p.y;
					//DeviceToLogical(mouse_event, device_scale_factor_);
					mouse_event.modifiers = GetCefMouseModifiers(wParam);
					browser_host->SendMouseMoveEvent(mouse_event, true);
				}
			} break;

			case WM_MOUSEWHEEL:
				//int x = GET_X_LPARAM(lParam);
				//int y = GET_Y_LPARAM(lParam);
				if (browser_host) {
					POINT screen_point = { x, y };
					HWND scrolled_wnd = ::WindowFromPoint(screen_point);
					if (scrolled_wnd != hwnd_)
						break;

					ScreenToClient(hwnd_, &screen_point);
					int delta = GET_WHEEL_DELTA_WPARAM(wParam);

					CefMouseEvent mouse_event;
					mouse_event.x = screen_point.x;
					mouse_event.y = screen_point.y;
					//ApplyPopupOffset(mouse_event.x, mouse_event.y);
					//DeviceToLogical(mouse_event, device_scale_factor_);
					mouse_event.modifiers = GetCefMouseModifiers(wParam);
					browser_host->SendMouseWheelEvent(mouse_event,
						IsKeyDown(VK_SHIFT) ? delta : 0,
						!IsKeyDown(VK_SHIFT) ? delta : 0);
				}
				break;
			}
		}
	}

	//bool cef_ui_handler::IsOverPopupWidget(int x, int y) const {
	//	CEF_REQUIRE_UI_THREAD();
	//	const CefRect& rc = renderer_.popup_rect();
	//	int popup_right = rc.x + rc.width;
	//	int popup_bottom = rc.y + rc.height;
	//	return (x >= rc.x) && (x < popup_right) && (y >= rc.y) && (y < popup_bottom);
	//}

	//int cef_ui_handler::GetPopupXOffset() const {
	//	CEF_REQUIRE_UI_THREAD();
	//	return renderer_.original_popup_rect().x - renderer_.popup_rect().x;
	//}

	//int cef_ui_handler::GetPopupYOffset() const {
	//	CEF_REQUIRE_UI_THREAD();
	//	return renderer_.original_popup_rect().y - renderer_.popup_rect().y;
	//}

	//void cef_ui_handler::ApplyPopupOffset(int& x, int& y) const {
	//	if (IsOverPopupWidget(x, y)) {
	//		x += GetPopupXOffset();
	//		y += GetPopupYOffset();
	//	}
	//}
}
