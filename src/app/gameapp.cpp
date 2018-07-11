#include "gameapp.h"
#include <thread>
#include "../utils.h"

GameApp::GameApp() {

}

GameApp* GameApp::Instance() {
	std::call_once(m_onceFlag, []() {
		log("ini game app!");
		m_instance.reset(new GameApp);
	});
	return m_instance.get();
}

void GameApp::Run(){
	m_bRun = true;
	while(m_bRun){
		Loop();
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