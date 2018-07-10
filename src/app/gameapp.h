#ifndef _GAME_APP_H_
#define _GAME_APP_H_

#include <memory>
#include <mutex>
#include <Windows.h>

extern "C" {
#include "lua.h"
#include "luadebug.h"
#include "lualib.h"
}

class GameApp {
private:
	GameApp();
	GameApp(GameApp const &) = delete;
	GameApp& operator = (const GameApp &) = delete;
	void initLuaState();
	bool setup();
	void getInputEvents();

public:
	static GameApp* Instance();
	bool InitGame(HWND hwnd);
	void RunLoop();
	void Release();

private:
	HWND m_Hwnd;
	static std::unique_ptr<GameApp> m_instance;
	static std::once_flag m_onceFlag;
};

#endif //_GAME_APP_H_
