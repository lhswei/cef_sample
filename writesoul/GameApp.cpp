#include "GameApp.h"
#include <Windows.h>
#include <Wincon.h>
#include "utils.h"

std::unique_ptr<GameApp> GameApp::_instance;
std::once_flag GameApp::onceFlag;

GameApp::GameApp() {
	d3d9.reset(new D3DApi);
}

GameApp* GameApp::Instance() {
	std::call_once(onceFlag, []() {
		log("init game app\n");
		_instance.reset(new GameApp);
	});
	return _instance.get();
}

bool GameApp::InitGame(HWND hwnd) {
	d3d9->InitD3d(hwnd);
	mHwnd = hwnd;
	return setup();
}

bool GameApp::setup() {
	initLuaState();
	uiWorld.CreateTestPanels();
	return true;
}

void GameApp::RunLoop() {
	getInputEvents();
	uiWorld.Sort();
	d3d9->RenderFrame(uiWorld.allObjects);
	/*std::for_each(uiWorld.allObjects.begin(), uiWorld.allObjects.end(), [](auto const& obj) {
		log("Handle: %d\n", obj->handle);
	});*/

	uiWorld.OnEvent(mouseX, mouseY, leftClick, rightClick, isDrag);
}

void GameApp::getInputEvents() {
	leftClick = rightClick = false;
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(mHwnd, &p);

	if (mouseX != p.x && mouseY != p.y) {
		isDrag = true;
	}

	RECT clientRect;
	GetClientRect(mHwnd, &clientRect);
	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0) {
		leftClick = true;
		//log("Left Click: (%d,%d)[%d:%d]\n", mouseX, mouseY, clientRect.right, clientRect.bottom);
	}

	if ((GetKeyState(VK_RBUTTON) & 0x100) != 0) {
		rightClick = true;
		//log("Right Click: (%d,%d)[%d:%d]\n", mouseX, mouseY, clientRect.right, clientRect.bottom);
	}

	isDrag &= leftClick;
	if (isDrag) {
		//log("Drag (%d,%d) To (%d,%d).\n", mouseX, mouseX, p.x, p.y);
	}

	mouseX = p.x;
	mouseY = p.y;
	wchar_t str[100];
	swprintf_s(str, L"UI Demo (%d, %d)", mouseX, mouseY);
	SetWindowText(mHwnd, str);
}

void GameApp::Release() {
	d3d9->Clean3d();
}

//--------------------------------- Lua Func
int C_CreateUI(lua_State *L) {
	if (lua_gettop(L) != 4) {
		lua_error(L, "expecting exactly 4 arguments");
		return 0;
	}
	int n1 = lua_tonumber(L, 1);
	int n2 = lua_tonumber(L, 2);
	int n3 = lua_tonumber(L, 3);
	int n4 = lua_tonumber(L, 4);
	InterfaceWorld& ui = GameApp::Instance()->uiWorld;
	GHandle handle = ui.CreatePanel(n1, n2, n3, n4);

	log("C_CreateUI#%d£¨%d,%d£©[%d:%d]\n", handle, n1, n2, n3, n4);
	lua_pushnumber(L, handle);
	return 1;
}

int C_ExcuteUICommand(lua_State *L) {
	int handle = lua_tonumber(L, 1);
	int cmd = lua_tonumber(L, 2);
	int arg_num = lua_tonumber(L, 3);
	std::vector<LuaBundle> args = LuaBundle::Parse(L, 4, arg_num);
	InterfaceWorld& ui = GameApp::Instance()->uiWorld;

	GObject* obj = ui.GetObj(handle);
	if (obj != nullptr) {
		obj->ExcuteCmd(cmd, args);
	}

	return 0;
}

void GameApp::initLuaState() {
	lua_State *L = lua_open(1 << 16);
	lua_baselibopen(L);
	lua_iolibopen(L);
	lua_strlibopen(L);
	lua_mathlibopen(L);
	lua_dblibopen(L);
	lua_register(L, "C_CreateUI", C_CreateUI);
	lua_register(L, "C_ExcuteUICommand", C_ExcuteUICommand);
	lua_dofile(L, "create_ui.lua");
}