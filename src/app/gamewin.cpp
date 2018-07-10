#include "gamedefine.h"
#include "gamewin.h"
#include "../utils.h"

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

std::unique_ptr<GameWin> GameWin::m_instance;

GameWin::GameWin() {

}

GameWin* GameWin::Instance() {
	std::call_once(m_onceFlag, []() {
		log("ini game windows!");
		m_instance.reset(new GameWin);
	});
	return m_instance.get();
}

void GameWin::InitWin(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;
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
		L"GUI Client",   // title of the window
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
}

// ===================================================================================
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

		default:
		{

		}
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}