#include "Move.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;

Move::Move(LWP::Object::Camera* camera) {
	pCamera_ = camera;
}

void Move::Initialize() {

}

void Move::Update() {
	// 機能を使えないなら早期リターン
	if (!isActive_) { return; }

	// 入力処理
	InputUpdate();
}

void Move::Reset() {
	// 移動速度
	moveVel_ = { 0.0f, 0.0f, 0.0f };
	// 向いている角度
	rotate_ = { 0.0f,0.0f,0.0f,1.0f };
}

void Move::InputUpdate() {
	// 方向を取得
	LWP::Math::Vector3 dir{ 0.0f, 0.0f, 0.0f };

#pragma region キーボード
	if (lwp::Keyboard::GetPress(DIK_W)) {
		dir.z += 1.0f;
	}
	else if (lwp::Keyboard::GetPress(DIK_S)) {
		dir.z -= 1.0f;
	}
	if (lwp::Keyboard::GetPress(DIK_A)) {
		dir.x -= 1.0f;
	}
	else if (lwp::Keyboard::GetPress(DIK_D)) {
		dir.x += 1.0f;
	}
#pragma endregion

#pragma region ゲームパッド
	// y軸方向の移動をしないようにする
	LWP::Math::Vector3 stickMovement = { LWP::Input::Controller::GetLStick().x, 0, LWP::Input::Controller::GetLStick().y };
	dir += ExponentialInterpolate(moveVel_, stickMovement, 1.0f) * 0.1f;
#pragma endregion

	// カメラが向いている方向に進む
	// 回転行列を求める
	Matrix4x4 rotMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(pCamera_->worldTF.rotation);
	// 方向ベクトルを求める
	moveVel_ = dir * rotMatrix;
	moveVel_.y = 0;

	// 移動速度からラジアンを求める(入力があるときのみ)
	if (LWP::Math::Vector3::Dot(dir, LWP::Math::Vector3{ 1,1,1 }) != 0) {
		float y = GetAngle(LWP::Math::Vector3{ 0,0,1 }, moveVel_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
		rotate_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, y);
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
		angle = 2 * M_PI - angle;
	}

	return angle;
}

LWP::Math::Quaternion Move::ExponentialInterpolate(const LWP::Math::Quaternion& current, const LWP::Math::Quaternion& target, float damping) {
	float factor = 1.0f - std::exp(-damping);
	LWP::Math::Quaternion dist = {
		target.x - current.x,
		target.y - current.y,
		target.z - current.z,
		target.w - current.w
	};
	return current + dist * factor;
}

LWP::Math::Vector3 Move::ExponentialInterpolate(const LWP::Math::Vector3& current, const LWP::Math::Vector3& target, float damping) {
	float factor = 1.0f - std::exp(-damping);
	return current + (target - current) * factor;
}

float Move::ExponentialInterpolate(const float& current, const float& target, float damping) {
	float factor = 1.0f - std::exp(-damping);
	return current + (target - current) * factor;
}
