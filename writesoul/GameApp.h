#pragma once
#include <memory>
#include <mutex>
#include <Windows.h>
#include "D3DApi.h"
#include "InterfaceWorld.h"

extern "C" {
#include "lua.h"
#include "luadebug.h"
#include "lualib.h"
}

class GameApp {
private:
	static std::unique_ptr<GameApp> _instance;
	static std::once_flag onceFlag;
	//GameApp() = default;
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

public:
	InterfaceWorld uiWorld;
private:
	HWND mHwnd;
	std::unique_ptr<D3DApi> d3d9;
	int mouseX, mouseY;
	bool leftClick, rightClick, isDrag;
};

