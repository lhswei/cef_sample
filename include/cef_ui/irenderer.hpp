#pragma once

#include "icanvas.hpp"

namespace cef_ui
{
	class irenderer
	{
	public:
		virtual ~irenderer() {};

		virtual bool is_available() = 0;

		virtual HWND get_window() = 0;
		virtual icanvas* get_canvas() = 0;

		virtual void register_frame_callback(std::function<void()> callback) = 0;
		virtual void unregister_frame_callback() = 0;
	};

	typedef irenderer* __cdecl create_interface_t();
}
