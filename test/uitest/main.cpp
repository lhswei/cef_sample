#include <windows.h>
#include "gamewin.h"
#include "gameapp.h"
#include "utils.h"

void BeforeCreate();
void BeforeExit();

// Entry point function for all processes.
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	BeforeCreate();

	// main win ui
	GameWin* pGameWin = GameWin::Instance();
	pGameWin->InitWin(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	HWND hwnd = pGameWin->GetWwnd();

	// app logic
	GameApp* pGameApp = GameApp::Instance();
	pGameApp->InitGame(hwnd);

	pGameApp->Run();

	// exit
	pGameApp->Release();
	pGameWin->Release();

	BeforeExit();
	
	return 0;
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
	::SetConsoleTitle(L"Client 3D Debug");
	freopen_s(&fp, "CONOUT$", "w+t", stdout);
#endif
}

void BeforeExit() {
#ifdef _DEBUG
	::FreeConsole();
#endif
}