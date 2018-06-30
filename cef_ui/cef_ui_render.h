#ifndef _CEF_UI_RENDER_H_
#define _CEF_UI_RENDER_H_

#include "cef_headers/include/cef_base.h"
#include "cef_headers/include/cef_render_handler.h"
#include "cef_headers/include/cef_client.h"

namespace cef_ui
{
    class cef_ui_app;
    class cef_ui_render : public CefClient, public CefRenderHandler
    {
      public:
        explicit cef_ui_render();
        ~cef_ui_render();

        virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override
        {
            return this;
        }

        virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
        virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

		// void set_canvas(icanvas* canvas_ptr);
		void trigger_resize();

	private:
		// icanvas* canvas = nullptr;

		//CefRefPtr<cef_ui_app> m_app;
		std::vector<CefRefPtr<CefBrowser>> browserList;

		IMPLEMENT_REFCOUNTING(cef_ui_render);
    };
}

#endif  //_CEF_UI_RENDER_H_
