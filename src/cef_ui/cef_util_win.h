#ifndef CEF_UTIL_WIN_H_
#define CEF_UTIL_WIN_H_
#pragma once
#include "include/internal/cef_types.h"
namespace cef_ui
{
	int GetCefMouseModifiers(WPARAM wparam);
	int GetCefKeyboardModifiers(WPARAM wparam, LPARAM lparam);
	bool IsKeyDown(WPARAM wparam);
};

#endif  // CEF_UTIL_WIN_H_
