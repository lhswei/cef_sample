#include <windows.h>
#include "gamewin.h"
#include "gameapp.h"

// Entry point function for all processes.
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

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

	return 0;
}