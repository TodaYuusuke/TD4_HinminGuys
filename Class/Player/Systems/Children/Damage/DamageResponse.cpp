#include "DamageResponse.h"
#include "../../../../Player/Player.h"

DamageResponse::DamageResponse(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;
	// ヒットストップの管理クラス
	hitStopController_ = HitStopController::GetInstance();
}

void DamageResponse::Initialize() {
	inputHandler_ = InputHandler::GetInstance();

	// アクションイベントの作成
	CreateEventOrders();
}

void DamageResponse::Update() {
	// 無敵時間の更新
	eventOrders_[(int)EventOrderState::kInvinsible].Update();

	// スタン時間
	eventOrders_[(int)EventOrderState::kStun].Update();

	// スタン状態
	CheckStunEventOrder();

	// 無敵時間終了
	if (eventOrders_[(int)EventOrderState::kInvinsible].GetIsEnd()) {
		eventOrders_[(int)EventOrderState::kInvinsible].Reset();
	}
	// スタン時間終了
	if (eventOrders_[(int)EventOrderState::kStun].GetIsEnd()) {
		eventOrders_[(int)EventOrderState::kStun].Reset();
		isActive_ = false;

		// 入力のあったシステム
		nextSystem_ = CheckNextSystems();
		// 何も入力がなければ移動システムを入れる
		if (nextSystem_.empty()) {
			nextSystem_[SystemState::kMove] = true;
		}
	}
}

void DamageResponse::Reset() {
	//nextSystem_ = CheckNextSystems();
}

void DamageResponse::DebugGUI() {
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

void DamageResponse::CreateJsonFIle() {
	json_.Init("Hit.json");
	// 無敵
	json_.BeginGroup("Inivinsible")
		.AddValue<float>("Time", &jsonData_.invinsibleTime)
		.EndGroup()
		// スタン
		.BeginGroup("Stun")
		.AddValue<float>("Time", &jsonData_.stunTime)
		.AddValue<float>("CancelTime", &jsonData_.stunCancelTime)
		.EndGroup()
		.CheckJsonFile();
}

void DamageResponse::StartInvinsible() {
	// 無敵時間を設定
	player_->GetSystemManager()->SetInvisibleTime(jsonData_.invinsibleTime);

	if (!player_->GetParameter()->GetIsSheathBreak()) {
		isActive_ = true;
		// 無敵タイミング
		eventOrders_[(int)EventOrderState::kInvinsible].Start();
		eventOrders_[(int)EventOrderState::kInvinsible].Update();
		// スタン
		eventOrders_[(int)EventOrderState::kStun].Start();
		eventOrders_[(int)EventOrderState::kStun].Update();
		// ガードアニメーション開始
		player_->ResetAnimation();
		player_->StartAnimation("Damage", 0.15f, 0.0f);
	}
}

void DamageResponse::HitUpdate() {
	if (currentFrame_ <= 0) {
		currentFrame_ = endFrame_;
		isHit_ = false;
	}
}

void DamageResponse::CreateEventOrders() {
	// 無敵
	CreateInvinsibleEventOrder();
	// スタン
	CreateStunEventOrder();
}

void DamageResponse::CreateInvinsibleEventOrder() {
	// 無敵
	eventOrders_[(int)EventOrderState::kInvinsible].Initialize();
	// 被弾時の無敵時間
	eventOrders_[(int)EventOrderState::kInvinsible].CreateTimeEvent(TimeEvent{ jsonData_.invinsibleTime * 60.0f, "InvinsibleTime" });
}

void DamageResponse::CreateStunEventOrder() {
	// スタン
	eventOrders_[(int)EventOrderState::kStun].Initialize();
	// 被弾時のスタン時間
	eventOrders_[(int)EventOrderState::kStun].CreateTimeEvent(TimeEvent{ jsonData_.stunTime * 60.0f, "StunTime" });
	// 被弾時のスタンのキャンセル可能時間
	eventOrders_[(int)EventOrderState::kStun].CreateTimeEvent(TimeEvent{ jsonData_.stunCancelTime * 60.0f, "CancelTime" });
}

void DamageResponse::CheckStunEventOrder() {
	if (eventOrders_[(int)EventOrderState::kStun].GetCurrentTimeEvent().name == "StunTime") {

	}
	else if (eventOrders_[(int)EventOrderState::kStun].GetCurrentTimeEvent().name == "CancelTime") {
		// 回避だけ入力可能
		if (CheckNextSystems()[SystemState::kEvasion]) {
			nextSystem_[SystemState::kEvasion] = true;
		}
	}

	preEventOrder_ = eventOrders_[(int)EventOrderState::kStun].GetCurrentTimeEvent().name;
}

void DamageResponse::CheckInvinsibleEventOrder() {

}