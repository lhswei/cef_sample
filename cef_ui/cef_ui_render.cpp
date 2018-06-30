#include "cef_ui_render.h"

namespace cef_ui
{
	cef_ui_render::cef_ui_render()
	{

	}

	cef_ui_render::~cef_ui_render()
	{

	}

	void cef_ui_render::trigger_resize()
	{
		for (auto& browser : this->browserList)
		{
			browser->GetHost()->WasResized();
		}
	}

	bool cef_ui_render::GetViewRect(CefRefPtr<CefBrowser> /*browser*/, CefRect& rect)
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

	void cef_ui_render::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType /*type*/, const RectList& /*dirtyRects*/, const void* buffer, int width, int height)
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

		browser->GetHost()->WasResized();
	}
}