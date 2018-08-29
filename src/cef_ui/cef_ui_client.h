#ifndef _CEF_UI_CLIENT_H_
#define _CEF_UI_CLIENT_H_
#pragma once

#include <map>
#include <queue>
#include "include/cef_client.h"
#include "cef_osr_browser_win.h"

namespace cef_ui
{
	static const unsigned int MAX_APPLY_COUNT = 5;
	class cef_ui_client : public CefClient, public CefDisplayHandler, public CefLifeSpanHandler, public CefLoadHandler, public CefRenderHandler
	{
	public:
		explicit cef_ui_client(HWND hWnd);
		~cef_ui_client();

		// Provide access to the single global instance of this object.
		static cef_ui_client* GetInstance();

		// CefClient methods:
		virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
			return this;
		}
		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
			return this;
		}
		virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }

		virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override
		{
			return this;
		}

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

		// CefLifeSpanHandler methods
		virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& target_url,
			const CefString& target_frame_name,
			WindowOpenDisposition target_disposition,
			bool user_gesture,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings,
			bool* no_javascript_access) OVERRIDE;


		virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
		virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

		void OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

		// Request that all existing browser windows close.
		void CloseAllBrowsers(bool force_close);

		// ͨ�� Identifier ��������ʹ�õ� browserWin
		CefRefPtr<cef_osr_browser> FindBrowserWin(int id);

		// ��ȡһ�����õ�(�����е�) browserWin
		CefRefPtr<cef_osr_browser> GetAvailableBrowserWin();

		// �������ҳ��
		bool CreateBrowser(std::string url);

		// ������ʹ�õ�ҳ��Żؿ��ж���
		void SetBrowserIdle(int id);
	private:
		// Platform-specific implementation.
		void PlatformTitleChange(CefRefPtr<CefBrowser> browser,
			const CefString& title);
	private:
		unsigned int m_applyCount;		// ������������
		HWND m_hWnd;
		// ��ʹ���е��б�
		typedef std::map<int, CefRefPtr<cef_osr_browser>> MAP_BROWSER;
		MAP_BROWSER m_browserWinMap;

		// �����б�
		typedef std::queue<CefRefPtr<cef_osr_browser>>  QE_BROWSER;
		QE_BROWSER	m_qeIdle;

		IMPLEMENT_REFCOUNTING(cef_ui_client); 
	};
};

#endif  //_CEF_UI_CLIENT_H_