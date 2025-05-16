#include "SystemManager.h"
#include "../../Enemy/EnemyManager.h"
#include "../Player.h"

using namespace LWP;
using namespace LWP::Math;

SystemManager::SystemManager(Player* player, EnemyManager* enemyManager, FollowCamera* followCamera, LWP::Object::Camera* camera) {
	player_ = player;
	enemyManager_ = enemyManager;
	followCamera_ = followCamera;
	pCamera_ = camera;
}

void SystemManager::Initialize() {
	// ロックオン機能
	lockOnSystem_ = std::make_unique<LockOn>(pCamera_, player_);
	lockOnSystem_->Initialize();
	lockOnSystem_->SetEnemyList(enemyManager_->GetEnemyListPtr());
	lockOnSystem_->SetFollowCamera(followCamera_);
	systems_.push_back(lockOnSystem_.get());
	// パリィ機能
	parrySystem_ = std::make_unique<Parry>(pCamera_, player_);
	parrySystem_->Initialize();
	systems_.push_back(parrySystem_.get());
	// 攻撃機能
	attackSystem_ = std::make_unique<Attack>(pCamera_, player_);
	attackSystem_->Initialize();
	attackSystem_->SetLockOnSystem(lockOnSystem_.get());
	systems_.push_back(attackSystem_.get());
	// 移動機能
	moveSystem_ = std::make_unique<Move>(pCamera_, player_);
	moveSystem_->Initialize();
	systems_.push_back(moveSystem_.get());
	// 回避機能
	evasionSystem_ = std::make_unique<Evasion>(pCamera_, player_);
	evasionSystem_->Initialize();
	systems_.push_back(evasionSystem_.get());
	// 鞘機能
	sheathSystem_ = std::make_unique<Sheath>(pCamera_, player_);
	sheathSystem_->Initialize();
	systems_.push_back(sheathSystem_.get());

	animator_.Initialize(player_->GetAnimation());

	// 入力状態
	inputState_ = InputState::kMove;
}

void SystemManager::Update() {
	// アニメーションを振り分ける
	animator_.Update(*player_);

	// 各機能
	for (ISystem* system : systems_) {
		system->Update();
	}

	// 移動入力が可能な状態なら自機に速度を加算
	EnableInputMoveState();
}

void SystemManager::Reset() {
	// 各機能
	for (ISystem* system : systems_) {
		system->Reset();
	}
}

void SystemManager::DebugGUI() {
#ifdef _DEBUG
	// 各機能
	for (ISystem* system : systems_) {
		system->DebugGUI();
	}
#endif // DEBUG
}

void SystemManager::EnableInputMoveState() {
	switch (inputState_) {
	case InputState::kMove:
		moveSystem_->SetIsActive(true);

		// 速度を加算
		velocity_ = moveSystem_->GetMoveVel();
		// 角度を加算
		radian_ = moveSystem_->GetMoveRadian();
		// クォータニオンに変換
		rotate_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
		break;
	case InputState::kAttack:
		moveSystem_->SetIsActive(false);

		// 速度を加算
		velocity_ = LWP::Utility::Interpolation::Exponential(velocity_,attackSystem_->GetAttackAssistVel(), 0.9f);
		// 角度を加算
		radian_ = attackSystem_->GetAttackAssistRadian();
		// クォータニオンに変換
		rotate_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);

		// MoveSystemクラス内の角度も変更
		if (Vector3::Dot(velocity_, velocity_) != 0) {
			moveSystem_->SetRotate(radian_);
		}
		break;
	case InputState::kParry:
		// 速度を加算
		velocity_ = LWP::Utility::Interpolation::Exponential(velocity_, Vector3{ 0.0f,0.0f,0.0f }, 0.9f);
		// 角度を加算
		radian_ = moveSystem_->GetMoveRadian();
		// クォータニオンに変換
		rotate_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
		break;
	case InputState::kEvasion:
		// 速度を加算
		velocity_ = LWP::Utility::Interpolation::Exponential(velocity_, evasionSystem_->GetVelocity() + moveSystem_->GetMoveVel(), 1.0f);
		// 角度を加算
		radian_ = moveSystem_->GetMoveRadian();
		// クォータニオンに変換
		rotate_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
		break;
	case InputState::kSheath:
		moveSystem_->SetIsActive(false);

		// 速度を加算
		velocity_ = LWP::Utility::Interpolation::Exponential(velocity_, sheathSystem_->GetVelocity(), 0.9f);
		// 角度を加算
		radian_ = sheathSystem_->GetRadian();

	/*	if (!sheathSystem_->GetIsActionRestrict("Collect")) {
			velocity_ += LWP::Utility::Interpolation::Exponential(velocity_, moveSystem_->GetMoveVel(), 0.9f); ;
			radian_ += sheathSystem_->GetRadian();
			moveSystem_->SetIsActive(true);
		}*/

		// クォータニオンに変換
		rotate_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);

		// MoveSystemクラス内の角度も変更
		if (Vector3::Dot(velocity_, velocity_) != 0) {
			moveSystem_->SetRotate(radian_);
		}
		break;
	}
}