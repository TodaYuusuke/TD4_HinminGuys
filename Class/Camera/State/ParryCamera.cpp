#include "ParryCamera.h"
#include "../FollowCamera.h"
#include "../../Player/Command/InputHandler.h"
#include "../../Player/Player.h"
#include "InputCamera.h"

using namespace LWP;
using namespace LWP::Math;

ParryCamera::ParryCamera(Player* player, FollowCamera* followCamera) {
	player_ = player;
	followCamera_ = followCamera;

	stateName_ = "Parry";

	// ロックオン入力を禁止
	InputHandler::GetInstance()->SetCurrentBanInput(StartBanInput(InputHandler::GetInstance()->GetBanInput(), BanLockOn));

	shakeRange_ = { 0.1f, 0.1f,0.0f };

	shake_.SetEndFrame(30.0f);
	shake_.SetRange(shakeRange_);
	shake_.SetTarget(&shakeOffset_);
	shake_.SetIsActive(true);
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
	shakeRange_ = LWP::Utility::Interpolation::Exponential(shakeRange_, Vector3{ 0.0f,0.0f,0.0f }, 0.3f);
	shake_.SetRange(shakeRange_);

	// ロックオン対象とカメラの距離を算出
	TargetDistUpdate();

	// カメラの角度を算出
	RotateUpdate();

	// カメラの演出が終わったら状態変更(この処理以降何も書かないこと。Stateが解放されるのでアクセスエラーになりますよ)
	if (currentFrame_ >= finishTime) {
		followCamera_->ChangeState(new InputCamera(followCamera_));
		return;
	}
}

void ParryCamera::RotateUpdate() {
	// ロックオン対象とカメラの間の座標をロックオンする
	// 方向ベクトルを算出
	lwp::Vector3 dist = (player_->GetSystemManager()->GetParrySystem()->GetParryTargetPos() / 2.0f - followCamera_->GetCamera()->worldTF.translation).Normalize();
	LWP::Math::Vector2 radian = {
		atan2(-dist.y, sqrt(dist.x * dist.x + dist.z * dist.z)),	// X軸（上下)
		atan2(dist.x, dist.z)										// Y軸（左右）
	};

	// 数値が1なら角度制限を行わない
	float isClampAngle = 1;
	// 角度制限
	followCamera_->ClampAngle(isClampAngle, (player_->GetSystemManager()->GetParrySystem()->GetParryTargetPos() - followCamera_->GetCamera()->worldTF.translation).Normalize(), LWP::Utility::DegreeToRadian(followCamera_->kOriginRotateX + 0.0f), LWP::Utility::DegreeToRadian(followCamera_->kOriginRotateX + 80.0f));

	if (currentFrame_ <= finishTime - 80.0f) {
		damping_ = LWP::Utility::Interpolation::Exponential(damping_, Vector3{ 0.0f, 0.0f, 0.1f }, 0.5f);
	}
	else {
		damping_ = LWP::Utility::Interpolation::Exponential(damping_, Vector3{ 0.0f, 0.0f, 0.0f }, 0.5f);
	}
	// y軸は常に上を向くように固定
	LWP::Math::Quaternion roll = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 0, 1 }, damping_.z);
	// z軸回転
	followCamera_->SetCameraRotate(roll * LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, radian.x));

	// 角度制限がない場合
	if (isClampAngle == 1) {
		radian_ = { radian.x, radian.y, 0.0f };
		// x軸回転
		followCamera_->SetCameraRotate(roll * LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, radian.x));
	}
	// 角度制限がある場合
	else {
		// x軸回転
		followCamera_->SetCameraRotate(roll * LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, radian_.x));
	}

	// y軸は常に上を向くように固定
	followCamera_->SetCameraRotate(LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian.y) * followCamera_->GetCamera()->worldTF.rotation);
}

void ParryCamera::TargetDistUpdate() {
	currentFrame_++;
	if (currentFrame_ <= finishTime - 80.0f) {
		lockOnOffset_ = {
			0.14f,
			0.25f,
			-2.0f
		};

		rate_ = 0.5f;
	}
	else {
		lockOnOffset_ = followCamera_->defaultTargetDist_;

		rate_ = 0.001f;
	}

	followCamera_->kTargetDist = LWP::Utility::Interpolation::Exponential(followCamera_->kTargetDist, lockOnOffset_, rate_);
}
