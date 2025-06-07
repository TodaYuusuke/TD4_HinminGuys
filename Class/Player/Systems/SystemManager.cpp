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
	delete currentSystem_;
	delete comboTree_;
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
	// 被弾機能
	damageResponseSystem_ = std::make_unique<DamageResponse>(pCamera_, player_);
	damageResponseSystem_->CreateJsonFIle();
	damageResponseSystem_->Initialize();

	comboTree_ = new ComboTree();
	// コンボツリーの初期化
	comboTree_->Init("Combo.json", player_->GetModel(), player_->GetAnimation());
	// コライダーのマスク設定
	comboTree_->SetColliderMaskFrag(GameMask::GetAttack(), GameMask::GetEnemy());
	// 攻撃の判定
	onCollision_ = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		// 鞘が外れている状態だと減らさない
		//if (player_->GetSystemManager()->GetSheathSystem()->GetSheathState()->GetStateName() != "Throw") { return; }

		player_->TakeSheathDamage(comboTree_->GetSheathDurabityLoss());
		};
	comboTree_->AddCollisionLamda(LWP::Utility::ComboEnum::ENTER, onCollision_);

#pragma region json用
	// 移動機能
	moveSystem_ = std::make_unique<Move>(pCamera_, player_);
	moveSystem_->CreateJsonFIle();
	moveSystem_->Initialize();
	// 角度が変になるので一度更新処理を入れ込む
	moveSystem_->Update();
	systems_.push_back(moveSystem_.get());
	// パリィ機能
	parrySystem_ = std::make_unique<Parry>(pCamera_, player_);
	parrySystem_->CreateJsonFIle();
	parrySystem_->Initialize();
	systems_.push_back(parrySystem_.get());
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
	// 攻撃機能
	attackSystem_ = std::make_unique<Attack>(pCamera_, player_, enemyManager_);
	attackSystem_->CreateJsonFIle();
	attackSystem_->Initialize();
	attackSystem_->SetLockOnSystem(lockOnSystem_.get());
	systems_.push_back(attackSystem_.get());
#pragma endregion

	// 入力状態
	inputState_ = InputState::kMove;

	// 移動機能をセット
	CreateMoveSystem();
	preSystemState_ = systemState_;
}

void SystemManager::Update() {
	// ロックオン機能
	lockOnSystem_->Update();
	// ダメージリアクション機能
	damageResponseSystem_->Update();


	if (currentSystem_) {
		comboTree_->Update();

		// 現在稼働している機能
		currentSystem_->Update();

		// 速度
		velocity_ = currentSystem_->GetVelocity();
		// 角度
		radian_ = currentSystem_->GetRadian();
		quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);

		// リセット関数を代入されたら呼びだす
		if (resetSystemFunc_) {
			resetSystemFunc_();
			resetSystemFunc_ = nullptr;
			delete currentSystem_;
			currentSystem_ = nullptr;
		}
	}

	// 機能の切り替え条件
	SwitchCurrentSystem();
}

void SystemManager::Reset() {

}

void SystemManager::DebugGUI() {
#ifdef _DEBUG
	// ロックオン
	lockOnSystem_->DebugGUI();
	damageResponseSystem_->DebugGUI();

	// 各機能
	for (ISystem* system : systems_) {
		system->DebugGUI();
	}
#endif // DEBUG
}

void SystemManager::CreateMoveSystem() {
	// 現在のシステムを一度削除
	if (currentSystem_) { delete currentSystem_; }

	// 移動機能
	Move* moveSystem = new Move(pCamera_, player_);
	moveSystem->SetJsonData(moveSystem_->GetJsonData());
	moveSystem->Initialize();
	moveSystem->Command();
	moveSystem->SetRotate(radian_);
	moveSystem->SetRotate(quat_);
	moveSystem->Update();

	currentSystem_ = moveSystem;

	systemState_ = SystemState::kMove;
}

void SystemManager::CreateAttackSystem() {
	// 現在のシステムを一度削除
	if (currentSystem_) { delete currentSystem_; }

	// 攻撃機能
	Attack* attackSystem = new Attack(pCamera_, player_, enemyManager_);
	attackSystem->SetLockOnSystem(lockOnSystem_.get());
	attackSystem->SetJsonData(attackSystem_->GetJsonData());
	attackSystem->Initialize();
	attackSystem->Command();

	currentSystem_ = attackSystem;

	systemState_ = SystemState::kAttack;
}

void SystemManager::CreateEvasionSystem() {
	// 現在のシステムを一度削除
	if (currentSystem_) { delete currentSystem_; }

	// 回避機能
	Evasion* evasionSystem = new Evasion(pCamera_, player_);
	evasionSystem->SetJsonData(evasionSystem_->GetJsonData());
	evasionSystem->Initialize();
	evasionSystem->Command();

	currentSystem_ = evasionSystem;

	systemState_ = SystemState::kEvasion;
}

void SystemManager::CreateParrySystem() {
	// 現在のシステムを一度削除
	if (currentSystem_) { delete currentSystem_; }

	// パリィ機能
	Parry* parrySystem = new Parry(pCamera_, player_);
	parrySystem->SetJsonData(parrySystem_->GetJsonData());
	parrySystem->Initialize();
	parrySystem->Command();

	currentSystem_ = parrySystem;

	systemState_ = SystemState::kParry;
}

void SystemManager::CreateSheathSystem() {
	// 現在のシステムを一度削除
	if (currentSystem_) { delete currentSystem_; }

	// 鞘機能
	Sheath* sheathSystem = new Sheath(pCamera_, player_);
	sheathSystem->SetJsonData(sheathSystem_->GetJsonData());
	sheathSystem->Initialize();
	sheathSystem->Command();

	currentSystem_ = sheathSystem;

	systemState_ = SystemState::kSheath;
}

void SystemManager::SwitchCurrentSystem() {
	// 現在のシステムに何も入ってないなら移動機能を入れる
	if (!currentSystem_) { CreateMoveSystem(); }
	// 次に遷移したい機能がない時は処理しない
	if (currentSystem_->GetNextSystems().empty()) {
		return;
	}

	// 攻撃
	if (currentSystem_->GetNextSystem(SystemState::kAttack) && systemState_ != SystemState::kAttack) {
		CreateAttackSystem();
	}
	// 回避
	else if (currentSystem_->GetNextSystem(SystemState::kEvasion) && systemState_ != SystemState::kEvasion) {
		CreateEvasionSystem();
		// コンボ初期化
		ComboReset();
	}
	// パリィ
	else if (currentSystem_->GetNextSystem(SystemState::kParry) && systemState_ != SystemState::kParry) {
		CreateParrySystem();
		// コンボ初期化
		ComboReset();
	}
	// 鞘
	else if (currentSystem_->GetNextSystem(SystemState::kSheath) && systemState_ != SystemState::kSheath) {
		//CreateSheathSystem();
		// コンボ初期化
		ComboReset();
	}
	// 移動
	else if (currentSystem_->GetNextSystem(SystemState::kMove) && systemState_ != SystemState::kMove) {
		CreateMoveSystem();
		// コンボ初期化
		ComboReset();
	}

	preSystemState_ = systemState_;
}