#include "include/wrapper/cef_helpers.h"
#include "include/base/cef_logging.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"
#include <string>
#include <windowsx.h>
#include "cef_ui_app.h"
#include "cef_util_win.h"
#include "cef_ui_client.h"

namespace cef_ui
{
	CefRefPtr<cef_ui_client> g_instance = NULL;

	cef_ui_client::cef_ui_client(HWND hWnd):
		m_hWnd(hWnd),
		m_applyCount(0)
	{
		DCHECK(!g_instance.get());
		g_instance = this;
	}

	cef_ui_client::~cef_ui_client()
	{
		g_instance = NULL;
	}

	cef_ui_client* cef_ui_client::GetInstance()
	{
		return g_instance.get();
	}

	bool cef_ui_client::CreateBrowser(std::string url) {
		if (m_hWnd == 0)
			return false;
		CefWindowInfo window_info;
		CefBrowserSettings browser_settings;
		window_info.SetAsWindowless(m_hWnd);
		//RECT wndRect;
		//wndRect.left = 0;//SCREEN_WIDTH / 4;
		//wndRect.top = 0;// SCREEN_HEIGHT / 4;
		//wndRect.right = 400;// (SCREEN_WIDTH * 3) / 4;
		//wndRect.bottom = 400;// (SCREEN_HEIGHT * 3) / 4;
		//window_info.SetAsChild(m_hWnd, wndRect);
		CefBrowserHost::CreateBrowser(window_info, this, url, browser_settings, NULL);
		m_applyCount++;
		return true;
	}
	// 通过 Identifier 查找正在使用的 browserWin
	CefRefPtr<cef_osr_browser> cef_ui_client::FindBrowserWin(int id)
	{
		CefRefPtr<cef_osr_browser> ret = NULL;

		auto it = m_browserWinMap.find(id);
		if (it != m_browserWinMap.end())
		{
			ret = it->second;
		}
		return ret;
	}

	// 获取一个可用的(空闲中的) browserWin
	// 如有可用的，则返回一个，没有则创建一个空页面，并返回空。
	// 可能一次申请没有可用的，可以下一帧再申请
	CefRefPtr<cef_osr_browser> cef_ui_client::GetAvailableBrowserWin()
	{
		CefRefPtr<cef_osr_browser> ret = NULL;
		
		// 不管是申请到了，还是没有申请到，如果没有空闲的可用了就申请一个
		// m_applyCount 的最大值不超过MAX_APPLY_COUNT，防止短时时间内大量的申请
		if (m_applyCount < MAX_APPLY_COUNT && m_qeIdle.empty())
			CreateBrowser("");

		if (!m_qeIdle.empty())
		{
			// 取出第一个可用的
			ret = m_qeIdle.front();
			m_qeIdle.pop();

			if (ret.get() && ret->GetIdentifier() != 0)
			{
				// 放到正在使用链表
				m_browserWinMap.insert(MAP_BROWSER::value_type(ret->GetIdentifier(), ret));
			}
		}


		return ret;
	}

	// 将正在使用的页面放回空闲队列
	void cef_ui_client::SetBrowserIdle(int id)
	{
		auto it = m_browserWinMap.find(id);
		if (it != m_browserWinMap.end() && it->second.get())
		{
			auto browserWin = it->second;
			m_browserWinMap.erase(it);
			browserWin->ReSet();
			m_qeIdle.push(browserWin);
		}
	}

	void cef_ui_client::OnTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title) {
		CEF_REQUIRE_UI_THREAD();

	}

	void cef_ui_client::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();

		// Add to the qeuqu.
		CefRefPtr<cef_osr_browser> browserWin = new cef_osr_browser(browser);
		m_qeIdle.push(browserWin);
		m_applyCount--;
	}

	bool cef_ui_client::DoClose(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();

		// Allow the close. For windowed browsers this will result in the OS close
		// event being sent.
		return false;
	}

	void cef_ui_client::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();

		// Remove from the map of existing browsers.
		auto bit = m_browserWinMap.begin();
		for (; bit != m_browserWinMap.end(); ++bit) {
			if (bit->second.get() && bit->second->IsSame(browser)) {
				m_browserWinMap.erase(bit);
				break;
			}
		}
	}

	void cef_ui_client::OnLoadError(CefRefPtr<CefBrowser> browser,
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

	bool cef_ui_client::OnBeforePopup(CefRefPtr<CefBrowser> browser,
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


	void cef_ui_client::CloseAllBrowsers(bool force_close) {
		if (!CefCurrentlyOn(TID_UI)) {
			// Execute on the UI thread.
			CefPostTask(TID_UI, base::Bind(&cef_ui_client::CloseAllBrowsers, this,
				force_close));
			return;
		}

		if (m_browserWinMap.empty() && m_qeIdle.empty())
			return;

		auto it = m_browserWinMap.begin();
		for (; it != m_browserWinMap.end(); ++it)
		{
			if (it->second.get())
				it->second->CloseBrowser(force_close);
		}

		m_browserWinMap.clear();

		while (!m_qeIdle.empty())
		{
			auto it = m_qeIdle.front();
			if (it.get())
			{
				it->CloseBrowser(force_close);
			}
			m_qeIdle.pop();
		}
	}

	void cef_ui_client::PlatformTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title) {

	}

	bool cef_ui_client::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
	{
		int width = 0;
		int height = 0;
		int pos_X = 0;
		int pos_y = 0;
		for (auto& it : m_browserWinMap)
		{
			if (it.second.get() && it.second->IsSame(browser))
			{
				width = it.second->GetW();
				height = it.second->GetH();
				break;
			}
		}
		width = std::max(width, 1);
		height = std::max(height, 1);
		rect.Set(pos_X, pos_y, width, height);
		return true;
	}

	void cef_ui_client::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType /*type*/, const RectList& /*dirtyRects*/, const void* buffer, int width, int height)
	{
		CEF_REQUIRE_UI_THREAD()
		for (auto& it : m_browserWinMap)
		{
			if (it.second.get() && it.second->IsSame(browser))
			{
				it.second->OnPaint(buffer, width, height);
				break;
			}
		}
	}
};


