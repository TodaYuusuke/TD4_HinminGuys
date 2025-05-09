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
	// 移動機能
	evasionSystem_ = std::make_unique<Evasion>(pCamera_, player_);
	evasionSystem_->Initialize();
	systems_.push_back(evasionSystem_.get());

	// 入力状態
	inputState_ = InputState::kMove;
}

void SystemManager::Update() {
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
	// 入力処理を禁止する処理
	// 回避時は方向転換のみ受け付ける
	if (evasionSystem_->GetIsActive()) {
		inputState_ = InputState::kEvasion;
	}
	// 攻撃時は移動入力の一切を受け付けない
	else if (attackSystem_->GetIsActive()) {
		inputState_ = InputState::kAttack;
		moveSystem_->SetIsActive(false);
	}
	// パリィ時は方向転換のみ受け付ける
	else if (parrySystem_->GetIsActive()) {
		inputState_ = InputState::kParry;
	}
	// 何もない時は移動入力を受け付ける
	else if (!parrySystem_->GetIsActive() && !attackSystem_->GetIsActive()) {
		inputState_ = InputState::kMove;
		moveSystem_->SetIsActive(true);

		// 移動中のアニメーション
		if (moveSystem_->GetIsMove()) {
			if (player_->GetAnimation()->GetLoadedPath() != "Run") {
				player_->StartAnimation("Run", 1.0f, 0.0f);
				player_->SetAnimationPlaySpeed(0.5f);
				player_->SetIsLoopAnimation(true);
			}
		}
		// 移動していないときのアニメーション
		else {
			if (player_->GetAnimation()->GetLoadedPath() != "Idle") {
				player_->StartAnimation("Idle", 1.0f, 0.0f);
				player_->SetAnimationPlaySpeed(1.0f);
				player_->SetIsLoopAnimation(true);
			}
		}
	}

	// ラジアン
	LWP::Math::Vector3 radian;

	switch (inputState_) {
	case InputState::kMove:
		// 速度を加算
		velocity_ = moveSystem_->GetMoveVel();
		// 角度を加算
		radian = moveSystem_->GetMoveRadian();
		// クォータニオンに変換
		rotate_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian.y);
		break;
	case InputState::kAttack:
		// 速度を加算
		velocity_ = attackSystem_->GetAttackAssistVel();
		// 角度を加算
		radian = attackSystem_->GetAttackAssistRadian();
		// クォータニオンに変換
		rotate_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian.y);

		// MoveSystemクラス内の角度も変更
		if (Vector3::Dot(velocity_, velocity_) != 0) {
			moveSystem_->SetRotate(radian);
		}
		break;
	case InputState::kParry:
		// 速度を消す
		velocity_ = { 0.0f,0.0f,0.0f };
		// 角度を加算
		radian = moveSystem_->GetMoveRadian();
		// クォータニオンに変換
		rotate_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian.y);
		break;
	case InputState::kEvasion:
		// 速度を加算
		velocity_ = evasionSystem_->GetVelocity();
		// 角度を加算
		radian = evasionSystem_->GetRadian();
		// クォータニオンに変換
		rotate_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian.y);
		break;
	}
}