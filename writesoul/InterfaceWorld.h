#pragma once
#include <vector>
#include <memory>
#include "GObject.h"

extern "C" {
#include "lua.h"
#include "luadebug.h"
#include "lualib.h"
}

/*
绑定lua，创建界面
*/
//class GObject;
//class GHandle;

class InterfaceWorld {
public:
	InterfaceWorld();
	std::vector<std::unique_ptr<GObject>> allObjects;
	void CreateTestPanels();
	GHandle CreatePanel(int x, int y, int w, int h);
	void Sort();

	GObject* GetObj(int handle);

	void OnEvent(int x, int y, bool lClick, bool rClick, bool drag);

public:
	GHandle focusObj;
	int lastX, lastY;
};

// lua的参数的封装
class LuaBundle {
public:
	void GetVal(lua_State *L);
	static std::vector<LuaBundle> Parse(lua_State *L, int s, int l);
public:
	int val;
	std::string str;
};