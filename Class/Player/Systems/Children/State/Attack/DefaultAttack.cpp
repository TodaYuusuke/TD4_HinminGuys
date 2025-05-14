#include "DefaultAttack.h"
#include "../../../../Player.h"

using namespace LWP;
using namespace LWP::Math;

DefaultAttack::DefaultAttack(Attack* attackSystem, Player* player) {
	attackSystem_ = attackSystem;
	player_ = player;
}

void DefaultAttack::Initialize() { 
	assistVel_ = { 0.0f,0.0f,0.0f };
	assistRadian_ = { 0.0f,0.0f,0.0f };
	assistQuat_ = { 0.0f,0.0f,0.0f,1.0f };
}

void DefaultAttack::Update() {
	// 自機の方向ベクトル
	Vector3 playerDir = { 0.0f,0.0f,1.0f };
	// 回転行列を求める
	Matrix4x4 rotMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(player_->GetWorldTF()->rotation);
	// 方向ベクトルを求める
	playerDir = playerDir * rotMatrix;
	playerDir.y = 0;

	// 移動速度からラジアンを求める
	assistRadian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, playerDir.Normalize(), LWP::Math::Vector3{ 0,1,0 });
	assistQuat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, assistRadian_.y);

	// AttackSystemクラスに送る
	attackSystem_->SetAttackAssistVel(assistVel_);
	attackSystem_->SetAttackAssistRadian(assistRadian_);
	attackSystem_->SetAttackAssistQuat(assistQuat_);
}