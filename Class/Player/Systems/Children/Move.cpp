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
}

void Move::DebugGUI() {
	if (ImGui::TreeNode("Move")) {
		;
		//ImGui::Checkbox("IsNormalAttack", &isNormalAttack_);
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
	moveVel_ = dir * rotMatrix;
	moveVel_.y = 0;

	// 移動ベクトルから体の向きを算出(入力があるときのみ処理する)
	if (LWP::Math::Vector3::Dot(Abs(dir), LWP::Math::Vector3{ 1,1,1 }) != 0) {
		// 移動速度からラジアンを求める
		radian_.y = GetAngle(LWP::Math::Vector3{ 0,0,1 }, moveVel_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
		quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
	}
}

float Move::GetAngle(const LWP::Math::Vector3& a, const LWP::Math::Vector3& b, const LWP::Math::Vector3& up) {
	Vector3 aN = a.Normalize();
	Vector3 bN = b.Normalize();

	float dot = LWP::Math::Vector3::Dot(aN, bN);
	dot = std::fmax(-1.0f, std::fmin(1.0f, dot)); // 数値誤差対策

	float angle = std::acos(dot); // 0〜π

	Vector3 cross = LWP::Math::Vector3::Cross(aN, bN);

	if (LWP::Math::Vector3::Dot(up, cross) < 0) {
		angle = 2 * (float)M_PI - angle;
	}

	return angle;
}