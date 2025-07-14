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

	guideOperation_.resize(7);
	//攻撃
	guideOperation_[0].LoadTexture("UI/ButtonUI/ButtonUI_Attack.png");
	guideOperation_[0].anchorPoint = { 1,0 };
	guideOperation_[0].Init();
	// 走り(回避)
	guideOperation_[1].LoadTexture("UI/ButtonUI/ButtonUI_Run.png");
	guideOperation_[1].anchorPoint = { 1,0 };
	guideOperation_[1].Init();
	// 
	guideOperation_[2].LoadTexture("UI/ButtonUI/LRButton1.png");
	guideOperation_[2].anchorPoint = { 1,0 };
	guideOperation_[2].isActive = false;
	guideOperation_[2].Init();
	// ロックオン
	guideOperation_[3].LoadTexture("UI/ButtonUI/UI_LTrigger.png");
	guideOperation_[3].anchorPoint = { 1,0 };
	guideOperation_[3].Init();
	// パリィ
	guideOperation_[4].LoadTexture("UI/ButtonUI/ButtonUI_Parry.png");
	guideOperation_[4].anchorPoint = { 1,0 };
	guideOperation_[4].Init();
	// 移動
	guideOperation_[5].LoadTexture("UI/ButtonUI/UI_Move.png");
	guideOperation_[5].anchorPoint = { 1,0 };
	guideOperation_[5].Init();
	// 鞘
	guideOperation_[6].LoadTexture("UI/ButtonUI/ButtonUI_Throw.png");
	guideOperation_[6].anchorPoint = { 1,0 };
	guideOperation_[6].Init();

	coolTimeFilter_.resize(7);
	coolTimeFilterSplitSize_.resize(7);
	// クールタイムのフィルタ
	for (int i = 0; i < coolTimeFilter_.size(); i++) {
		coolTimeFilter_[i].LoadTexture("UI/ButtonUI/ButtonUI_Shadow.png");
		coolTimeFilter_[i].anchorPoint = { 1,0 };
		coolTimeFilter_[i].isActive = false;
		coolTimeFilter_[i].Init();
		coolTimeFilter_[i].worldTF.Parent(&guideOperation_[i].worldTF);

		coolTimeFilterSplitSize_[i] = { 150.0f, 150.0f };
	}



	// jsonの値を保存
	json_.Init("UI_Config");
	json_.BeginGroup("Button")

		.BeginGroup("Attack")
		.BeginGroup("CoolTimeFilter")
		.AddValue<Vector3>("Translation", &coolTimeFilter_[0].worldTF.translation)
		.EndGroup()
		.AddValue<Vector3>("Translation", &guideOperation_[0].worldTF.translation)
		.EndGroup()
		.BeginGroup("Evasion")
		.BeginGroup("CoolTimeFilter")
		.AddValue<Vector3>("Translation", &coolTimeFilter_[1].worldTF.translation)
		.EndGroup()
		.AddValue<Vector3>("Translation", &guideOperation_[1].worldTF.translation)
		.EndGroup()
		.BeginGroup("Dash")
		.BeginGroup("CoolTimeFilter")
		.AddValue<Vector3>("Translation", &coolTimeFilter_[2].worldTF.translation)
		.EndGroup()
		.AddValue<Vector3>("Translation", &guideOperation_[2].worldTF.translation)
		.EndGroup()
		.BeginGroup("LockOn")
		.BeginGroup("CoolTimeFilter")
		.AddValue<Vector3>("Translation", &coolTimeFilter_[3].worldTF.translation)
		.EndGroup()
		.AddValue<Vector3>("Translation", &guideOperation_[3].worldTF.translation)
		.EndGroup()
		.BeginGroup("Parry")
		.BeginGroup("CoolTimeFilter")
		.AddValue<Vector3>("Translation", &coolTimeFilter_[4].worldTF.translation)
		.EndGroup()
		.AddValue<Vector3>("Translation", &guideOperation_[4].worldTF.translation)
		.EndGroup()
		.BeginGroup("Move")
		.BeginGroup("CoolTimeFilter")
		.AddValue<Vector3>("Translation", &coolTimeFilter_[5].worldTF.translation)
		.EndGroup()
		.AddValue<Vector3>("Translation", &guideOperation_[5].worldTF.translation)
		.EndGroup()
		.BeginGroup("Sheath")
		.BeginGroup("CoolTimeFilter")
		.AddValue<Vector3>("Translation", &coolTimeFilter_[6].worldTF.translation)
		.EndGroup()
		.AddValue<Vector3>("Translation", &guideOperation_[6].worldTF.translation)
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
	if (player_->GetSystemManager()->GetIsEnableDash()) {
		guideOperation_[1].isActive = false;
		guideOperation_[2].isActive = true;
	}
	else {
		guideOperation_[2].isActive = false;
		guideOperation_[1].isActive = true;
	}

	// クールタイムのフィルタ
	CoolTimeFilterUpdate();


	// HP
	hp_.Update();
	// 鞘ゲージ
	sheathGauge_.Update();
}
Vector2 aaaa{};
void UIManager::DebugGUI() {
	json_.DebugGUI();
	ImGui::DragFloat2("size", &aaaa.x, 0.1f,0.0f);
	sheathGauge_.DebugGUI();

	hp_.DebugGUI();
//	coolTimeFilter_[6].SetSplitSize(aaaa);
//	coolTimeFilter_[6].Init();
}

void UIManager::CoolTimeFilterUpdate() {
	// 攻撃


	// 鞘
	if (!player_->GetSystemManager()->GetCoolTimer()->GetSheathCoolTimeData().isFinish) {
		coolTimeFilter_[6].isActive = true;
		coolTimeFilterSplitSize_[6].y = 150.0f * player_->GetSystemManager()->GetCoolTimer()->GetSheathCoolTimeData().coolTime / player_->GetSystemManager()->GetCoolTimer()->GetSheathCoolTimeData().maxCoolTime;
		coolTimeFilter_[6].SetSplitSize(coolTimeFilterSplitSize_[6]);
		coolTimeFilter_[6].Init();
	}
	else {
		coolTimeFilter_[6].isActive = false;
		coolTimeFilterSplitSize_[6].y = 150.0f;
		coolTimeFilter_[6].SetSplitSize(coolTimeFilterSplitSize_[6]);
		coolTimeFilter_[6].Init();
	}
}
