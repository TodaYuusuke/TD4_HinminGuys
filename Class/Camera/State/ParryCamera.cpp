#include "ParryCamera.h"
#include "../FollowCamera.h"
#include "../../Player/Command/InputHandler.h"
#include "../../Player/Player.h"
#include "../../Components/HitStopController.h"
#include "InputCamera.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

ParryCamera::ParryCamera(Player* player, FollowCamera* followCamera) {
	player_ = player;
	followCamera_ = followCamera;

	stateName_ = "Parry";

	shakeRange_ = followCamera_->parryShakeRange;
	// カメラの揺れ
	shake_.SetEndFrame(followCamera_->parryShakeTime);
	shake_.SetRange(shakeRange_);
	shake_.SetTarget(&shakeOffset_);
	shake_.SetIsActive(true);

	// 追従対象との距離をイージングするためのイベント生成
	CreateEventOrder();
	targetDistOrder_.Start();

	Vector3 dist = (player_->GetSystemManager()->GetSuccessParryData().targetPos - player_->GetWorldTF()->GetWorldPosition()) / 2.0f;
	Vector3 p2c = (player_->GetWorldTF()->GetWorldPosition() - followCamera_->GetCamera()->worldTF.GetWorldPosition()).Normalize();
	float yaw = atan2(dist.x, dist.z);
	targetPos_ = player_->GetWorldTF()->GetWorldPosition() + dist;

	// イージング開始角度
	start_ = followCamera_->GetRadian();
	// 最短距離で補間するためにカメラの角度がどちらに近いかを求める
	Vector3 checkCloseAngle[2] = {
		{
			followCamera_->parryAngle.x,
			yaw + followCamera_->parryAngle.y,
			-followCamera_->parryAngle.z
		},
		{
			followCamera_->parryAngle.x,
			yaw - followCamera_->parryAngle.y,
			followCamera_->parryAngle.z
		},
	};
	float checkDiff[2];
	for (int i = 0; i < 2; i++) {
		checkDiff[i] = std::fabsf(followCamera_->GetRadian().y - checkCloseAngle[i].y);
	}

	if (checkDiff[0] >= checkDiff[1]) {
		end_ = checkCloseAngle[1];
	}
	else {
		end_ = checkCloseAngle[0];
	}

	// ラジアルブラー
	followCamera_->camera_->pp.use = true;
	followCamera_->camera_->pp.radialBlur.use = true;
	followCamera_->camera_->pp.radialBlur.blurWidth = followCamera_->parryBlurWidth;
	// ブルーム
	//followCamera_->camera_->pp.bloom.use = true;
	followCamera_->camera_->pp.CreateShaderFile();

	// ヒットストップ
	HitStopController::GetInstance()->Start(0.3f, timeScale_);
}

ParryCamera::~ParryCamera() {
	followCamera_->camera_->pp.use = false;
	followCamera_->camera_->pp.radialBlur.use = false;
	timeScale_ = 1.0f;
	HitStopController::GetInstance()->SetTimeMultiply(timeScale_);
}

void ParryCamera::Initialize() {

}

void ParryCamera::Update() {
	Vector3 dist = (player_->GetSystemManager()->GetSuccessParryData().targetPos - player_->GetWorldTF()->GetWorldPosition()) / 2.0f;
	targetPos_ = player_->GetWorldTF()->GetWorldPosition() + dist;
	followCamera_->SetTargetPosition(targetPos_);

	// カメラの揺れ
	shake_.Update();
	// 揺れを適用
	followCamera_->SetShakeOffset(shake_.GetValue());
	shakeRange_ = LWP::Utility::Interpolation::Exponential(shakeRange_, Vector3{ 0.0f,0.0f,0.0f }, 0.05f);
	shake_.SetRange(shakeRange_);

	t_++;

	if (t_ >= 0.2f * 60.0f) {
		timeScale_ = LerpF(0.0f, 1.0f, Utility::Easing::InExpo((t_ - 0.2f * 60.0f) / (followCamera_->zoomFinishTime - 0.2f * 60.0f)));
		timeScale_ = std::clamp<float>(timeScale_, 0.0f, 1.0f);
		HitStopController::GetInstance()->SetTimeMultiply(timeScale_);
	}

	// ロックオン対象とカメラの距離を算出
	TargetDistUpdate();

	// カメラの角度を算出
	RotateUpdate();

	// カメラの演出が終わったら状態変更(この処理以降何も書かないこと。Stateが解放されるのでアクセスエラーになりますよ)
	if (targetDistOrder_.GetIsEnd()) {
		player_->GetSystemManager()->SetIsSuccessParry(false);
		followCamera_->ChangeState(new InputCamera(player_,followCamera_));
		return;
	}
}

void ParryCamera::RotateUpdate() {
	radian_ = Lerp(start_, end_, Utility::Easing::OutExpo(t_ / followCamera_->zoomFinishTime));

	followCamera_->SetRadian(radian_);

	// x軸回転
	followCamera_->SetCameraRotate(LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 0, 1 }, radian_.z) * LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, radian_.x));

	// y軸は常に上を向くように固定
	followCamera_->SetCameraRotate(LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y) * followCamera_->GetCamera()->worldTF.rotation);
}

void ParryCamera::TargetDistUpdate() {
	targetDistOrder_.Update();

	// カメラを近づける
	if (targetDistOrder_.GetCurrentTimeEvent().name == "Zoom") {
		followCamera_->kTargetDist = Lerp(followCamera_->defaultTargetDist_, followCamera_->defaultTargetDist_ + followCamera_->parryDist, Easing::OutExpo(targetDistOrder_.GetCurrentFrame() / followCamera_->zoomFinishTime));
	}
}

void ParryCamera::CreateEventOrder() {
	targetDistOrder_.Initialize();
	targetDistOrder_.SetIsTimeScale(false);
	// カメラを近づける
	targetDistOrder_.CreateTimeEvent(TimeEvent{ followCamera_->zoomFinishTime, "Zoom" });
	targetDistOrder_.CreateTimeEvent(TimeEvent{ followCamera_->zoomHoldFinishTime, "Wait" });
}

Vector3 ParryCamera::RotateToDir(const Vector3& rotate) {
	Vector3 result = { 0,0,-1 };
	// 回転行列を求める
	Matrix4x4 rotMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(rotate);
	result = result * rotMatrix;

	return result;
}

int ParryCamera::DetermineSide3D(const Vector3& forward, const Vector3& targetDir, const Vector3& up) {
	Vector3 cross = Vector3::Cross(forward, targetDir);
	float dot = Vector3::Dot(cross, up);

	if (dot > 0) return 1;   // 右
	if (dot < 0) return -1;  // 左
	return 0;                // 正面 or 背後
}
