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
	// 走り(回避)
	guideOperation_[1].LoadTexture("UI/ButtonUI/ButtonUI_Run.png");
	// 
	guideOperation_[2].LoadTexture("UI/ButtonUI/LRButton1.png");
	guideOperation_[2].isActive = false;
	// ロックオン
	guideOperation_[3].LoadTexture("UI/ButtonUI/UI_LTrigger.png");
	// パリィ
	guideOperation_[4].LoadTexture("UI/ButtonUI/ButtonUI_Parry.png");
	// 移動
	guideOperation_[5].LoadTexture("UI/ButtonUI/UI_Move.png");
	// 鞘
	guideOperation_[6].LoadTexture("UI/ButtonUI/ButtonUI_Throw.png");
	for (int i = 0; i < guideOperation_.size(); i++) {
		guideOperation_[i].anchorPoint = { 0.5f, 0.0f };
	}

	coolTimeFilter_.resize(7);
	coolTimeFilterSplitSize_.resize(7);
	// クールタイムのフィルタ
	for (int i = 0; i < coolTimeFilter_.size(); i++) {
		coolTimeFilter_[i].LoadTexture("UI/ButtonUI/ButtonUI_Shadow.png");
		coolTimeFilter_[i].anchorPoint = { 0.5f,0.0f };
		coolTimeFilter_[i].isActive = false;
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



	// 各UIの残像
	afterimage_.resize(7);
	//攻撃
	afterimage_[0].sprite.LoadTexture("UI/ButtonUI/ButtonUI_Attack.png");
	// 走り(回避)
	afterimage_[1].sprite.LoadTexture("UI/ButtonUI/ButtonUI_Run.png");
	// 
	afterimage_[2].sprite.LoadTexture("UI/ButtonUI/LRButton1.png");
	afterimage_[2].sprite.isActive = false;
	// ロックオン
	afterimage_[3].sprite.LoadTexture("UI/ButtonUI/UI_LTrigger.png");
	// パリィ
	afterimage_[4].sprite.LoadTexture("UI/ButtonUI/ButtonUI_Parry.png");
	// 移動
	afterimage_[5].sprite.LoadTexture("UI/ButtonUI/UI_Move.png");
	// 鞘
	afterimage_[6].sprite.LoadTexture("UI/ButtonUI/ButtonUI_Throw.png");
	for (int i = 0; i < afterimage_.size(); i++) {
		afterimage_[i].sprite = guideOperation_[i];
		afterimage_[i].sprite.isActive = false;
	}
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

void UIManager::DebugGUI() {
	json_.DebugGUI();

	sheathGauge_.DebugGUI();

	coolTimeFilter_[6].DebugGUI();

	hp_.DebugGUI();
}

void UIManager::CoolTimeFilterUpdate() {
	// 攻撃
	if (player_->GetSystemManager()->GetComboTree()->GetStifnessProgress() > 0.0f) {
		coolTimeFilter_[0].isActive = true;
		coolTimeFilterSplitSize_[0].y = 150.0f * (1.0f - player_->GetSystemManager()->GetComboTree()->GetStifnessProgress());
		coolTimeFilter_[0].clipRect.max.y = coolTimeFilterSplitSize_[0].y;
	}

	// 鞘
	if (!player_->GetSystemManager()->GetCoolTimer()->GetSheathCoolTimeData().isFinish) {
		coolTimeFilter_[6].isActive = true;
		coolTimeFilterSplitSize_[6].y = 150.0f * player_->GetSystemManager()->GetCoolTimer()->GetSheathCoolTimeData().coolTime / player_->GetSystemManager()->GetCoolTimer()->GetSheathCoolTimeData().maxCoolTime;
		coolTimeFilter_[6].clipRect.max.y = coolTimeFilterSplitSize_[6].y;
	}
}

void UIManager::StartEnableUIEffect(const int& id) {
	afterimage_[id].sprite.isActive = true;

	// 徐々に大きくなる
	afterimage_[id].sprite.worldTF.scale = Utility::Interpolation::Lerp(afterimage_[id].startScale, afterimage_[id].endScale, Utility::Easing::InExpo(afterimage_[id].currentFrame / afterimage_[id].endFrame));
	// 透明度
	afterimage_[id].sprite.material.color.A = (int)Utility::Interpolation::LerpF(afterimage_[id].alpha, 0.0f, Utility::Easing::InExpo(afterimage_[id].currentFrame / afterimage_[id].endFrame));

	afterimage_[id].currentFrame++;
}