#include "Hit.h"

Hit::Hit(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;
}

void Hit::Initialize() {
	// HPバー
	//hp_.Initialize();

	// アクションイベントの作成
	CreateEventOrders();

	// jsonに保存している値を呼び出す
	CreateJsonFIle();
}

void Hit::Update() {
	// HPバー
	//hp_.Update();
	// 無敵時間の更新
	eventOrders_[(int)EventOrderState::kInvinsible].Update();

	// 無敵時間終了
	if (eventOrders_[(int)EventOrderState::kInvinsible].GetIsEnd()) {
		eventOrders_[(int)EventOrderState::kInvinsible].Reset();
	}
}

void Hit::Reset() {
	eventOrders_[(int)EventOrderState::kInvinsible].Reset();
}

void Hit::DebugGUI() {
	if (ImGui::TreeNode("HP")) {
		//hp_.DebugGUI();

		if (ImGui::TreeNode("Hit")) {
			// 無敵時間のアクションイベントを保存
			if (ImGui::TreeNode("Json")) {
				// アクションイベントを実行してないときのみ変更可能
				if (eventOrders_[(int)EventOrderState::kInvinsible].GetIsEnd()) {
					json_.DebugGUI();
					// アクションイベントを再登録
					eventOrders_[(int)EventOrderState::kInvinsible].Initialize();
					CreateEventOrders();
				}
				else {
					ImGui::Text("Event Running!");
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void Hit::CreateJsonFIle() {
	json_.Init("Hit.json");
	json_.BeginGroup("Inivinsible")
		.AddValue<float>("Time", &invinsibleTime)
		.EndGroup()
		.CheckJsonFile();
}

void Hit::HitUpdate() {
	if (currentFrame_ <= 0) {
		currentFrame_ = endFrame_;
		isHit_ = false;
	}
}

void Hit::CreateEventOrders() {
	eventOrders_[(int)EventOrderState::kInvinsible].Initialize();
	// 被弾時の無敵時間
	eventOrders_[(int)EventOrderState::kInvinsible].CreateTimeEvent(TimeEvent{ invinsibleTime * 60.0f, "InvinsibleTime" });
}
