#include "InputCamera.h"
#include "../FollowCamera.h"
#include "../../Player/Player.h"

using namespace LWP;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

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
	t_++;
	// カメラと追従対象との距離を初期の値に徐々に戻す
	if (t_ < followCamera_->returnTargetDistTime) {
		followCamera_->kTargetDist = LWP::Utility::Interpolation::Lerp(startDist_, followCamera_->defaultTargetDist_, (t_ / followCamera_->returnTargetDistTime));
	}

	if (t_ < followCamera_->switchTargetPosTime) {
		followCamera_->SetTargetPosition(LWP::Utility::Interpolation::Lerp(startTargetPos_, player_->GetWorldTF()->GetWorldPosition(), (t_ / followCamera_->switchTargetPosTime)));
	}
	else {
		followCamera_->SetTargetPosition(player_->GetWorldTF()->GetWorldPosition());
	}

	// 初期角度にカメラの角度を戻す
	ReturnRotate();

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

	// 入力があったならカメラの角度を戻す処理をしない
	if (dir.Length() != 0.0f) {
		returnRot_.isActive = false;
	}

	if (!returnRot_.isActive) {
		// スティックの入力をイージング
		LWP::Math::Vector3 goal = { dir.x, dir.y, 0 };
		stickDir_ = LWP::Utility::Interpolation::Exponential(stickDir_, goal, followCamera_->rotateRate);

		// 角度制限
		followCamera_->ClampAngle(stickDir_.x, (followCamera_->GetTargetPos() - followCamera_->GetCamera()->worldTF.translation).Normalize(), LWP::Utility::DegreeToRadian(followCamera_->kOriginRotateX + followCamera_->kMinRotateX), LWP::Utility::DegreeToRadian(followCamera_->kOriginRotateX + followCamera_->kMaxRotateX));

		radian_.x += 0.03f * stickDir_.x;
		radian_.y += 0.03f * stickDir_.y;

		if (radian_.y >= 2 * (float)std::numbers::pi) {
			radian_.y -= 2 * (float)std::numbers::pi;
		}
		if (radian_.y <= -2 * (float)std::numbers::pi) {
			radian_.y += 2 * (float)std::numbers::pi;
		}
	}
	// Z軸を0に戻す
	radian_ = LWP::Utility::Interpolation::Exponential(radian_, LWP::Math::Vector3{ radian_.x, radian_.y, 0.0f }, 0.1f);

	followCamera_->SetRadian(radian_);
}

void InputCamera::ReturnRotate() {
	if (!returnRot_.isActive) { return; }

	returnRot_.t+=HitStopController::GetInstance()->GetDeltaTime();
	
	radian_.x = LerpF(returnRot_.start.x, returnRot_.end.x, LWP::Utility::Easing::OutCubic(returnRot_.t / followCamera_->returnRotateTime));

	if (returnRot_.t >= followCamera_->returnRotateTime) {
		returnRot_.isActive = false;
	}
}

void InputCamera::FinishReturnTargetDistance() {
	t_ = followCamera_->returnTargetDistTime;
}

void InputCamera::StartReturnRotate() {
	// カメラの角度を初期角度に戻す
	returnRot_.start = followCamera_->GetRadian();
	returnRot_.end = {
		LWP::Utility::DegreeToRadian(followCamera_->returnAngle.x) ,
		LWP::Utility::DegreeToRadian(followCamera_->returnAngle.y) ,
		0.0f
	};
	returnRot_.isActive = true;
}
