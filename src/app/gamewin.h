#ifndef _GAME_WIN_H_
#define _GAME_WIN_H_
#include <memory>
#include <mutex>
#include <Windows.h>

class GameWin {
private:
	GameWin();
	GameWin(GameWin const &) = delete;
	GameWin& operator = (const GameWin &) = delete;

public:
	static GameWin* Instance();
	void InitWin(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
private:
	HWND m_Hwnd;
	static std::unique_ptr<GameWin> m_instance;
	static std::once_flag m_onceFlag;
};

#endif //_GAME_WIN_H_
