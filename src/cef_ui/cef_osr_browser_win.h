#ifndef _CEF_OSR_BROSWER_H_
#define _CEF_OSR_BROSWER_H_
#pragma once

#include "cef_ui_app.h"
#include "include/cef_browser.h"

namespace cef_ui
{
	class cef_osr_browser : public virtual CefBaseRefCounted
	{
	public:
		cef_osr_browser(CefRefPtr<CefBrowser> browser);
		~cef_osr_browser();

	public:
		// ����
		void ReSet();

		void AttackHandle(int h) { m_handle = h; }
		void DettackHandle() { m_handle = 0; }
		int  GetIdentifier();
		bool IsSame(CefRefPtr<CefBrowser> that);
		void Paint();

		void SetEnable(int enable) { m_enable = enable; }
		int GetW() const { return m_width; }
		int GetH() const { return m_height; }
		void ReSize(int width, int height);
		void SetW(int width) { ReSize(width, m_height); }
		void SetH(int height) { ReSize(m_width, height); }
		void OnPaint(const void* buffer, int biwidth, int biheight);

		void CloseBrowser(bool force_close = false);
		void RequestUrl(std::string& url);

		void SetFoucs(bool bFoucs = false);
		//=============���ַ�ʽ������������Ϣ============================
		// 1 ����ԭʼ��Ϣ
		void OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
		void OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
		void OnKeyEvent(UINT message, WPARAM wParam, LPARAM lParam);
		void OnWndPaint(UINT message, WPARAM wParam, LPARAM lParam);

		// 2 �������������Ϣ
		void OnKeyEvent(int key_down, int key_char);		// �����ַ�
		void OnLeftDown(int x, int y);	// �������
		void OnLeftUp(int x, int y);	// ����ɿ�
		void OnMouseMove(int x, int y);	// ����ƶ�
		void OnMouseLeave(int x, int y);// ����뿪
		void OnWheelUp(int x, int y);	// ���Ϲ���
		void OnWheelDown(int x, int y);	// ���¹���
		//=====================================================
	private:
		int m_handle;
		int m_width;
		int m_height;
		int m_enable;
		CefRefPtr<CefBrowser> m_holdBrowser;

		// Mouse state tracking.
		bool m_isFoucs = false;
		POINT m_last_mouse_pos;
		POINT m_current_mouse_pos;
		bool m_mouse_rotation = false;
		bool m_mouse_tracking = false;
		int m_last_click_x = 0;
		int m_last_click_y = 0;
		CefBrowserHost::MouseButtonType m_last_click_button;
		int m_last_click_count = 0;
		double m_last_click_time = 0;
		bool m_last_mouse_down_on_view = 0;

		IMPLEMENT_REFCOUNTING(cef_osr_browser);
	};
};


#endif // _CEF_OSR_BROSWER_H_

