#include "InputCamera.h"
#include "../FollowCamera.h"

InputCamera::InputCamera(FollowCamera* followCamera) {
	followCamera_ = followCamera;

	stateName_ = "Input";
}

void InputCamera::Initialize() {

}

void InputCamera::Update() {
	// カメラの角度を算出
	RotateUpdate();
}

void InputCamera::RotateUpdate() {
	// 回転する向き
	LWP::Math::Vector2 dir = { 0.0f,0.0f };

	// キーボードでの回転
	if (LWP::Input::Keyboard::GetPress(DIK_UP)) {
		dir.x -= followCamera_->sensitivity;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_DOWN)) {
		dir.x += followCamera_->sensitivity;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_RIGHT)) {
		dir.y += followCamera_->sensitivity;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_LEFT)) {
		dir.y -= followCamera_->sensitivity;
	}

	// コントローラーでの回転
	dir.x -= LWP::Input::Pad::GetRStick().y * followCamera_->sensitivity;
	dir.y += LWP::Input::Pad::GetRStick().x * followCamera_->sensitivity;

	// スティックの入力をイージング
	LWP::Math::Vector3 goal = { dir.x, dir.y, 0 };
	stickDir_ = LWP::Utility::Interpolation::Exponential(stickDir_, goal, followCamera_->rotateRate);

	// 角度制限
	followCamera_->ClampAngle(stickDir_.x, (followCamera_->GetTargetPos() - followCamera_->GetCamera()->worldTF.translation).Normalize(), LWP::Utility::DegreeToRadian(followCamera_->kOriginRotateX + followCamera_->kMinRotateX), LWP::Utility::DegreeToRadian(followCamera_->kOriginRotateX + followCamera_->kMaxRotateX));

	// x軸回転
	followCamera_->SetCameraRotate(followCamera_->GetCamera()->worldTF.rotation * LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, 0.03f * stickDir_.x));
	// y軸は常に上を向くように固定
	followCamera_->SetCameraRotate(LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, 0.03f * stickDir_.y) * followCamera_->GetCamera()->worldTF.rotation);
}