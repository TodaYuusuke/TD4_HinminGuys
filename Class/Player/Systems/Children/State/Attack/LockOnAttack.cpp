#include "LockOnAttack.h"
#include "../../../../Player.h"

using namespace LWP;
using namespace LWP::Math;

LockOnAttack::LockOnAttack(Attack* attackSystem, Player* player, IEnemy* lockOnTarget) {
	attackSystem_ = attackSystem;
	player_ = player;
	lockOnTarget_ = lockOnTarget;
}

void LockOnAttack::Initialize() {
	assistVel_ = { 0.0f,0.0f,0.0f };
	assistRadian_ = { 0.0f,0.0f,0.0f };
	assistQuat_ = { 0.0f,0.0f,0.0f,1.0f };
}

void LockOnAttack::Update() {
	// 自機とロックオン中の敵との距離
	Vector3 attackTargetDist = (player_->GetLockOnSystem()->GetCurrentLockOnTarget()->GetWorldTF()->GetWorldPosition() - player_->GetWorldTF()->GetWorldPosition()) * 0.4f;
	assistVel_ = LWP::Utility::Interpolation::Slerp(Vector3{ 0.0f,0.0f,0.0f }, attackTargetDist, 0.25f);

	// 移動速度からラジアンを求める
	assistRadian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, assistVel_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
	assistQuat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, assistRadian_.y);

	// AttackSystemクラスに送る
	attackSystem_->SetAttackAssistVel(assistVel_);
	attackSystem_->SetAttackAssistRadian(assistRadian_);
	attackSystem_->SetAttackAssistQuat(assistQuat_);
}
