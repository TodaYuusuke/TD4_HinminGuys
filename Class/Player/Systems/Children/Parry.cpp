#include "Parry.h"
#include "../../Player.h"

Parry::Parry(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;
	debugTimeEvents_ = CreateParryData();
}

void Parry::Initialize() {
	currentTime_ = 0;
	isActive_ = false;
}

void Parry::Update() {
	// 入力処理
	InputUpdate();

	// パリィ機能を使えないなら早期リターン
	if (!isActive_) { return; }

	// イベントの猶予時間を過ぎていたら先頭のイベント削除
	if (currentTime_ >= timeEvents_[0].graceTime) {
		timeEvents_.erase(timeEvents_.begin());
		currentTime_ = 0;
	}

	// 時間を進める
	currentTime_++;

	// 全てのイベントが終了しているなら機能停止
	if (timeEvents_.empty()) {
		Reset();
	}
}

void Parry::Reset() {
	currentTime_ = 0;
	isActive_ = false;
}

void Parry::DebugGUI() {
	if (ImGui::TreeNode("Parry")) {
		if (ImGui::TreeNode("Event")) {
			// イベントの進む順序
			if (ImGui::TreeNode("ExecutionOrder")) {
				int i = 0;
				for (TimeEvent& timeEvent : debugTimeEvents_) {
					i++;
					std::string timeEventName = std::to_string(i) + " : % s";
					ImGui::Text(timeEventName.c_str(), timeEvent.name.c_str());
					ImGui::Text("GraceTime : %f", timeEvent.graceTime);
				}
				ImGui::TreePop();
			}
			// 現在のイベントと猶予時間
			if (ImGui::TreeNode("CurrentData")) {
				if (!timeEvents_.empty()) {
					ImGui::Text("Event : %s", timeEvents_[0].name.c_str());
					ImGui::DragFloat("GraceTime", &timeEvents_[0].graceTime, 1, 0, 1000);
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		// 経過時間
		ImGui::DragFloat("CurrentTime", &currentTime_, 1, 0, 1000);
		// パリィ中か
		ImGui::Checkbox("IsParry", &isActive_);

		ImGui::TreePop();
	}
}

void Parry::InputUpdate() {
	if (LWP::Input::Keyboard::GetPress(DIK_SPACE) || LWP::Input::Pad::GetPress(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		// パリィボタンを押していないときのみ
		if (timeEvents_.empty()) {
			// パリィの情報を作成
			timeEvents_ = CreateParryData();
			isActive_ = true;
		}
	}
}

std::vector<TimeEvent> Parry::CreateParryData() {
	std::vector<TimeEvent> timeEvents;
	// パリィ発生までの時間
	timeEvents.push_back(TimeEvent{ kSwingTime, "SwingTime" });
	// ジャストパリィの猶予時間
	timeEvents.push_back(TimeEvent{ kJustParryTime, "JustParry" });
	// 通常パリィの猶予時間
	timeEvents.push_back(TimeEvent{ kGoodParryTime , "GoodParry" });
	// パリィの硬直時間
	timeEvents.push_back(TimeEvent{ kRecoveryTime , "RecoveryTime" });

	return timeEvents;
}