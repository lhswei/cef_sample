#ifndef _UTILS_H_
#define _UTILS_H_

#include <cstdio>
#include <stdarg.h> 
#include <Windows.h>
#include <string>
#include <algorithm>
#include <cassert>

#ifdef _DEBUG
void log_message(const char * fmt, ...);
#define log log_message
#else 
#define log 
#endif

std::string GetLastErrorAsString();

#endif //_UTILS_H_