#pragma once
#include <cstdio>
#include <stdarg.h> 
#include <Windows.h>
#include <string>
#include <algorithm>
#include <cassert>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

#ifdef _DEBUG
void log_message(const char * fmt, ...);
#define log log_message
#else 
#define log 
#endif

std::string GetLastErrorAsString();