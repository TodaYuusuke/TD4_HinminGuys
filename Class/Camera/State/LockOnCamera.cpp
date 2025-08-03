#include "LockOnCamera.h"
#include "../FollowCamera.h"
#include "../../Player/Player.h"

using namespace LWP;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

LockOnCamera::LockOnCamera(Player* player, FollowCamera* followCamera) {
	player_ = player;
	followCamera_ = followCamera;

	stateName_ = "LockOn";

	rate_ = 0.05f;
	radian_ = followCamera_->GetRadian();
}

void LockOnCamera::Initialize() {

}

void LockOnCamera::Update() {
	followCamera_->SetTargetPosition(player_->GetWorldTF()->GetWorldPosition());

	// カメラの角度を算出
	RotateUpdate();
}

void LockOnCamera::RotateUpdate() {
	// ロックオン対象との距離
	float lockOnTargetDist = (followCamera_->GetLockOnData().targetTransform->translation - followCamera_->GetTargetPos()).Length();

	if (lockOnTargetDist <= 1.8f) { return; }

	lockOnTargetDist = std::clamp<float>(lockOnTargetDist, followCamera_->maxLength / 10.0f, followCamera_->maxLength);

	// ロックオン対象との距離に応じてカメラのオフセットを変更
	// ロックオン対象から近いほどカメラが上に行く
	lockOnOffset_.x = followCamera_->defaultTargetDist_.x + (0.8f * (1.0f - lockOnTargetDist / followCamera_->maxLength));
	lockOnOffset_.y = followCamera_->defaultTargetDist_.y + (0.1f * (lockOnTargetDist / followCamera_->maxLength));
	lockOnOffset_.z = followCamera_->defaultTargetDist_.z - (2.0f * (1.0f - lockOnTargetDist / followCamera_->maxLength));
	followCamera_->kTargetDist = LWP::Utility::Interpolation::Exponential(followCamera_->kTargetDist, lockOnOffset_, followCamera_->targetDistRate);

	// ロックオン対象とカメラとの方向ベクトルを算出
	LWP::Math::Vector3 cameraPos = followCamera_->GetCamera()->worldTF.translation;
	lwp::Vector3 dist = ((followCamera_->GetLockOnData().targetTransform->GetWorldPosition() - cameraPos)).Normalize();
	LWP::Math::Vector2 dir;
	dir.y = atan2(dist.x, dist.z);                        // Y軸（左右）
	dir.x = atan2(-dist.y, sqrt(dist.x * dist.x + dist.z * dist.z)); // X軸（上下

	// ロックオンした瞬間なめらかに角度を補間
	t_++;
	rate_ = LerpF(0.05f, 1.0f, t_ / 60.0f);

	// 数値が1なら角度制限を行わない
	float isClampAngle = 1;
	// 角度制限
	followCamera_->ClampAngle(isClampAngle, ((followCamera_->GetLockOnData().targetTransform->GetWorldPosition()) - followCamera_->GetCamera()->worldTF.translation).Normalize(), LWP::Utility::DegreeToRadian(followCamera_->kOriginRotateX + 0.0f), LWP::Utility::DegreeToRadian(followCamera_->kOriginRotateX + 20.0f));

	// 角度が-2πor2π以上以下にならないようにする
	if (radian_.y >= 2 * (float)std::numbers::pi) {
		radian_.y -= 2 * (float)std::numbers::pi;
	}
	if (radian_.y <= -2 * (float)std::numbers::pi) {
		radian_.y += 2 * (float)std::numbers::pi;
	}

	// 角度上限下限に違反しているとき
	if (isClampAngle == 0) {
		radian_ = Exponential(radian_, Vector3{ radian_.x, dir.y, 0.0f }, rate_);
	}
	else {
		radian_ = Exponential(radian_, Vector3{ dir.x, dir.y, 0.0f }, rate_);
	}

	followCamera_->SetRadian(radian_);
}