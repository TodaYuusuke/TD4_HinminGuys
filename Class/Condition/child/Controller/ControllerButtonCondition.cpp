#include "ControllerButtonCondition.h"

using namespace LWP::Input;
using namespace LWP::Utility;

void LWP::Utility::ControllerButtonCondition::Update()
{
	// 入力条件が長押しで無ければこの後の処理は行わない
	if (inputState_ != Condition::PRESS) { return; }

	// もし指定されたボタンがトリガーされている場合
	if (Controller::GetTrigger(kID_)) {
		// 必要秒数でタイマーを開始する
		pressTimer_.Start(pressTime_);
	}

	// もし指定されたボタンが押されている場合
	if (Controller::GetPress(kID_)) {
		// その間タイマーを更新し続ける
		pressTimer_.Update();
	}
	else { // 押されていない場合
		// タイマーを非アクティブ状態に
		pressTimer_.SetIsActive(false);
	}
}

void LWP::Utility::ControllerButtonCondition::DebugGUI()
{
	ImGui::SeparatorText("ButtonCondition");

	// 現状の結果表示
	bool Result = CheckCondition();
	ImGui::Checkbox("Result", &Result);

	// 入力条件の設定
	if (ImGui::TreeNodeEx("Input State Setting", ImGuiTreeNodeFlags_DefaultOpen)) {
		InputStateSettings();
		ImGui::TreePop();
	}

	// ボタン条件の設定
	if (ImGui::TreeNodeEx("Button Setting", ImGuiTreeNodeFlags_DefaultOpen)) {
		ButtonSettings();
		ImGui::TreePop();
	}
}

bool LWP::Utility::ControllerButtonCondition::CheckCondition()
{
	// 結果の返還用
	bool result = false;
	
	// 入力条件によって処理を変更する
	switch (inputState_)
	{
	// 押した瞬間
	case Condition::TRIGGER: result = Controller::GetTrigger(kID_); break;
	// 押され続けているか
	case Condition::PRESS: result = pressTimer_.GetIsActive() && pressTimer_.GetIsFinish(); break;
	// 離した瞬間
	case Condition::RELEASE: result = Controller::GetRelease(kID_); break;
	default: break;
	}

	// 検証結果を返還する
	return result;
}

void LWP::Utility::ControllerButtonCondition::InputStateSettings()
{
	// 入力条件の表示、および切り替え
	std::string name = "Now State : ";
	// 入力条件によってテキストの内容を変更する
	switch (inputState_)
	{
	case Condition::TRIGGER: name += "TRIGGER"; break;
	case Condition::PRESS: name += "PRESS"; break;
	case Condition::RELEASE: name += "RELEASE"; break;
	}
	// 現在の状態をテキストで表示
	ImGui::Text(name.c_str());

	ImGui::SameLine();

	// 入力条件変更メニュー
	if (ImGui::BeginMenu("Change Input")) {
		ImGui::RadioButton("Trigger", &inputState_, Condition::TRIGGER);
		ImGui::RadioButton("Press", &inputState_, Condition::PRESS);
		ImGui::RadioButton("Release", &inputState_, Condition::RELEASE);
		ImGui::EndMenu();
	}

	// 長押しの状態になっている場合
	if (inputState_ == Condition::PRESS) {
		ImGui::DragFloat("Press Time", &pressTime_, 0.01f, 0.0f);
	}

	ImGui::NewLine();
}

void LWP::Utility::ControllerButtonCondition::ButtonSettings()
{
	// 入力条件の表示、および切り替え
	std::string name = "Button : ";
	// 入力条件によってテキストの内容を変更する
	switch (kID_)
	{
	case Condition::Controller::A: name += "A"; break;
	case Condition::Controller::B: name += "B"; break;
	case Condition::Controller::X: name += "X"; break;
	case Condition::Controller::Y: name += "Y"; break;
	case Condition::Controller::RB: name += "RB"; break;
	case Condition::Controller::LB: name += "LB"; break;
	case Condition::Controller::DPAD_UP: name += "DPAD_UP"; break;
	case Condition::Controller::DPAD_LEFT: name += "DPAD_LEFT"; break;
	case Condition::Controller::DPAD_DOWN: name += "DPAD_DOWN"; break;
	case Condition::Controller::DPAD_RIGHT: name += "DPAD_RIGHT"; break;
	}
	// 現在のボタンをテキストで表示
	ImGui::Text(name.c_str());

	ImGui::SameLine();

	// ボタン変更メニュー
	if (ImGui::BeginMenu("Change Button")) {
		ImGui::RadioButton("A", &kID_, Condition::Controller::A);
		ImGui::RadioButton("B", &kID_, Condition::Controller::B);
		ImGui::RadioButton("X", &kID_, Condition::Controller::X);
		ImGui::RadioButton("Y", &kID_, Condition::Controller::Y);
		ImGui::RadioButton("RB", &kID_, Condition::Controller::RB);
		ImGui::RadioButton("LB", &kID_, Condition::Controller::LB);
		ImGui::RadioButton("DPAD_UP", &kID_, Condition::Controller::DPAD_UP);
		ImGui::RadioButton("DPAD_LEFT", &kID_, Condition::Controller::DPAD_LEFT);
		ImGui::RadioButton("DPAD_DOWN", &kID_, Condition::Controller::DPAD_DOWN);
		ImGui::RadioButton("DPAD_RIGHT", &kID_, Condition::Controller::DPAD_RIGHT);
		ImGui::EndMenu();
	}

	ImGui::NewLine();
}
