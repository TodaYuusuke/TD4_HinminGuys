#include "EventOrder.h"

void EventOrder::Initialize() {
	currentTime_ = 0;
	isActive_ = false;
}

void EventOrder::Update() {
	// 機能を使えないなら早期リターン
	if (!isActive_) { return; }

	// アクションイベントの猶予時間を過ぎていたら先頭のイベント削除
	if (currentTime_ >= timeEvents_[0].graceTime) {
		timeEvents_.erase(timeEvents_.begin());
		currentTime_ = 0;
	}

	// 時間を進める
	currentTime_++;

	// 全てのアクションイベントが終了しているなら機能停止
	if (timeEvents_.empty()) {
		Reset();
	}
}

void EventOrder::Start() {
	// 何もリストに格納されていないとき
	if (timeEvents_.empty()) {
		// 情報を作成
		timeEvents_ = timeEventSchedules_;
		isActive_ = true;
	}
}

void EventOrder::Reset() {
	timeEvents_.clear();
	currentTime_ = 0;
	isActive_ = false;
}

void EventOrder::CreateTimeEvent(TimeEvent timeEvent) {
	// アクションイベントの予定を格納
	timeEventSchedules_.push_back(timeEvent);
}

void EventOrder::DebugGUI() {
	if (ImGui::TreeNode("Event")) {
		// イベントの進む順序
		if (ImGui::TreeNode("ExecutionOrder")) {
			int i = 0;
			for (TimeEvent& timeEvent : timeEventSchedules_) {
				i++;
				std::string timeEventName = std::to_string(i) + " : % s";
				std::string timeEventGraceTime = "##TimeEvent" + std::to_string(i);
				ImGui::Text(timeEventName.c_str(), timeEvent.name.c_str());
				ImGui::DragFloat((std::string("GraceTime") + timeEventGraceTime).c_str(), &timeEvent.graceTime, 1, 0, 10000);
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
		// 経過時間
		ImGui::DragFloat("CurrentTime", &currentTime_, 1, 0, 1000);
		// 機能を使用中か
		ImGui::Checkbox("IsActive", &isActive_);

		ImGui::TreePop();
	}

}