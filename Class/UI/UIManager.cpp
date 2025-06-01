#include "UIManager.h"
#include "../Player/Player.h"

using namespace LWP;
using namespace LWP::Math;

UIManager::UIManager(Player* player) {
	player_ = player;

	// HP
	hp_.Initialize();
	// 鞘ゲージ
	sheathGauge_.Initialize();

	guideOperation_.resize(6);
	guideOperation_[0].LoadTexture("UI/UI_Button.png");
	guideOperation_[0].anchorPoint = { 1,0 };
	guideOperation_[0].Init();

	guideOperation_[1].LoadTexture("UI/LRButton0.png");
	guideOperation_[1].anchorPoint = { 1,0 };
	guideOperation_[1].Init();

	guideOperation_[2].LoadTexture("UI/LRButton1.png");
	guideOperation_[2].anchorPoint = { 1,0 };
	guideOperation_[2].Init();

	guideOperation_[3].LoadTexture("UI/UI_LTrigger.png");
	guideOperation_[3].anchorPoint = { 1,0 };
	guideOperation_[3].Init();

	guideOperation_[4].LoadTexture("UI/UI_RTrigger.png");
	guideOperation_[4].anchorPoint = { 1,0 };
	guideOperation_[4].Init();

	guideOperation_[5].LoadTexture("UI/UI_Move.png");
	guideOperation_[5].anchorPoint = { 1,0 };
	guideOperation_[5].Init();

	// jsonの値を保存
	json_.Init("UI_Config");
	json_.BeginGroup("Button")

		.BeginGroup("Attack")
		.AddValue<Vector3>("Translation", &guideOperation_[0].worldTF.translation)
		.EndGroup()
		.BeginGroup("Evasion")
		.AddValue<Vector3>("Translation", &guideOperation_[1].worldTF.translation)
		.EndGroup()
		.BeginGroup("Dash")
		.AddValue<Vector3>("Translation", &guideOperation_[2].worldTF.translation)
		.EndGroup()
		.BeginGroup("LockOn")
		.AddValue<Vector3>("Translation", &guideOperation_[3].worldTF.translation)
		.EndGroup()
		.BeginGroup("Parry")
		.AddValue<Vector3>("Translation", &guideOperation_[4].worldTF.translation)
		.EndGroup()
		.BeginGroup("Move")
		.AddValue<Vector3>("Translation", &guideOperation_[5].worldTF.translation)
		.EndGroup()

		.EndGroup()
		.CheckJsonFile();

	// 座標適用
	guideOperation_[2].worldTF = guideOperation_[1].worldTF;
}

void UIManager::Initialize() {

}

void UIManager::Update() {
	// 回避中はダッシュUIに変更
	if (player_->GetSystemManager()->GetEvasionSystem()->GetIsActive()) {
		guideOperation_[1].isActive = false;
		guideOperation_[2].isActive = true;
	}
	else {
		guideOperation_[2].isActive = false;
		guideOperation_[1].isActive = true;
	}

	// HP
	hp_.Update();
	// 鞘ゲージ
	sheathGauge_.Update();
}

void UIManager::DebugGUI() {
	json_.DebugGUI();

	sheathGauge_.DebugGUI();

	hp_.DebugGUI();
}
