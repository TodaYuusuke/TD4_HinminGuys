#include "TakeDamage.h"
#include "../../../Player/Player.h"

TakeDamage::TakeDamage(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;
}

void TakeDamage::Initialize() {
	inputHandler_ = InputHandler::GetInstance();

	// jsonに保存している値を呼び出す
	CreateJsonFIle();

	// アクションイベントの作成
	CreateEventOrders();
}

void TakeDamage::Update() {
	// 無敵時間の更新
	eventOrders_[(int)EventOrderState::kInvinsible].Update();

	if (!isActive_) { return; }

	// スタン時間
	eventOrders_[(int)EventOrderState::kStun].Update();

	// スタン状態
	CheckSunEventOrder();

	// 無敵時間終了
	if (eventOrders_[(int)EventOrderState::kInvinsible].GetIsEnd()) {
		eventOrders_[(int)EventOrderState::kInvinsible].Reset();
	}
	// スタン時間終了
	if (eventOrders_[(int)EventOrderState::kStun].GetIsEnd()) {
		eventOrders_[(int)EventOrderState::kStun].Reset();
		isActive_ = false;
		// 回避をしてないなかったら入力をすべて受け付ける状態に戻す
		if (!player_->GetSystemManager()->GetEvasionSystem()->GetIsActive()) {
			inputHandler_->SetCurrentBanInput(0);
		}
	}
}

void TakeDamage::Reset() {

}

void TakeDamage::DebugGUI() {
	if (ImGui::TreeNode("Hit")) {
		// アクションイベントを保存
		if (ImGui::TreeNode("Json")) {
			json_.DebugGUI();
			// 無敵アクションイベントを実行してないときのみ変更可能
			if (eventOrders_[(int)EventOrderState::kInvinsible].GetIsEnd()) {
				// アクションイベントを再登録
				eventOrders_[(int)EventOrderState::kInvinsible].Initialize();
				CreateInvinsibleEventOrder();
			}
			// スタンアクションイベントを実行してないときのみ変更可能
			if (eventOrders_[(int)EventOrderState::kStun].GetIsEnd()) {
				// アクションイベントを再登録
				eventOrders_[(int)EventOrderState::kStun].Initialize();
				CreateStunEventOrder();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Invinsible")) {
			eventOrders_[(int)EventOrderState::kInvinsible].DebugGUI();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Stun")) {
			eventOrders_[(int)EventOrderState::kStun].DebugGUI();
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

void TakeDamage::CreateJsonFIle() {
	json_.Init("Hit.json");
	// 無敵
	json_.BeginGroup("Inivinsible")
		.AddValue<float>("Time", &invinsibleTime)
		.EndGroup()
		// スタン
		.BeginGroup("Stun")
		.AddValue<float>("Time", &stunTime)
		.AddValue<float>("CancelTime", &stunCancelTime)
		.EndGroup()
		.CheckJsonFile();
}

void TakeDamage::StartInvinsible() {
	eventOrders_[(int)EventOrderState::kInvinsible].Start();
	eventOrders_[(int)EventOrderState::kInvinsible].Update();
	eventOrders_[(int)EventOrderState::kStun].Start();
	eventOrders_[(int)EventOrderState::kStun].Update();
	isActive_ = true;
	// 全ての行動をできないようにする
	inputHandler_->SetCurrentBanInput(inputHandler_->GetBanInput() | BanALL);
	// ガードアニメーション開始
	player_->ResetAnimation();
	player_->StartAnimation("Damage", 0.0f, 0.0f);
}

void TakeDamage::HitUpdate() {
	if (currentFrame_ <= 0) {
		currentFrame_ = endFrame_;
		isHit_ = false;
	}
}

void TakeDamage::CreateEventOrders() {
	// 無敵
	CreateInvinsibleEventOrder();
	// スタン
	CreateStunEventOrder();
}

void TakeDamage::CreateInvinsibleEventOrder() {
	// 無敵
	eventOrders_[(int)EventOrderState::kInvinsible].Initialize();
	// 被弾時の無敵時間
	eventOrders_[(int)EventOrderState::kInvinsible].CreateTimeEvent(TimeEvent{ invinsibleTime * 60.0f, "InvinsibleTime" });
}

void TakeDamage::CreateStunEventOrder() {
	// スタン
	eventOrders_[(int)EventOrderState::kStun].Initialize();
	// 被弾時のスタン時間
	eventOrders_[(int)EventOrderState::kStun].CreateTimeEvent(TimeEvent{ stunTime * 60.0f, "StunTime" });
	// 被弾時のスタンのキャンセル可能時間
	eventOrders_[(int)EventOrderState::kStun].CreateTimeEvent(TimeEvent{ stunCancelTime * 60.0f, "CancelTime" });
}

void TakeDamage::CheckSunEventOrder() {
	if (eventOrders_[(int)EventOrderState::kStun].GetCurrentTimeEvent().name == "StunTime") {

	}
	else if (eventOrders_[(int)EventOrderState::kStun].GetCurrentTimeEvent().name == "CancelTime") {
		// ビットの演算が一瞬じゃないと数値が壊れる
		if (preEventOrder_ == "StunTime") {
			// 回避のみ可能にする
			inputHandler_->SetCurrentBanInput(EraceBanInput(inputHandler_->GetBanInput(), (BanEvasion)));
		}
	}

	preEventOrder_ = eventOrders_[(int)EventOrderState::kStun].GetCurrentTimeEvent().name;
}