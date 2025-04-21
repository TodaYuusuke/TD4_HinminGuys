#include "MoveSystem.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;

void MoveSystem::Initialize() {

}

void MoveSystem::Update() {
	// 機能を使えないなら早期リターン
	if (!isActive_) { return; }

	// 入力処理
	InputUpdate();
}

void MoveSystem::Reset() {
	// 移動速度
	moveVel_ = { 0.0f, 0.0f, 0.0f };
	// 向いている角度
	rotate_ = { 0.0f,0.0f,0.0f,1.0f };
}

void MoveSystem::InputUpdate() {
	// 方向を取得
	LWP::Math::Vector3 direct{ 0.0f, 0.0f, 0.0f };

#pragma region キーボード
	if (lwp::Keyboard::GetPress(DIK_W)) {
		direct.z += 1.0f;
	}
	else if (lwp::Keyboard::GetPress(DIK_S)) {
		direct.z -= 1.0f;
	}
	if (lwp::Keyboard::GetPress(DIK_A)) {
		direct.x -= 1.0f;
	}
	else if (lwp::Keyboard::GetPress(DIK_D)) {
		direct.x += 1.0f;
	}
#pragma endregion

#pragma region ゲームパッド
	// y軸方向の移動をしないようにする
	//LWP::Math::Vector3 stickMovement = { LWP::Input::Controller::GetLStick().x, 0, LWP::Input::Controller::GetLStick().y };
	//direct = ExponentialInterpolate(moveVel_, stickMovement, 1.0f) * 0.1f;
#pragma endregion
	// 移動速度を反映
	moveVel_ = direct;

	// 移動速度から角度を求める
	lwp::Vector3 cross = lwp::Vector3::Cross({ 0.0f,1.0f,0.0f }, moveVel_.Normalize()).Normalize();
	float dot = lwp::Vector3::Dot({ 0.0f,1.0f,0.0f }, moveVel_);
	//行きたい方向のQuaternionの作成
	rotate_ = lwp::Quaternion::CreateFromAxisAngle(cross, std::acos(dot));

	ImGui::Begin("MoveSystem");
	ImGui::DragFloat4("rotate", &rotate_.x, 0.0f);
	ImGui::End();
}

LWP::Math::Quaternion MoveSystem::ExponentialInterpolate(const LWP::Math::Quaternion& current, const LWP::Math::Quaternion& target, float damping) {
	float factor = 1.0f - std::exp(-damping);
	LWP::Math::Quaternion dist = {
		target.x - current.x,
		target.y - current.y,
		target.z - current.z,
		target.w - current.w
	};
	return current + dist * factor;
}

LWP::Math::Vector3 MoveSystem::ExponentialInterpolate(const LWP::Math::Vector3& current, const LWP::Math::Vector3& target, float damping) {
	float factor = 1.0f - std::exp(-damping);
	return current + (target - current) * factor;
}

float MoveSystem::ExponentialInterpolate(const float& current, const float& target, float damping) {
	float factor = 1.0f - std::exp(-damping);
	return current + (target - current) * factor;
}
