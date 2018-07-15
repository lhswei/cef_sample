#ifndef _GAME_DEFINE_H_
#define _GAME_DEFINE_H_

#include "ltimer.h"

const int MILLIS_PER_SECONDE = 1000;
const int GAME_FPS = 18;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int SCREEN_XPOS = 600;
const int SCREEN_YPOS = 300;

typedef std::chrono::duration<int64_t, ratio<1, GAME_FPS>> LT_GAME_FPS;

enum class  Even_Type {
	EI_MOUSE,
	EI_KEY,
	EI_PAIN,
	EI_RESIZE,
	EI_FOCUS,
	EI_CAPTURELOST,
	EI_ERASEBKGND,
	EI_IMESET,
	EI_IMESTART,
	EI_IMECOMP,
	EI_IMECANCELCOMP,
};

#endif //_GAME_DEFINE_H_
