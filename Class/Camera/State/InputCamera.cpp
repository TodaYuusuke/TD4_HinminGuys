#include "InputCamera.h"
#include "../FollowCamera.h"
#include "../../Player/Player.h"

InputCamera::InputCamera(Player* player, FollowCamera* followCamera) {
	player_ = player;
	followCamera_ = followCamera;

	stateName_ = "Input";

	radian_ = followCamera_->GetRadian();

	startDist_ = followCamera_->kTargetDist;
	startTargetPos_ = followCamera_->GetTargetPosition();
}

void InputCamera::Initialize() {

}

void InputCamera::Update() {
	if (t_ <= followCamera_->returnTargetDist) {
		t_++;
		// カメラと追従対象との距離を初期の値に徐々に戻す
		followCamera_->kTargetDist = LWP::Utility::Interpolation::Lerp(startDist_, followCamera_->defaultTargetDist_, (t_ / followCamera_->returnTargetDist));
		followCamera_->SetTargetPosition(LWP::Utility::Interpolation::Lerp(startTargetPos_, player_->GetWorldTF()->GetWorldPosition(), (t_ / followCamera_->returnTargetDist)));
	}
	else {
		followCamera_->SetTargetPosition(player_->GetWorldTF()->GetWorldPosition());
	}

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

	radian_ = LWP::Utility::Interpolation::Exponential(radian_, LWP::Math::Vector3{ radian_.x, radian_.y, 0.0f }, 0.01f);

	radian_.x += stickDir_.x;
	radian_.y += stickDir_.y;
	if (radian_.y >= 2 * (float)std::numbers::pi) {
		radian_.y -= 2 * (float)std::numbers::pi;
	}
	if (radian_.y <= -2 * (float)std::numbers::pi) {
		radian_.y += 2 * (float)std::numbers::pi;
	}
	followCamera_->SetRadian(radian_);

	// x軸回転
	followCamera_->SetCameraRotate(LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 0, 1 }, radian_.z) * LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, radian_.x + 0.03f * stickDir_.x));
	// y軸は常に上を向くように固定
	followCamera_->SetCameraRotate(LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y + 0.03f * stickDir_.y) * followCamera_->GetCamera()->worldTF.rotation);
}