// include the basic windows header file
#include "cef_ui/cef_ui_app.h"
#include "cef_ui/cef_ui_handler.h"
#include <windows.h>
#include <windowsx.h>
#include "D3DApi.h"
#include "GameApp.h"
#include "utils.h"
#include <iostream>


// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

void BeforeCreate();
void BeforeExit();
void InitCef(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow);
void CloseCef();
void test_cout() {
	const int num = 7;
	int* ptr = const_cast<int *> (&num);
	*ptr = 8;
	int arr[num];
	std::cout << &num << " " << ptr << std::endl;
	std::cout << num << std::endl;
	std::cout << *ptr << std::endl;
	std::cout << sizeof(arr) / sizeof(num) << std::endl;

	std::cout << "========" << std::endl;
	int a = 1;
	std::cout << sizeof(a++) << std::endl;
	std::cout << a << std::endl;
}

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	InitCef(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"GameMainWindowsClass";

	// register the window class
	RegisterClassEx(&wc);

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		wc.lpszClassName,    // name of the window class
		L"GUI Demo",   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		600,    // x-position of the window
		300,    // y-position of the window
		SCREEN_WIDTH,    // width of the window
		SCREEN_HEIGHT,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

				  // display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	BeforeCreate();
	
	//test_cout();

	// enter the main loop:
	GameApp::Instance()->InitGame(hWnd);

	// this struct holds Windows event messages
	MSG msg = {0};

	// wait for the next message in the queue, store the result in 'msg'
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);
		}
		else {
			GameApp::Instance()->RunLoop();
		}
	}

	GameApp::Instance()->Release();

	CloseCef();

	BeforeExit();
	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
		case WM_DESTROY:
		{
			MessageBox(NULL,
				L"Exit and Close!",
				L"Want to exit the game!",
				MB_ICONEXCLAMATION | MB_OK);
			// close the application entirely
			PostQuitMessage(0);
			return 0;
		} break;

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
			//float xPos = GET_X_LPARAM(lParam);
			//float yPos = GET_Y_LPARAM(lParam);
			//log("Message Mouse Move: %d %d\n", xPos, yPos);

			CefRefPtr<cef_ui::cef_ui_handler> handler = cef_ui::cef_ui_handler::GetInstance();
			if (handler.get())
			{
				handler.get()->OnMouseEvent(message, wParam, lParam);
			}
		} break;
		case WM_SYSCHAR:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		{
			CefRefPtr<cef_ui::cef_ui_handler> handler = cef_ui::cef_ui_handler::GetInstance();
			if (handler.get())
			{
				handler.get()->OnKeyEvent(message, wParam, lParam);
			}
			break;
		}
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		{
			CefRefPtr<cef_ui::cef_ui_handler> handler = cef_ui::cef_ui_handler::GetInstance();
			if (handler.get())
			{
				handler.get()->OnFocus(message == WM_SETFOCUS);
			}
			break;
		}
		case WM_CREATE:
		{
			CefWindowInfo window_info;
			CefBrowserSettings browser_settings;
			CefRefPtr<cef_ui::cef_ui_handler> handler = cef_ui::cef_ui_handler::GetInstance();

			auto fun = [](const void* buffer, size_t width, size_t height)-> void {
				GameApp::Instance()->Update(buffer, width, height);
			};

			handler->SetFun(fun);

			std::string url("https://www.baidu.com/");

			//window_info.SetAsPopup(NULL, "cef browser");
			RECT wndRect;
			wndRect.left = 0;//SCREEN_WIDTH / 4;
			wndRect.top = 0;// SCREEN_HEIGHT / 4;
			wndRect.right = 300;// (SCREEN_WIDTH * 3) / 4;
			wndRect.bottom = 400;// (SCREEN_HEIGHT * 3) / 4;
			//window_info.SetAsChild(hWnd, wndRect);
			window_info.SetAsWindowless(hWnd);
			//Create the first browser window.
			CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
				NULL);
			break;

		}
		
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);

			CefRefPtr<cef_ui::cef_ui_handler> handler = cef_ui::cef_ui_handler::GetInstance();
			if (handler.get())
			{
				handler.get()->OnPaint();
			}
			break;
		}
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void ScreenSize(int x, int y) {

	COORD coord;
	coord.X = x;
	coord.Y = y;

	SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = x - 1;
	Rect.Right = y - 1;

	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(Handle, coord);
	if (SetConsoleWindowInfo(Handle, TRUE, &Rect) == FALSE) {
		std::string str = GetLastErrorAsString();
		MessageBoxA(NULL,
			str.data(),
			"Error",
			MB_ICONEXCLAMATION | MB_OK);
		//exit(-1);
	}

}

void BeforeCreate() {
#ifdef _DEBUG
	::AllocConsole();
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	//ScreenSize(100, 200);
	FILE* fp = NULL;
	OutputDebugString(L"Start Debug Console");
	::SetConsoleTitle(L"WriteSoul 3D Debug");
	freopen_s(&fp, "CONOUT$", "w+t", stdout);
#endif
}

void BeforeExit() {
#ifdef _DEBUG
	::FreeConsole();
#endif
}

void InitCef(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

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
	CefRefPtr<cef_ui::cef_ui_handler> handler(new cef_ui::cef_ui_handler(app));
	settings.multi_threaded_message_loop = 1;
	settings.windowless_rendering_enabled = 1;
	//settings.single_process = 1;
	// Initialize CEF.
	CefInitialize(main_args, settings, app.get(), NULL);

}

void CloseCef() {
	// Shut down CEF.
	CefShutdown();
}
