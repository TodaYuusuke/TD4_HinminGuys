#include "ButtonCondition.h"

using namespace LWP::Input;
using namespace LWP::Utility;

void LWP::Utility::ButtonCondition::Update()
{
	// 入力条件が長押しで無ければこの後の処理は行わない
	if (inputState_ != Condition::PRESS) { return; }

	// もし指定されたボタン、もしくはキーがトリガーされている場合
	if (Controller::GetTrigger(buttonID_) || Keyboard::GetTrigger(keyID_)) {
		// タイマーがアクティブ状態であれば早期リターン
		if (pressTimer_.GetIsActive()) { return; }
		
		// 必要秒数でタイマーを開始する
		pressTimer_.Start(pressTime_);
	}

	// もし指定されたボタンが押されている場合
	if (Controller::GetPress(buttonID_) || Keyboard::GetPress(keyID_)) {
		// その間タイマーを更新し続ける
		pressTimer_.Update();
	}
	else { // 押されていない場合
		// タイマーを非アクティブ状態に
		pressTimer_.SetIsActive(false);
	}
}

void LWP::Utility::ButtonCondition::DebugGUI()
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

bool LWP::Utility::ButtonCondition::CheckCondition()
{
	// 結果の返還用
	bool result = false;
	
	// 入力条件によって処理を変更する
	switch (inputState_)
	{
	// 押した瞬間
	case Condition::TRIGGER: result = Controller::GetTrigger(buttonID_) || Keyboard::GetTrigger(keyID_); break;
	// 押され続けているか
	case Condition::PRESS: result = pressTimer_.GetIsActive() && pressTimer_.GetIsFinish(); break;
	// 離した瞬間
	case Condition::RELEASE: result = Controller::GetRelease(buttonID_) || Keyboard::GetRelease(keyID_); break;
	default: break;
	}

	// 検証結果を返還する
	return result;
}

void LWP::Utility::ButtonCondition::InputStateSettings()
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

void LWP::Utility::ButtonCondition::ButtonSettings()
{
	// 入力条件の表示、および切り替え
	std::string buttonName = "Button : ";
	// 入力条件によってテキストの内容を変更する
	switch (buttonID_)
	{
	case Condition::Controller::A: buttonName += "A"; break;
	case Condition::Controller::B: buttonName += "B"; break;
	case Condition::Controller::X: buttonName += "X"; break;
	case Condition::Controller::Y: buttonName += "Y"; break;
	case Condition::Controller::RB: buttonName += "RB"; break;
	case Condition::Controller::LB: buttonName += "LB"; break;
	case Condition::Controller::DPAD_UP: buttonName += "DPAD_UP"; break;
	case Condition::Controller::DPAD_LEFT: buttonName += "DPAD_LEFT"; break;
	case Condition::Controller::DPAD_DOWN: buttonName += "DPAD_DOWN"; break;
	case Condition::Controller::DPAD_RIGHT: buttonName += "DPAD_RIGHT"; break;
	}
	// 現在のボタンをテキストで表示
	ImGui::Text(buttonName.c_str());

	ImGui::SameLine();

	// ボタン変更メニュー
	if (ImGui::BeginMenu("Change Button")) {
		ImGui::RadioButton("A", &buttonID_, Condition::Controller::A);
		ImGui::RadioButton("B", &buttonID_, Condition::Controller::B);
		ImGui::RadioButton("X", &buttonID_, Condition::Controller::X);
		ImGui::RadioButton("Y", &buttonID_, Condition::Controller::Y);
		ImGui::RadioButton("RB", &buttonID_, Condition::Controller::RB);
		ImGui::RadioButton("LB", &buttonID_, Condition::Controller::LB);
		ImGui::RadioButton("DPAD_UP", &buttonID_, Condition::Controller::DPAD_UP);
		ImGui::RadioButton("DPAD_LEFT", &buttonID_, Condition::Controller::DPAD_LEFT);
		ImGui::RadioButton("DPAD_DOWN", &buttonID_, Condition::Controller::DPAD_DOWN);
		ImGui::RadioButton("DPAD_RIGHT", &buttonID_, Condition::Controller::DPAD_RIGHT);
		ImGui::EndMenu();
	}

	// 入力条件の表示、および切り替え
	std::string KeyName = "KeyName : ";
	// 入力条件によってテキストの内容を変更する
	switch (keyID_)
	{
	case Condition::KeyBoard::W: KeyName += "W"; break;
	case Condition::KeyBoard::A: KeyName += "A"; break;
	case Condition::KeyBoard::S: KeyName += "S"; break;
	case Condition::KeyBoard::D: KeyName += "D"; break;
	case Condition::KeyBoard::E: KeyName += "E"; break;
	case Condition::KeyBoard::Q: KeyName += "Q"; break;
	case Condition::KeyBoard::F: KeyName += "F"; break;
	case Condition::KeyBoard::J: KeyName += "J"; break;
	case Condition::KeyBoard::K: KeyName += "K"; break;
	case Condition::KeyBoard::L: KeyName += "L"; break;
	case Condition::KeyBoard::UP: KeyName += "UP"; break;
	case Condition::KeyBoard::LEFT: KeyName += "LEFT"; break;
	case Condition::KeyBoard::DOWN: KeyName += "DOWN"; break;
	case Condition::KeyBoard::RIGHT: KeyName += "RIGHT"; break;
	case Condition::KeyBoard::SHIFT: KeyName += "SHIFT"; break;
	case Condition::KeyBoard::SPACE: KeyName += "SPACE"; break;
	}
	// 現在のボタンをテキストで表示
	ImGui::Text(KeyName.c_str());

	ImGui::SameLine();

	// キー変更メニュー
	if (ImGui::BeginMenu("Change Key")) {
		ImGui::RadioButton("W", &keyID_, Condition::KeyBoard::W);
		ImGui::RadioButton("A", &keyID_, Condition::KeyBoard::A);
		ImGui::RadioButton("S", &keyID_, Condition::KeyBoard::S);
		ImGui::RadioButton("D", &keyID_, Condition::KeyBoard::D);
		ImGui::RadioButton("E", &keyID_, Condition::KeyBoard::E);
		ImGui::RadioButton("Q", &keyID_, Condition::KeyBoard::Q);
		ImGui::RadioButton("F", &keyID_, Condition::KeyBoard::F);
		ImGui::RadioButton("J", &keyID_, Condition::KeyBoard::J);
		ImGui::RadioButton("K", &keyID_, Condition::KeyBoard::K);
		ImGui::RadioButton("L", &keyID_, Condition::KeyBoard::L);
		ImGui::RadioButton("UP", &keyID_, Condition::KeyBoard::UP);
		ImGui::RadioButton("LEFT", &keyID_, Condition::KeyBoard::LEFT);
		ImGui::RadioButton("DOWN", &keyID_, Condition::KeyBoard::DOWN);
		ImGui::RadioButton("RIGHT", &keyID_, Condition::KeyBoard::RIGHT);
		ImGui::RadioButton("SHIFT", &keyID_, Condition::KeyBoard::SHIFT);
		ImGui::RadioButton("SPACE", &keyID_, Condition::KeyBoard::SPACE);
		ImGui::EndMenu();
	}

	ImGui::NewLine();
}
