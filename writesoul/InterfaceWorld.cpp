#include "InterfaceWorld.h"
#include <algorithm>

InterfaceWorld::InterfaceWorld() : focusObj(0) {
	lastX = INT_MIN;
	lastY = INT_MIN;
}

void InterfaceWorld::CreateTestPanels() {
	auto obj = std::make_unique<GObject>();
	//obj->color = ColorRGB::Yellow;
	obj->gx = obj->gy = 50;
	obj->gw = obj->gh = 50;
	allObjects.push_back(std::move(obj));
	obj = std::make_unique<GObject>();
	obj->gx = obj->gy = 0;
	obj->gw = 50;
	obj->gh = 50;
	allObjects.push_back(std::move(obj));
}

void InterfaceWorld::Sort() {
	// 逆序排
	std::sort(allObjects.begin(), allObjects.end(), [](const std::unique_ptr<GObject> & a, const std::unique_ptr<GObject> & b) {
		if (a->isTop) return false;
		else if (b->isTop) return true;
		return a->z < b->z;
	});
}

GObject* InterfaceWorld::GetObj(int h) {
	auto res = std::find_if(allObjects.begin(), allObjects.end(), [h](auto const & obj) {
		return obj->handle == h;
	});
	return res != allObjects.end() ? res->get() : nullptr;
}


GHandle InterfaceWorld::CreatePanel(int x, int y, int w, int h) {
	auto obj = std::make_unique<GObject>();
	obj->gx = x;
	obj->gy = y;
	obj->gw = w;
	obj->gh = h;
	int handle = obj->handle;
	allObjects.push_back(std::move(obj));
	return handle;
}

void InterfaceWorld::OnEvent(int x, int y, bool lClick, bool rClick, bool drag) {
	if (lastX == INT_MIN) {
		lastX = x;
		lastY = y;
	}

	for (auto iter = allObjects.rbegin(); iter != allObjects.rend(); iter++) {
		GObject *obj = iter->get();
		if (obj->IsInRect(x, y)) {
			if (focusObj != obj->handle) {
				GObject* last = GetObj(focusObj);
				if (last != nullptr) {
					last->OnMouseOut(x, y);
				}
				obj->OnMouseIn(x, y);
			}
			else {
				if (drag) {
					obj->OnDrag(x - lastX, y - lastY);
				}
				else {
					if (lClick) obj->OnClick(x, y);
					if (rClick) obj->OnRightClick(x, y);
				}
			}
			lastX = x; 
			lastY = y;
			focusObj = obj->handle;
			return;
		}
	}
	// 移到没有界面的地方
	lastX = x;
	lastY = y;
	GObject* last = GetObj(focusObj);
	if (last != nullptr) {
		last->OnMouseOut(x, y);
	}
	focusObj = 0;
}

std::vector<LuaBundle> LuaBundle::Parse(lua_State *L, int s, int l) {
	std::vector<LuaBundle> v;
	for (int i = s; i < s + l; i++) {
		int n1 = lua_tonumber(L, i);
		LuaBundle l;
		l.val = n1;
		v.push_back(l);
	}
	return v;
}