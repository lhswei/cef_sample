#include "gamedefine.h"
#include "gamewin.h"
#include "../utils.h"

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

std::unique_ptr<GameWin> GameWin::m_instance;
std::once_flag GameWin::m_onceFlag;

template <class T, class R, typename... Args>
MyDelegate<T, R, Args...> CreateDelegate(T* t, R (T::*f)(Args...))
{
    return MyDelegate<T, R, Args...>(t, f);
}

GameWin::GameWin() {

}

void GameWin::Release() {

}

GameWin* GameWin::Instance() {
	std::call_once(m_onceFlag, []() {
		log("init game windows!\n");
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
		SCREEN_XPOS,    // x-position of the window
		SCREEN_YPOS,    // y-position of the window
		SCREEN_WIDTH,    // width of the window
		SCREEN_HEIGHT,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

	m_HWnd = hWnd;
	// display the window on the screen
	ShowWindow(hWnd, nCmdShow);
}

bool GameWin::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam) {

	return true;
}

bool GameWin::OnSize() {
	
	return true;
}

bool GameWin::OnFocus(bool setFocus) {

	return true;
}

bool GameWin::OnCaptureLost() {
	
	return true;
}

bool GameWin::OnKeyEvent(UINT message, WPARAM wParam, LPARAM lParam) {

	return true;
}

bool GameWin::OnPaint() {
	
	return true;
}

bool GameWin::OnEraseBkgnd() {
	
	return true;
}

bool GameWin::OnIMESetContext(UINT message, WPARAM wParam, LPARAM lParam) {

	return true;
}

bool GameWin::OnIMEStartComposition() {
	
	return true;
}

bool GameWin::OnIMEComposition(UINT message, WPARAM wParam, LPARAM lParam) {

	return true;
}

bool GameWin::OnIMECancelCompositionEvent() {
	
	return true;
}

void GameWin::RegisterEvent(Even_Type eventId, void* ppfun){
	if (!ppfun)
		return;

	++m_eventId;
	auto itMap = m_eventMap.find(eventId);
	if (itMap == m_eventMap.end()){
		FUN_MAP funMap;
		funMap.insert(FUN_MAP::value_type(m_eventId, ppfun));
	}
	else {
		itMap->second.insert(FUN_MAP::value_type(m_eventId, ppfun));
	}
}

void GameWin::UnRegisterEvent(int eventId){
	for (auto itMap = m_eventMap.begin() ; itMap != m_eventMap.end(); ++itMap){
		auto it = itMap->second.find(eventId);
		if (it != itMap->second.end()){
			itMap->second.erase(it);
			return;
		}
	}
}
// ===================================================================================
// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GameWin* self = GameWin::Instance();

	if (!self)
    	return DefWindowProc(hWnd, message, wParam, lParam);

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
		case WM_IME_SETCONTEXT:
			self->OnIMESetContext(message, wParam, lParam);
			return 0;
		case WM_IME_STARTCOMPOSITION:{
			self->OnIMEStartComposition();
			return 0;	
		case WM_IME_COMPOSITION:
			self->OnIMEComposition(message, wParam, lParam);
			return 0;
		case WM_IME_ENDCOMPOSITION:
			self->OnIMECancelCompositionEvent();
			// Let WTL call::DefWindowProc() and release its resources.
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_MOUSELEAVE:
		case WM_MOUSEWHEEL:
			self->OnMouseEvent(message, wParam, lParam);
			break;

		case WM_SIZE:
			self->OnSize();
			break;

		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			self->OnFocus(message == WM_SETFOCUS);
			break;

		case WM_CAPTURECHANGED:
		case WM_CANCELMODE:
			self->OnCaptureLost();
			break;

		case WM_SYSCHAR:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
			self->OnKeyEvent(message, wParam, lParam);
			break;

		case WM_PAINT:
			self->OnPaint();
			return 0;

		case WM_ERASEBKGND:
			if (self->OnEraseBkgnd())
				break;
			// Don't erase the background.
			return 0;
		default:
		{

		}
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}