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

#endif //_GAME_DEFINE_H_
