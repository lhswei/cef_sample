
#include "cef_ui_app.h"
#include "cef_ui_client.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"

namespace cef_ui
{
	extern CefRefPtr<cef_ui_client> g_instance;

	cef_ui_app::cef_ui_app()
	{
	
	}

	void cef_ui_app::OnContextInitialized()
	{
		CEF_REQUIRE_UI_THREAD();

		//CefRefPtr<CefCommandLine> command_line =
		//	CefCommandLine::GetGlobalCommandLine();

	}

	void cef_ui_app::OnBeforeCommandLineProcessing(const CefString& /*process_type*/, CefRefPtr<CefCommandLine> command_line)
	{
		//command_line->AppendSwitch("enable-experimental-web-platform-features");
		//command_line->AppendSwitch("in-process-gpu");
		//command_line->AppendSwitch("enable-media-stream");
		//command_line->AppendSwitch("use-fake-ui-for-media-stream");
		//command_line->AppendSwitch("enable-speech-input");
		//command_line->AppendSwitch("ignore-gpu-blacklist");
		//command_line->AppendSwitch("enable-usermedia-screen-capture");
		command_line->AppendSwitchWithValue("default-encoding", "utf-8");
		command_line->AppendSwitch("enable-rgba-4444-textures");

		/*if (IsWindows10OrGreater())
		{
			command_line->AppendSwitch("force-gpu-rasterization");
		}*/

		command_line->AppendSwitch("disable-gpu");	// 这个选项目前是必须开的，要不然会宕机
		//command_line->AppendSwitch("disable-gpu-compositing");
		//command_line->AppendSwitch("enable-begin-frame-scheduling");
		//command_line->AppendSwitch("enable-surface-synchronization");
	}



	void InitCef(HINSTANCE hInstance) {
		// Enable High-DPI support on Windows 7 or newer.
		CefEnableHighDPISupport();

		// Provide CEF with command-line arguments.
		CefMainArgs main_args(hInstance);

		// Specify CEF global settings here.
		CefSettings settings;

		// SimpleApp implements application-level callbacks for the browser process.
		// It will create the first browser instance in OnContextInitialized() after
		// CEF has initialized.
		CefRefPtr<cef_ui::cef_ui_app> app(new cef_ui::cef_ui_app);
		// SimpleHandler implements browser-level callbacks.

		// 多线程模式，嵌入游戏中必须使用多线程模式
		settings.multi_threaded_message_loop = 1;

		// 支持离屏渲染
		settings.windowless_rendering_enabled = 1;

		// V8渲染引擎，需要多进程模式
		// 在这里不需要v8渲染，使用单进程模式
		settings.single_process = 1;
		// Initialize CEF.
		CefInitialize(main_args, settings, app.get(), NULL);

	}

	void CreateCefClient(HWND hWnd)
	{
		// 单实例模式，只能创建一次
		new cef_ui_client(hWnd);
	}

	void CloseCef() {
		// Shut down CEF.
		// cef_ui_client 的实例
		if (g_instance.get())
		{
			// g_instance 是智能指针，自动删除对象
			g_instance->CloseAllBrowsers(true);
			g_instance = NULL;
		}
		CefShutdown();
	}
}
