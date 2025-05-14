#include "Move.h"
#include "../../Player.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;

Move::Move(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;
}

void Move::Initialize() {
	// 移動速度
	moveVel_ = { 0.0f, 0.0f, 0.0f };
	// 向いている角度
	quat_ = { 0.0f,0.0f,0.0f,1.0f };
	radian_ = { 0.0f, 0.0f, 0.0f };

	json_.Init("MoveData.json");
	json_.AddValue<float>("MoveMultiply", &moveMultiply)
		.CheckJsonFile();
}

void Move::Update() {
	// 機能を使えないなら早期リターン
	if (!isActive_) { return; }

	// 入力処理
	InputUpdate();

	isPreActive_ = isActive_;
}

void Move::Reset() {
	// 移動速度
	moveVel_ = { 0.0f, 0.0f, 0.0f };
	// 向いている角度
	quat_ = { 0.0f,0.0f,0.0f,1.0f };
	radian_ = { 0.0f, 0.0f, 0.0f };
	// アニメーションを初期化
	player_->ResetAnimation();
	isMove_ = false;
}

void Move::DebugGUI() {
	if (ImGui::TreeNode("Move")) {
		if (ImGui::TreeNode("Json")) {
			json_.DebugGUI();
			ImGui::TreePop();
		}

		ImGui::DragFloat3("Velocity", &moveVel_.x, 0.1f, -10000, 10000);
		ImGui::DragFloat3("Rotation", &radian_.x, 0.1f, -6.28f, 6.28f);
		ImGui::DragFloat4("Quaternion", &quat_.x, 0.1f, -1, 1);
		ImGui::TreePop();
	}
}

void Move::InputUpdate() {
	// 方向を取得
	LWP::Math::Vector3 dir{ 0.0f, 0.0f, 0.0f };

#pragma region ゲームパッド
	// y軸方向の移動をしないようにする
	LWP::Math::Vector3 stickMovement = { LWP::Input::Controller::GetLStick().x, 0, LWP::Input::Controller::GetLStick().y };
	dir = stickMovement;
#pragma endregion

#pragma region キーボード
	if (lwp::Keyboard::GetPress(DIK_W)) {
		dir.z = 1.0f;
	}
	if (lwp::Keyboard::GetPress(DIK_S)) {
		dir.z = -1.0f;
	}
	if (lwp::Keyboard::GetPress(DIK_A)) {
		dir.x = -1.0f;
	}
	if (lwp::Keyboard::GetPress(DIK_D)) {
		dir.x = 1.0f;
	}
#pragma endregion
	// カメラが向いている方向に進む
	// 回転行列を求める
	Matrix4x4 rotMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(pCamera_->worldTF.rotation);
	// 方向ベクトルを求める
	moveVel_ = dir * rotMatrix * moveMultiply;
	moveVel_.y = 0;

	isMove_ = false;
	// 移動ベクトルから体の向きを算出(入力があるときのみ処理する)
	if (LWP::Math::Vector3::Dot(Abs(dir), LWP::Math::Vector3{ 1,1,1 }) != 0) {
		// 移動速度からラジアンを求める
		radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, moveVel_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
		quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
		isMove_ = true;
	}
}