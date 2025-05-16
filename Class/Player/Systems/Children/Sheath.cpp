#include "Sheath.h"
#include "State/Sheath/Throw.h"
#include "../../Player.h"

Sheath::Sheath(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;

	sheathModel_.LoadCube();
	sheathModel_.worldTF.scale = { 0.25f, 1.0f, 0.25f };
	sheathModel_.isActive = true;

	// 状態の生成
	state_ = new Throw(this, player_);
	state_->Initialize();
}

void Sheath::Initialize() {
	// コマンドの登録
	inputHandler_ = InputHandler::GetInstance();
	inputHandler_->GetA();
}

void Sheath::Update() {
	if (!isActive_) { return; }

	// 状態
	state_->Update();

	isPreActive_ = isActive_;
}

void Sheath::Reset() {
	isActive_ = false;
	isPreActive_ = false;
	// 移動速度
	velocity_ = { 0.0f,0.0f,0.0f };
	// 向いている角度
	quat_ = { 0.0f,0.0f,0.0f,1.0f };
	radian_ = { 0.0f,0.0f,0.0f };
}

void Sheath::DebugGUI() {
	if (ImGui::TreeNode("Sheath")) {
		ImGui::DragFloat3("Velocity", &velocity_.x);
		ImGui::DragFloat3("Radian", &radian_.x);

		ImGui::Checkbox("IsEvasion", &isActive_);

		ImGui::TreePop();
	}
}

void Sheath::Command() {
	player_->GetSystemManager()->SetInputState(InputState::kSheath);
	// 状態によって変更
	state_->Command();
}

void Sheath::AnimCommand() {
	// 状態によって変更
	state_->AnimCommand();
}

void Sheath::ChangeState(ISheathSystemState* pState) {
	delete state_;
	state_ = pState;
}
