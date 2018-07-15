#ifndef _GAME_WIN_H_
#define _GAME_WIN_H_
#include <memory>
#include <mutex>
#include <Windows.h>
#include <map>
#include <functional>
#include "gamedefine.h"

template <class T, class R, typename... Args>
class  MyDelegate
{
public:
    MyDelegate(T* t, R  (T::*f)(Args...) ):m_t(t),m_f(f) {}

    R operator()(Args&&... args) 
    {
            return (m_t->*m_f)(std::forward<Args>(args) ...);
    }

private:
    T* m_t;
    R  (T::*m_f)(Args...);
};   

template <class T, class R, typename... Args>
MyDelegate<T, R, Args...> CreateDelegate(T* t, R (T::*f)(Args...));



typedef std::map<int, void*> FUN_MAP;
typedef std::map<Even_Type, FUN_MAP> EVENT_MAP;

class GameWin {
private:
	GameWin();
	GameWin(GameWin const &) = delete;
	GameWin& operator = (const GameWin &) = delete;

public:
	void InitWin(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	void Release();
	HWND GetWwnd() {return m_HWnd;}

	// WndProc message handlers.
	bool OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
	bool OnSize();
	bool OnFocus(bool setFocus);
	bool OnCaptureLost();
	bool OnKeyEvent(UINT message, WPARAM wParam, LPARAM lParam);
	bool OnPaint();
	bool OnEraseBkgnd();

	bool OnIMESetContext(UINT message, WPARAM wParam, LPARAM lParam);
	bool OnIMEStartComposition();
	bool OnIMEComposition(UINT message, WPARAM wParam, LPARAM lParam);
	bool OnIMECancelCompositionEvent();

	void RegisterEvent(Even_Type eventId, void* ppfun);
	void UnRegisterEvent(int eventId);
public:
	static GameWin* Instance();

private:
	HWND m_HWnd;
	static std::unique_ptr<GameWin> m_instance;
	static std::once_flag m_onceFlag;
	int m_eventId = 0;
	EVENT_MAP m_eventMap;
};

#endif //_GAME_WIN_H_
