#include "cef_osr_browser_win.h"
#include "cef_util_win.h"
#include <windowsx.h>

namespace cef_ui
{
	cef_osr_browser::cef_osr_browser(CefRefPtr<CefBrowser> browser):
		m_handle(0),
		m_width(0),
		m_height(0),
		m_enable(1),
		m_holdBrowser(browser)
	{

	}

	cef_osr_browser::~cef_osr_browser()
	{
		ReSet();
		if (m_holdBrowser.get())
		{
			m_holdBrowser = NULL;
		}
	}

	void cef_osr_browser::ReSet()
	{
		m_width = 1;
		m_height = 1;
		m_enable = 1;
		DettackHandle();
	}

	void cef_osr_browser::CloseBrowser(bool force_close /* = flase*/)
	{
		if (m_holdBrowser.get())
		{
			m_holdBrowser->GetHost()->CloseBrowser(force_close);
			m_holdBrowser = NULL;
		}
	}

	void cef_osr_browser::RequestUrl(std::string& url)
	{
		if (m_holdBrowser.get())
		{
			m_holdBrowser->GetMainFrame()->LoadURL(url);
		}
	}

	void cef_osr_browser::ReSize(int width, int height)
	{
		m_width = width; 
		m_height = height;
		if (m_holdBrowser.get())
		{
			m_holdBrowser->GetHost()->WasResized();
		}
	}

	int cef_osr_browser::GetIdentifier()
	{
		int id = 0;
		if (m_holdBrowser.get())
		{
			id = m_holdBrowser->GetIdentifier();
		}

		return id;
	}

	bool cef_osr_browser::IsSame(CefRefPtr<CefBrowser> that)
	{
		bool ret = false;
		if (m_holdBrowser.get())
		{
			ret = m_holdBrowser->IsSame(that);
		}

		return ret;
	}

	void cef_osr_browser::Paint()
	{
		if (m_holdBrowser.get())
		{
			//m_holdBrowser->GetHost()->Invalidate(PET_VIEW);
			HWND hWnd = m_holdBrowser->GetHost()->GetWindowHandle();
			::PostMessage(hWnd, WM_PAINT, 0, 0);
		}
	}

	void cef_osr_browser::OnPaint(const void* buffer, int biwidth, int biheight)
	{
		if (biwidth <= 0 || biheight <= 0)
		{
			return;
		}

	}

	void cef_osr_browser::SetFoucs(bool bFoucs /*= false*/)
	{
		m_isFoucs = bFoucs;
		if (m_holdBrowser.get())
		{
			m_holdBrowser->GetHost()->SendFocusEvent(m_isFoucs);
		}
	}

	void cef_osr_browser::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (m_handle && message == WM_PAINT)
		{
			OnWndPaint(message, wParam, lParam);
			return;
		}

