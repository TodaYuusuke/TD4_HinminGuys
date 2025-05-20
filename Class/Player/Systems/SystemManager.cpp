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

SystemManager::~SystemManager() {
	delete currentState_;
}

void SystemManager::Initialize() {
	// コマンドの登録
	inputHandler_ = InputHandler::GetInstance();

	// ロックオン機能
	lockOnSystem_ = std::make_unique<LockOn>(pCamera_, player_);
	lockOnSystem_->CreateJsonFIle();
	lockOnSystem_->Initialize();
	lockOnSystem_->SetEnemyList(enemyManager_->GetEnemyListPtr());
	lockOnSystem_->SetFollowCamera(followCamera_);
	systems_.push_back(lockOnSystem_.get());
	// パリィ機能
	parrySystem_ = std::make_unique<Parry>(pCamera_, player_);
	parrySystem_->CreateJsonFIle();
	parrySystem_->Initialize();
	systems_.push_back(parrySystem_.get());
	// 攻撃機能
	attackSystem_ = std::make_unique<Attack>(pCamera_, player_);
	attackSystem_->CreateJsonFIle();
	attackSystem_->Initialize();
	attackSystem_->SetLockOnSystem(lockOnSystem_.get());
	systems_.push_back(attackSystem_.get());
	// 移動機能
	moveSystem_ = std::make_unique<Move>(pCamera_, player_);
	moveSystem_->CreateJsonFIle();
	moveSystem_->Initialize();
	systems_.push_back(moveSystem_.get());
	// 回避機能
	evasionSystem_ = std::make_unique<Evasion>(pCamera_, player_);
	evasionSystem_->CreateJsonFIle();
	evasionSystem_->Initialize();
	systems_.push_back(evasionSystem_.get());
	// 鞘機能
	sheathSystem_ = std::make_unique<Sheath>(pCamera_, player_);
	sheathSystem_->CreateJsonFIle();
	sheathSystem_->Initialize();
	systems_.push_back(sheathSystem_.get());

	// 現在起動しているシステム
	Move* move = new Move(pCamera_, player_);
	move->Initialize();
	move->Command();
	currentState_ = move;
	//*preState_ = *currentState_;

	// 自機のアニメーションを管理
	animator_.Initialize(player_->GetAnimation());

	// 入力状態
	inputState_ = InputState::kMove;
}

void SystemManager::Update() {
	// アニメーションを振り分ける
	animator_.Update(*player_);

	// 各機能
	//for (ISystem* system : systems_) {
		//system->Update();
	//}

	// システム遷移条件
	if (currentState_) {
		// 次に遷移するシステムを取得
		nextEnableState_ = currentState_->GetNextState();
		//if (preState_ != currentState_) {
			// 入力があった機能は遷移条件を満たしているかを検索
			//if (inputHandler_->CheckEnableCommand(InputParry)) {
			if (currentState_->GetCurrentState() != InputParry && IsBitSame(InputParry, currentState_->GetNextState(), GetSetBitPosition(currentState_->GetNextState()))) {
				/*parrySystem_->Command();
				ChangeState(parrySystem_.get()); */
				//*preState_ = *currentState_;
				Parry* parry = new Parry(pCamera_, player_);
				parry->Initialize();
				parry->Command();
				ChangeState(parry);
			}
			//else if (inputHandler_->CheckEnableCommand(InputAttack)) {
			else if (currentState_->GetCurrentState() != InputAttack && IsBitSame(InputAttack, currentState_->GetNextState(), GetSetBitPosition(currentState_->GetNextState()))) {
				//*preState_ = *currentState_;
				Attack* attack = new Attack(pCamera_, player_);
				attack->Initialize();
				attack->NormalCommand();
				ChangeState(attack);
			}
			//else if (inputHandler_->CheckEnableCommand(InputEvasion)) {
			else if (currentState_->GetCurrentState() != InputEvasion && IsBitSame(InputEvasion, currentState_->GetNextState(), GetSetBitPosition(currentState_->GetNextState()))) {
				/*evasionSystem_->Command();
				ChangeState(evasionSystem_.get());*/
				//*preState_ = *currentState_;
				Evasion* evasion = new Evasion(pCamera_, player_);
				evasion->Initialize();
				evasion->Command();
				ChangeState(evasion);
			}
			//else if (inputHandler_->CheckEnableCommand(InputSheath)) {
			else if (currentState_->GetCurrentState() != InputSheath && IsBitSame(InputSheath, currentState_->GetNextState(), GetSetBitPosition(currentState_->GetNextState()))) {
				/*sheathSystem_->Command();
				ChangeState(sheathSystem_.get());*/
				//*preState_ = *currentState_;
				Sheath* sheath = new Sheath(pCamera_, player_);
				sheath->Initialize();
				sheath->Command();
				ChangeState(sheath);
			}
			//else if (inputHandler_->CheckEnableCommand(InputMove)) {
			else if (currentState_->GetCurrentState() != InputMove && IsBitSame(InputMove, currentState_->GetNextState(), GetSetBitPosition(currentState_->GetNextState()))) {
				/*parrySystem_->Command();
				ChangeState(moveSystem_.get()); */
				//*preState_ = *currentState_;
				Move* move = new Move(pCamera_, player_);
				move->Initialize();
				move->Command();
				ChangeState(move);
			}

		//}
	}

	// 稼働している機能
	currentState_->Update();

	// 移動入力が可能な状態なら自機に速度を加算
	EnableInputMoveState();

	//*preState_ = *currentState_;
	//preEnableState_ = nextEnableState_;
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
		//system->DebugGUI();
	}
#endif // DEBUG
}

void SystemManager::ChangeState(ISystem* pState) {
	delete currentState_;
	currentState_ = pState;
}

void SystemManager::EnableInputMoveState() {
	switch (inputState_) {
	case InputState::kMove:
		// 速度を加算
		velocity_ = moveSystem_->GetMoveVel();
		// 角度を加算
		radian_ = moveSystem_->GetMoveRadian();
		// クォータニオンに変換
		rotate_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
		break;
	case InputState::kAttack:
		// 速度を加算
		velocity_ = LWP::Utility::Interpolation::Exponential(velocity_, attackSystem_->GetAttackAssistVel(), 0.9f);
		// 角度を加算
		radian_ = attackSystem_->GetAttackAssistRadian();
		// クォータニオンに変換
		rotate_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);

		// MoveSystemクラス内の角度も変更
		if (Vector3::Dot(velocity_, velocity_) != 0) {
			moveSystem_->SetRotate(radian_);
		}
		moveSystem_->SetMoveVel({ 0,0,0 });
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
		velocity_ = LWP::Utility::Interpolation::Exponential(velocity_, evasionSystem_->GetVelocity(), 1.0f);
		// 角度を加算
		radian_ = moveSystem_->GetMoveRadian();
		// クォータニオンに変換
		rotate_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
		break;
	case InputState::kSheath:
		// 速度を加算
		velocity_ = sheathSystem_->GetVelocity();
		// 角度を加算
		radian_ = sheathSystem_->GetRadian();
		// クォータニオンに変換
		rotate_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);

		// MoveSystemクラス内の角度も変更
		if (Vector3::Dot(velocity_, velocity_) != 0) {
			moveSystem_->SetRotate(radian_);
		}
		break;
	}
}