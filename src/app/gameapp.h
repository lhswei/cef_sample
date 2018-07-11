#ifndef _GAME_APP_H_
#define _GAME_APP_H_

#include <memory>
#include <mutex>
#include <Windows.h>
#include "gamedefine.h"

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
	void Run();
	void Release();

private:
	// 循环
	void Loop();

	// 每毫秒执行
	void OnLoo();

	// 每秒执行
	void OnSec();

	// 每帧执行
	void OnFps();
private:
	HWND m_Hwnd;
	static std::unique_ptr<GameApp> m_instance;
	static std::once_flag m_onceFlag;

	bool m_bRun = false;							// 是否运行
	LUTimer<std::chrono::seconds> m_timerSec;		// 秒定时器
    LUTimer<LT_GAME_FPS> m_timerFps;				// 帧定时器
};

#endif //_GAME_APP_H_