		if (!m_isFoucs || !m_enable)
		{
			return;
		}
		if (m_handle)
		{
			switch (message)
			{
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
				case WM_MBUTTONDOWN:
				case WM_LBUTTONUP:
				case WM_RBUTTONUP:
				case WM_MBUTTONUP:
				case WM_MOUSEMOVE:
				case WM_MOUSELEAVE:
				case WM_MOUSEWHEEL:
				{
					OnMouseEvent(message, wParam, lParam);
				} break;
				case WM_SYSCHAR:
				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP:
				case WM_KEYDOWN:
				case WM_KEYUP:
				case WM_CHAR:
				{
					OnKeyEvent(message, wParam, lParam);
				}break;
			}
		}

	}

	void cef_osr_browser::OnWndPaint(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (m_holdBrowser.get())
		{
			m_holdBrowser->GetHost()->Invalidate(PET_VIEW);
		}
	}

	void cef_osr_browser::OnKeyEvent(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (!m_holdBrowser.get())
		{
			return;
		}

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
		m_holdBrowser->GetHost()->SendKeyEvent(event);
	}

	void cef_osr_browser::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam) 
	{
		if (!m_holdBrowser.get())
		{
			return;
		}

		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		CefRefPtr<CefBrowserHost> browser_host = m_holdBrowser->GetHost();
		HWND hwnd_ = browser_host->GetWindowHandle();
		LONG currentTime = 0;
		bool cancelPreviousClick = false;

		if (message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN ||
			message == WM_MBUTTONDOWN || message == WM_MOUSEMOVE ||
			message == WM_MOUSELEAVE) {
			currentTime = GetMessageTime();

			cancelPreviousClick =
				(abs(m_last_click_x - x) > (GetSystemMetrics(SM_CXDOUBLECLK) / 2)) ||
				(abs(m_last_click_y - y) > (GetSystemMetrics(SM_CYDOUBLECLK) / 2)) ||
				((currentTime - m_last_click_count) > GetDoubleClickTime());
			if (cancelPreviousClick &&
				(message == WM_MOUSEMOVE || message == WM_MOUSELEAVE)) {
				m_last_click_count = 0;
				m_last_click_x = 0;
				m_last_click_y = 0;
				m_last_click_time = 0;
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
				m_last_mouse_pos.x = m_current_mouse_pos.x = x;
				m_last_mouse_pos.y = m_current_mouse_pos.y = y;
				m_mouse_rotation = true;
			}
			else {
				CefBrowserHost::MouseButtonType btnType =
					(message == WM_LBUTTONDOWN
						? MBT_LEFT
						: (message == WM_RBUTTONDOWN ? MBT_RIGHT : MBT_MIDDLE));
				if (!cancelPreviousClick && (btnType == m_last_click_button)) {
					++m_last_click_count;
				}
				else {
					m_last_click_count = 1;
					m_last_click_x = x;
					m_last_click_y = y;
				}
				m_last_click_time = currentTime;
				m_last_click_button = btnType;

				if (browser_host) {
					CefMouseEvent mouse_event;
					mouse_event.x = x;
					mouse_event.y = y;
					//last_mouse_down_on_view_ = !IsOverPopupWidget(x, y);
					//ApplyPopupOffset(mouse_event.x, mouse_event.y);
					//DeviceToLogical(mouse_event, device_scale_factor_);
					mouse_event.modifiers = GetCefMouseModifiers(wParam);
					browser_host->SendMouseClickEvent(mouse_event, btnType, false,
						m_last_click_count);
				}
			}
		} break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			if (GetCapture() == hwnd_)
				ReleaseCapture();
			if (m_mouse_rotation) {
				// End rotation effect.
				m_mouse_rotation = false;
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
						m_last_click_count);
				}
			}
			break;

		case WM_MOUSEMOVE: {
			//int x = GET_X_LPARAM(lParam);
			//int y = GET_Y_LPARAM(lParam);
			if (m_mouse_rotation) {
				// Apply rotation effect.
				m_current_mouse_pos.x = x;
				m_current_mouse_pos.y = y;
				//renderer_.IncrementSpin(current_mouse_pos_.x - last_mouse_pos_.x,
				//	current_mouse_pos_.y - last_mouse_pos_.y);
				m_last_mouse_pos.x = m_current_mouse_pos.x;
				m_last_mouse_pos.y = m_current_mouse_pos.y;
				//Invalidate();
			}
			else {
				if (!m_mouse_tracking) {
					// Start tracking mouse leave. Required for the WM_MOUSELEAVE event to
					// be generated.
					TRACKMOUSEEVENT tme;
					tme.cbSize = sizeof(TRACKMOUSEEVENT);
					tme.dwFlags = TME_LEAVE;
					tme.hwndTrack = hwnd_;
					TrackMouseEvent(&tme);
					m_mouse_tracking = true;
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
			if (m_mouse_tracking) {
				// Stop tracking mouse leave.
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE & TME_CANCEL;
				tme.hwndTrack = hwnd_;
				TrackMouseEvent(&tme);
				m_mouse_tracking = false;
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

	// 输入字符
	void cef_osr_browser::OnKeyEvent(int key_down, int key_char)
	{
		OnKeyEvent(WM_KEYDOWN, key_down, 0x001E0001);
		OnKeyEvent(WM_CHAR, key_char, 0x001E0001);
	}

	// 左键按下
	void cef_osr_browser::OnLeftDown(int x, int y)
	{
		if (!m_holdBrowser.get())
		{
			return;
		}

		CefRefPtr<CefBrowserHost> browser_host = m_holdBrowser->GetHost();
		CefMouseEvent mouse_event;
		mouse_event.x = x;
		mouse_event.y = y;
		mouse_event.modifiers = EVENTFLAG_LEFT_MOUSE_BUTTON;
		browser_host->SendMouseClickEvent(mouse_event, MBT_LEFT, false, 1);
	}

	// 左键松开
	void cef_osr_browser::OnLeftUp(int x, int y)
	{
		if (!m_holdBrowser.get())
		{
			return;
		}

		CefRefPtr<CefBrowserHost> browser_host = m_holdBrowser->GetHost();
		CefMouseEvent mouse_event;
		mouse_event.x = x;
		mouse_event.y = y;
		mouse_event.modifiers = EVENTFLAG_LEFT_MOUSE_BUTTON;
		browser_host->SendMouseClickEvent(mouse_event, MBT_LEFT, true, 1);
	}

	// 鼠标移动
	void cef_osr_browser::OnMouseMove(int x, int y)
	{
		if (!m_holdBrowser.get())
		{
			return;
		}

		CefRefPtr<CefBrowserHost> browser_host = m_holdBrowser->GetHost();
		CefMouseEvent mouse_event;
		mouse_event.x = x;
		mouse_event.y = y;
		mouse_event.modifiers = GetCefMouseModifiers(0);
		browser_host->SendMouseMoveEvent(mouse_event, false);
	}

	// 鼠标移动
	void cef_osr_browser::OnMouseLeave(int x, int y)
	{
		if (!m_holdBrowser.get())
		{
			return;
		}

		CefRefPtr<CefBrowserHost> browser_host = m_holdBrowser->GetHost();
		CefMouseEvent mouse_event;
		mouse_event.x = x;
		mouse_event.y = y;
		mouse_event.modifiers = GetCefMouseModifiers(0);
		browser_host->SendMouseMoveEvent(mouse_event, true);
	}

	// 向上滚动
	void cef_osr_browser::OnWheelUp(int x, int y)
	{
		if (!m_holdBrowser.get())
		{
			return;
		}

		CefRefPtr<CefBrowserHost> browser_host = m_holdBrowser->GetHost();
		int delta = GET_WHEEL_DELTA_WPARAM(0x00780000);

		CefMouseEvent mouse_event;
		mouse_event.x = x;
		mouse_event.y = y;
		mouse_event.modifiers = GetCefMouseModifiers(0);
		browser_host->SendMouseWheelEvent(mouse_event,
			IsKeyDown(VK_SHIFT) ? delta : 0,
			!IsKeyDown(VK_SHIFT) ? delta : 0);
	}

	// 向下滚动
	void cef_osr_browser::OnWheelDown(int x, int y)
	{
		if (!m_holdBrowser.get())
		{
			return;
		}

		CefRefPtr<CefBrowserHost> browser_host = m_holdBrowser->GetHost();
		int delta = GET_WHEEL_DELTA_WPARAM(0xff880000);

		CefMouseEvent mouse_event;
		mouse_event.x = x;
		mouse_event.y = y;
		mouse_event.modifiers = GetCefMouseModifiers(0);
		browser_host->SendMouseWheelEvent(mouse_event,
			IsKeyDown(VK_SHIFT) ? delta : 0,
			!IsKeyDown(VK_SHIFT) ? delta : 0);
	}
};

