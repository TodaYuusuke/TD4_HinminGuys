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

	guideUI_.resize(7);
	//攻撃
	guideUI_[0].sprite.LoadTexture("UI/ButtonUI/ButtonUI_Attack.png");
	// 走り(回避)
	guideUI_[1].sprite.LoadTexture("UI/ButtonUI/ButtonUI_Run.png");
	// 
	guideUI_[2].sprite.LoadTexture("UI/ButtonUI/LRButton1.png");
	guideUI_[2].sprite.isActive = false;
	// ロックオン
	guideUI_[3].sprite.LoadTexture("UI/ButtonUI/UI_LTrigger.png");
	// パリィ
	guideUI_[4].sprite.LoadTexture("UI/ButtonUI/ButtonUI_Parry.png");
	// 移動
	guideUI_[5].sprite.LoadTexture("UI/ButtonUI/UI_Move.png");
	// 鞘
	guideUI_[6].sprite.LoadTexture("UI/ButtonUI/ButtonUI_Throw.png");
	for (int i = 0; i < guideUI_.size(); i++) {
		guideUI_[i].sprite.anchorPoint = { 0.5f, 0.0f };
	}

	// クールタイムのフィルタ
	for (int i = 0; i < guideUI_.size(); i++) {
		guideUI_[i].coolTimeFilter.LoadTexture("UI/ButtonUI/ButtonUI_Shadow.png");
		guideUI_[i].coolTimeFilter.anchorPoint = { 0.5f,0.0f };
		guideUI_[i].coolTimeFilter.isActive = false;
		guideUI_[i].coolTimeFilter.worldTF.Parent(&guideUI_[i].sprite.worldTF);

		guideUI_[i].coolTimeFilterSplitSize = guideUI_[i].coolTimeFilter.material.texture.t.GetSize();
		guideUI_[i].coolTimeFilter.clipRect.max = guideUI_[i].coolTimeFilterSplitSize;
	}

	// jsonの値を保存
	json_.Init("UI_Config");
	json_.BeginGroup("Button")

		.BeginGroup("Attack")
		.BeginGroup("CoolTimeFilter")
		.AddValue<Vector3>("Translation", &guideUI_[0].coolTimeFilter.worldTF.translation)
		.EndGroup()
		.AddValue<Vector3>("Translation", &guideUI_[0].sprite.worldTF.translation)
		.EndGroup()
		.BeginGroup("Evasion")
		.BeginGroup("CoolTimeFilter")
		.AddValue<Vector3>("Translation", &guideUI_[1].coolTimeFilter.worldTF.translation)
		.EndGroup()
		.AddValue<Vector3>("Translation", &guideUI_[1].sprite.worldTF.translation)
		.EndGroup()
		.BeginGroup("Dash")
		.BeginGroup("CoolTimeFilter")
		.AddValue<Vector3>("Translation", &guideUI_[2].coolTimeFilter.worldTF.translation)
		.EndGroup()
		.AddValue<Vector3>("Translation", &guideUI_[2].sprite.worldTF.translation)
		.EndGroup()
		.BeginGroup("LockOn")
		.BeginGroup("CoolTimeFilter")
		.AddValue<Vector3>("Translation", &guideUI_[3].coolTimeFilter.worldTF.translation)
		.EndGroup()
		.AddValue<Vector3>("Translation", &guideUI_[3].sprite.worldTF.translation)
		.EndGroup()
		.BeginGroup("Parry")
		.BeginGroup("CoolTimeFilter")
		.AddValue<Vector3>("Translation", &guideUI_[4].coolTimeFilter.worldTF.translation)
		.EndGroup()
		.AddValue<Vector3>("Translation", &guideUI_[4].sprite.worldTF.translation)
		.EndGroup()
		.BeginGroup("Move")
		.BeginGroup("CoolTimeFilter")
		.AddValue<Vector3>("Translation", &guideUI_[5].coolTimeFilter.worldTF.translation)
		.EndGroup()
		.AddValue<Vector3>("Translation", &guideUI_[5].sprite.worldTF.translation)
		.EndGroup()
		.BeginGroup("Sheath")
		.BeginGroup("CoolTimeFilter")
		.AddValue<Vector3>("Translation", &guideUI_[6].coolTimeFilter.worldTF.translation)
		.EndGroup()
		.AddValue<Vector3>("Translation", &guideUI_[6].sprite.worldTF.translation)
		.EndGroup()

		.EndGroup()
		.CheckJsonFile();

	// 座標適用
	guideUI_[2].sprite.worldTF = guideUI_[1].sprite.worldTF;
}

void UIManager::Initialize() {

}

void UIManager::Update() {
	// 回避中はダッシュUIに変更
	//if (player_->GetSystemManager()->GetIsEnableDash()) {
	//	guideUI_[1].isActive = false;
		//guideUI_[2].isActive = true;
	//}
	//else {
		//guideUI_[2].isActive = false;
		//guideUI_[1].isActive = true;
	//}

	// クールタイムのフィルタ
	CoolTimeFilterUpdate();

	for (auto it = afterimageUI_.begin(); it != afterimageUI_.end(); ) {
		EchoMotion* afterUI = *it;
		if (!afterUI->GetIsActive()) {
			delete afterUI;
			it = afterimageUI_.erase(it); // eraseの戻り値で次に進む
		}
		else {
			afterUI->Update();
			++it;
		}
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

void UIManager::CoolTimeFilterUpdate() {
	// 攻撃
	if (player_->GetSystemManager()->GetComboTree()->GetStifnessProgress() > 0.0f) {
		guideUI_[0].coolTimeFilter.isActive = true;
		guideUI_[0].coolTimeFilterSplitSize.y = 150.0f * (1.0f - player_->GetSystemManager()->GetComboTree()->GetStifnessProgress());
		guideUI_[0].coolTimeFilter.clipRect.max.y = guideUI_[0].coolTimeFilterSplitSize.y;

		// 残像UI表示可能
		guideUI_[0].isActive = true;
	}
	else {
		guideUI_[0].isActive = false;
	}

	// 鞘
	if (!player_->GetSystemManager()->GetCoolTimer()->GetSheathCoolTimeData().isFinish) {
		guideUI_[6].coolTimeFilter.isActive = true;
		guideUI_[6].coolTimeFilterSplitSize.y = 150.0f * player_->GetSystemManager()->GetCoolTimer()->GetSheathCoolTimeData().coolTime / player_->GetSystemManager()->GetCoolTimer()->GetSheathCoolTimeData().maxCoolTime;
		guideUI_[6].coolTimeFilter.clipRect.max.y = guideUI_[6].coolTimeFilterSplitSize.y;

		// 残像UI表示可能
		guideUI_[6].isActive = true;
	}
	else {
		guideUI_[6].isActive = false;
	}

	// 残像生成
	for (GuideUI& ui : guideUI_) {
		if (ui.isPreActive && !ui.isActive) {
			EchoMotion* afterUI = new EchoMotion(ui.sprite.material.texture);
			afterUI->SetSpriteData(ui.sprite);
			// アンカーポイント分ずらす
			Vector3 offset = ui.sprite.worldTF.translation;
			offset.y += ui.sprite.material.texture.t.GetSize().y / 2.0f;
			afterUI->SetPos(offset);
			afterUI->Initialize();
			afterimageUI_.push_back(afterUI);
		}

		ui.isPreActive = ui.isActive;
	}
}