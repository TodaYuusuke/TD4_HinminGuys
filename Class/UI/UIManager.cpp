#include "UIManager.h"

UIManager::UIManager() {
	hp_.Initialize();
	sheathGauge_.Initialize();
}

void UIManager::Initialize() {

}

void UIManager::Update() {
	hp_.Update();
	sheathGauge_.Update();
}

void UIManager::DebugGUI() {

}
