#ifndef _CEF_UI_HANDLER_H_
#define _CEF_UI_HANDLER_H_

#include <list>
#include <functional>
#include "cef_headers/include/cef_client.h"


namespace cef_ui
{
	class cef_ui_app;

	class cef_ui_handler : public CefClient, public CefDisplayHandler, public CefLifeSpanHandler, public CefLoadHandler, public CefRenderHandler
	{
	public:
        explicit cef_ui_handler(CefRefPtr<cef_ui_app> app);
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

		virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE
		{
			return this;
		}

		virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
		virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

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

		// WndProc message handlers.
		void OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
		void OnSize();
		void OnFocus(bool setFocus);
		void OnCaptureLost();
		void OnKeyEvent(UINT message, WPARAM wParam, LPARAM lParam);
		void OnPaint();
		bool OnEraseBkgnd();

		void OnIMESetContext(UINT message, WPARAM wParam, LPARAM lParam);
		void OnIMEStartComposition();
		void OnIMEComposition(UINT message, WPARAM wParam, LPARAM lParam);
		void OnIMECancelCompositionEvent();

		// Manage popup bounds.
		//bool IsOverPopupWidget(int x, int y) const;
		//int GetPopupXOffset() const;
		//int GetPopupYOffset() const;
		//void ApplyPopupOffset(int& x, int& y) const;

		// Request that all existing browser windows close.
		void CloseAllBrowsers(bool force_close);

		bool IsClosing() const { return is_closing_; }

		//void trigger_resize();

		void SetFun(std::function<void(const void*, size_t, size_t)>);
	private:
		// Platform-specific implementation.
		void PlatformTitleChange(CefRefPtr<CefBrowser> browser,
			const CefString& title);

		CefRefPtr<cef_ui_app> m_app;

		// List of existing browser windows. Only accessed on the CEF UI thread.
		typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
		BrowserList browser_list_;

		bool is_closing_;

		std::function<void(const void*, size_t, size_t)> m_fun = nullptr;


		// Mouse state tracking.
		POINT last_mouse_pos_;
		POINT current_mouse_pos_;
		bool mouse_rotation_;
		bool mouse_tracking_;
		int last_click_x_;
		int last_click_y_;
		CefBrowserHost::MouseButtonType last_click_button_;
		int last_click_count_;
		double last_click_time_;
		bool last_mouse_down_on_view_;

		IMPLEMENT_REFCOUNTING(cef_ui_handler);
	};
}


#endif //_CEF_UI_HANDLER_H_