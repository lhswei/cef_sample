#pragma once
#include <string>
#include <vector>

class LuaBundle;

struct ColorRGB {
	int r, g, b;
	ColorRGB();
	ColorRGB(int r, int g, int b);
	static ColorRGB Yellow;
	static ColorRGB GenByIndex();
};

// auto increase handle
class GHandle {
private:
	int handle;
	//GHandle(const GHandle &) {};

public:
	GHandle() : handle(GHandle::Create()) {}
	GHandle(int h) : handle(h) {}
	operator int() { return handle; }
	static int Create();
};

class GObject {

public:
	int gx, gy, gw, gh, z;
	ColorRGB color;
	GHandle handle;
	std::string title;
	bool isTop = false;
	bool inFocus = false;

private:
	GObject(const GObject &) {};

public:
	GObject();
	

	bool ExcuteCmd(int cmd, const std::vector<LuaBundle> &args);

	bool IsInRect(int x, int y);

	// 鼠标事件分发
	void OnMouseIn(int x, int y);
	void OnMouseOut(int x, int y);
	bool OnClick(int x, int y);
	bool OnRightClick(int x, int y);
	bool OnDrag(int x, int y);

private:
	//int lastX, lastY;
};