#include "GObject.h"
#include "utils.h"

ColorRGB::ColorRGB() {
	r = g = b = 255;
}

ColorRGB::ColorRGB(int r, int g, int b):r(r), g(g), b(b) {
}

ColorRGB ColorRGB::Yellow = ColorRGB(255, 128, 0);

ColorRGB ColorRGB::GenByIndex() {
	static int idx = 1;
	static std::vector<ColorRGB> colors = {
		ColorRGB(255, 0, 0),
		ColorRGB(255, 64, 0),
		ColorRGB(255, 128, 0),
		ColorRGB(255, 191, 0),
		ColorRGB(255, 255, 0),
		ColorRGB(191, 255, 0),
		ColorRGB(128, 255, 0),
		ColorRGB(64, 255, 0),
		ColorRGB(0, 255, 0),
		ColorRGB(0, 255, 64),
		ColorRGB(0, 255, 128),
		ColorRGB(0, 255, 191),
		ColorRGB(0, 64, 255),
		ColorRGB(255, 0, 191),
	};
	idx = (idx + 1) % colors.size();
	return colors[idx];
}

int GHandle::Create() {
	static int idx = 0;
	return ++idx;
}

GObject::GObject() {
	gx = gy = gw = gh = 0;
	z = 200;
	color = ColorRGB::GenByIndex();
	title = "#" + handle;
}

bool GObject::ExcuteCmd(int cmd, const std::vector<LuaBundle> &args) {

	return true;
}

bool GObject::IsInRect(int x1, int y1) {
	int lx = gx + gw, ly = gy + gh;
	if (x1 > gx && x1 < lx && y1 > gy && y1 < ly) {
		//log("#%d->IsInRect:(%d,%d) [%d,%d,%d,%d]\n", (int) handle, x1, y1, gx, gy, lx, ly);
		return true;
	}
	/*else {
		log("#%d not in:(%d,%d) [%d,%d,%d,%d]\n", (int)handle, x1, y1, gx, gy, lx, ly);
	}*/
	return false;
}

void GObject::OnMouseIn(int x, int y) {
	log("#%d->OnMouseIn:(%d,%d)\n", handle, x, y);
	
}

void GObject::OnMouseOut(int x, int y) {
	log("#%d->OnMouseOut:(%d,%d)\n", handle, x, y);
}

bool GObject::OnClick(int x, int y) {
	log("#%d->OnClick:(%d,%d)\n", handle, x, y);
	return true;
}

bool GObject::OnRightClick(int x, int y) {
	log("#%d->OnRightClick:(%d,%d)\n", handle, x, y);
	return true;
}

bool GObject::OnDrag(int mx, int my) {
	gx += mx;
	gy += my;
	log("#%d->OnDrag:(%d,%d)\n", handle, mx, my);
	return true;
}