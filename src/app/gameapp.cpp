#include "gameapp.h"
#include <thread>
#include "../utils.h"

std::unique_ptr<GameApp> GameApp::m_instance;
std::once_flag GameApp::m_onceFlag;

GameApp::GameApp() {

}

GameApp* GameApp::Instance() {
	std::call_once(m_onceFlag, []() {
		log("ini game app!");
		m_instance.reset(new GameApp);
	});
	return m_instance.get();
}

void GameApp::Release() {

}

void GameApp::Run(){
	static MSG msg = { 0 };
	m_bRun = true;
	while(m_bRun && msg.message != WM_QUIT){
		// this struct holds Windows event messages

		// wait for the next message in the queue, store the result in 'msg'
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);
		}
		else {
			Loop();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1)); //–›√ﬂ1∫¡√Î 
	}
}

void GameApp::Loop(){
	OnLoo();

	if(m_timerFps.ticking())
        OnFps();

    if(m_timerSec.ticking())
        OnSec();
}

void GameApp::OnLoo(){

}

void GameApp::OnFps(){
	
}

void GameApp::OnSec(){
	
}

bool GameApp::InitGame(HWND hwnd){
	m_Hwnd = hwnd;
	return true;
}