#include "ParryCamera.h"
#include "../FollowCamera.h"
#include "../../Player/Command/InputHandler.h"
#include "../../Player/Player.h"
#include "InputCamera.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

ParryCamera::ParryCamera(Player* player, FollowCamera* followCamera) {
	player_ = player;
	followCamera_ = followCamera;

	stateName_ = "Parry";

	// ロックオン入力を禁止
	InputHandler::GetInstance()->SetCurrentBanInput(StartBanInput(InputHandler::GetInstance()->GetBanInput(), BanLockOn));

	shakeRange_ = { 0.1f, 0.1f,0.0f };

	// カメラの揺れ
	shake_.SetEndFrame(22.0f);
	shake_.SetRange(shakeRange_);
	shake_.SetTarget(&shakeOffset_);
	shake_.SetIsActive(true);

	// 追従対象との距離をイージングするためのイベント生成
	CreateEventOrder();

	targetDistOrder_.Start();
}

ParryCamera::~ParryCamera() {
	// ロックオン入力禁止解除
	InputHandler::GetInstance()->SetCurrentBanInput(EraceBanInput(InputHandler::GetInstance()->GetBanInput(), BanLockOn));
}

void ParryCamera::Initialize() {

}

void ParryCamera::Update() {
	// カメラの揺れ
	shake_.Update();
	// 揺れを適用
	followCamera_->SetShakeOffset(shake_.GetValue());
	shakeRange_ = LWP::Utility::Interpolation::Exponential(shakeRange_, Vector3{ 0.0f,0.0f,0.0f }, 0.08f);
	shake_.SetRange(shakeRange_);

	targetDistOrder_.Update();

	// ロックオン対象とカメラの距離を算出
	TargetDistUpdate();

	// カメラの角度を算出
	RotateUpdate();

	// カメラの演出が終わったら状態変更(この処理以降何も書かないこと。Stateが解放されるのでアクセスエラーになりますよ)
	if (targetDistOrder_.GetIsEnd()) {
		followCamera_->ChangeState(new InputCamera(followCamera_));
		return;
	}
}

void ParryCamera::RotateUpdate() {
	// カメラが近づいているときのみ行う
	if (targetDistOrder_.GetCurrentTimeEvent().name != "Zoom") { return; }

	// ロックオン対象とカメラの間の座標をロックオンする
	// 方向ベクトルを算出
	lwp::Vector3 dist = (player_->GetSystemManager()->GetParrySystem()->GetParryTargetPos() - followCamera_->GetCamera()->worldTF.translation).Normalize() / 2.0f;
	LWP::Math::Vector3 radian = {
		0.0f,														// X軸（上下)
		atan2(dist.x, dist.z),										// Y軸（左右）
		0.08f
	};

	// 数値が1なら角度制限を行わない
	float isClampAngle = 1;
	// 角度制限
	followCamera_->ClampAngle(isClampAngle, (player_->GetSystemManager()->GetParrySystem()->GetParryTargetPos() - followCamera_->GetCamera()->worldTF.translation).Normalize() / 2.0f, LWP::Utility::DegreeToRadian(followCamera_->kOriginRotateX + 0.0f), LWP::Utility::DegreeToRadian(followCamera_->kOriginRotateX + 80.0f));

	radian_.z = LWP::Utility::Interpolation::Exponential(radian_, radian, 0.1f).z;
	// y軸は常に上を向くように固定
	LWP::Math::Quaternion roll = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 0, 1 }, radian_.z);

	// 角度制限がない場合
	if (isClampAngle == 1) {
		radian_.x = LWP::Utility::Interpolation::Exponential(radian_, radian, 0.6f).x;
		radian_.y = LWP::Utility::Interpolation::Exponential(radian_, radian, 0.6f).y;
		// x軸回転
		followCamera_->SetCameraRotate(roll * LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, radian_.x));
	}
	// 角度制限がある場合
	else {
		// x軸回転
		followCamera_->SetCameraRotate(roll * LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, radian_.x));
	}

	// y軸は常に上を向くように固定
	followCamera_->SetCameraRotate(LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y) * followCamera_->GetCamera()->worldTF.rotation);
}

void ParryCamera::TargetDistUpdate() {
	// カメラを近づける
	if (targetDistOrder_.GetCurrentTimeEvent().name == "Zoom") {
		followCamera_->kTargetDist = Lerp(followCamera_->defaultTargetDist_, followCamera_->defaultTargetDist_ + Vector3{ -0.2f, 0.1f, 2.5f }, Easing::OutExpo(targetDistOrder_.GetCurrentFrame() / zoomFinishTime));
	}
	else if (targetDistOrder_.GetCurrentTimeEvent().name == "Wait") {

	}
	else if (targetDistOrder_.GetCurrentTimeEvent().name == "Return") {
		followCamera_->kTargetDist = LWP::Utility::Interpolation::Exponential(followCamera_->kTargetDist, followCamera_->defaultTargetDist_, 0.05f);

		// Z軸を戻す
		radian_.z = LWP::Utility::Interpolation::Exponential(radian_, Vector3{ 0,0,0 }, 0.6f).z;
		// 
		LWP::Math::Quaternion roll = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 0, 1 }, radian_.z);
		// x軸回転
		followCamera_->SetCameraRotate(roll * LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, radian_.x));
		// y軸は常に上を向くように固定
		followCamera_->SetCameraRotate(LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y) * followCamera_->GetCamera()->worldTF.rotation);
	}
}

void ParryCamera::CreateEventOrder() {
	targetDistOrder_.Initialize();
	// カメラを近づける
	targetDistOrder_.CreateTimeEvent(TimeEvent{ zoomFinishTime, "Zoom" });
	targetDistOrder_.CreateTimeEvent(TimeEvent{ zoomHoldFinishTime, "Wait" });
	// カメラを戻す
	targetDistOrder_.CreateTimeEvent(TimeEvent{ returnFinishTime, "Return" });
}