#include "SystemManager.h"
#include "../../Enemy/EnemyManager.h"
#include "../Player.h"

using namespace LWP;
using namespace LWP::Math;

SystemManager::SystemManager(Player* player, EnemyManager* enemyManager, FollowCamera* followCamera, LWP::Object::Camera* camera)
	: parryAABB_(parryCollision_.SetBroadShape(LWP::Object::Collider::AABB())),
	sheathAttackCapsule_(sheathCollision_.SetBroadShape(LWP::Object::Collider::Capsule()))
{
	player_ = player;
	enemyManager_ = enemyManager;
	followCamera_ = followCamera;
	pCamera_ = camera;
}

SystemManager::~SystemManager() {
	delete currentSystem_;
	delete comboTree_;
	delete coolTimer_;
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
	// 鞘機能
	sheathSystem_ = std::make_unique<Sheath>(pCamera_, player_);
	sheathSystem_->CreateJsonFIle();
	sheathSystem_->Initialize();

	// クールタイマー生成
	coolTimer_ = new CoolTimer();

	// コンボ
	comboTree_ = new ComboTree();
	// コンボツリーの初期化
	comboTree_->Init("Combo.json", player_->GetModel(), player_->GetAnimation());
	// コライダーのマスク設定
	comboTree_->SetColliderMaskFrag(GameMask::GetAttack(), GameMask::GetEnemy());
	// 攻撃の判定
	attackOnHitFunc_ = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		// 鞘が外れている状態だと減らさない
		if (player_->GetSystemManager()->GetSheathSystem()->GetSheathState()->GetStateName() != "Throw") { return; }

		player_->TakeSheathDamage(comboTree_->GetSheathDurabityLoss());
		};
	comboTree_->AddCollisionLamda(LWP::Utility::ComboEnum::ENTER, attackOnHitFunc_);

	// パリィ判定生成
	parryAABB_.min = { -1.0f, -1.0f, -1.0f };
	parryAABB_.max = { 1.0f, 1.0f, 1.0f };
	parryCollision_.SetFollow(player_->GetWorldTF());
	parryCollision_.isActive = false;
	parryCollision_.worldTF.translation = { 0.0f, 1.0f, 0.0f };
	parryCollision_.mask.SetBelongFrag(GameMask::GetParry());
	parryCollision_.mask.SetHitFrag(GameMask::GetAttack());

	// 攻撃判定生成
	sheathCollision_.SetFollow(player_->GetWorldTF());
	sheathCollision_.isActive = false;
	sheathCollision_.worldTF.translation = { 0.0f, 1.0f, 0.0f };
	sheathCollision_.mask.SetBelongFrag(GameMask::GetAttack());
	sheathCollision_.mask.SetHitFrag(GameMask::GetEnemy());

#pragma region json用
	// 被弾機能
	damageResponse_ = std::make_unique<DamageResponse>(pCamera_, player_);
	damageResponse_->CreateJsonFIle();
	damageResponse_->Initialize();
	systems_.push_back(damageResponse_.get());
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
	// 攻撃機能
	attackSystem_ = std::make_unique<Attack>(pCamera_, player_, enemyManager_);
	attackSystem_->CreateJsonFIle();
	attackSystem_->Initialize();
	attackSystem_->SetLockOnSystem(lockOnSystem_.get());
	systems_.push_back(attackSystem_.get());
#pragma endregion

	// 移動機能をセット
	CreateMoveSystem(currentSystem_);
	systemState_ = SystemState::kMove;
}

void SystemManager::Update() {
	// ロックオン機能
	lockOnSystem_->Update();

	// 現在稼働しているシステムの更新
	CurrentSystemUpdate();

	// 機能の切り替え条件
	SwitchCurrentSystem();

	// 鞘機能(ダメージ中は何もしない)
	if (systemState_ != SystemState::kDamage) {
		sheathSystem_->Update();
		if (sheathSystem_->GetIsActive() && sheathSystem_->GetSheathState()->GetStateName() != "SwordDrawn") {
			// 速度
			velocity_ = sheathSystem_->GetVelocity();
			// 角度
			radian_ = sheathSystem_->GetRadian();
			quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
		}
	}

	// 各機能のクールタイムの処理
	coolTimer_->Update();

	// 無敵処理
	if (invinsibleTime_ != 0.0f) {
		invinsibleTime_--;
	}
}

void SystemManager::Reset() {
	sheathSystem_->Reset();
}

void SystemManager::DebugGUI() {
#ifdef _DEBUG
	// ロックオン
	lockOnSystem_->DebugGUI();
	// 鞘
	sheathSystem_->DebugGUI();

	// 各機能
	for (ISystem* system : systems_) {
		system->DebugGUI();
	}

	// 当たり判定
	if (ImGui::TreeNode("Collider")) {
		if (ImGui::TreeNode("Parry")) {
			parryCollision_.DebugGUI();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("SheathAttack")) {
			sheathCollision_.DebugGUI();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	ImGui::DragFloat3("Velocity", &velocity_.x);
	ImGui::DragFloat3("Radian", &radian_.x);
	ImGui::DragFloat("InvinsibleTime", &invinsibleTime_);

	if (ImGui::Button("Take Damage")) {
		player_->TakeDamage(1.0f);
	}
#endif // DEBUG
}

void SystemManager::CreateMoveSystem(ISystem*& system) {
	// 現在のシステムを一度削除
	if (system) { delete system; }

	// 移動機能
	Move* moveSystem = new Move(pCamera_, player_);
	moveSystem->SetJsonData(moveSystem_->GetJsonData());
	moveSystem->Initialize();
	moveSystem->Command();
	moveSystem->SetRotate(radian_);
	moveSystem->SetRotate(quat_);
	moveSystem->Update();

	system = moveSystem;
}

void SystemManager::CreateAttackSystem(ISystem*& system) {
	// 現在のシステムを一度削除
	if (system) { delete system; }

	// 攻撃機能
	Attack* attackSystem = new Attack(pCamera_, player_, enemyManager_);
	attackSystem->SetLockOnSystem(lockOnSystem_.get());
	attackSystem->SetJsonData(attackSystem_->GetJsonData());
	attackSystem->Initialize();
	attackSystem->Command();

	system = attackSystem;
}

void SystemManager::CreateEvasionSystem(ISystem*& system) {
	// 現在のシステムを一度削除
	if (system) { delete system; }

	// 回避機能
	Evasion* evasionSystem = new Evasion(pCamera_, player_);
	evasionSystem->SetJsonData(evasionSystem_->GetJsonData());
	evasionSystem->Initialize();
	evasionSystem->Command();

	system = evasionSystem;
}

void SystemManager::CreateParrySystem(ISystem*& system) {
	// 現在のシステムを一度削除
	if (system) { delete system; }

	// パリィ機能
	Parry* parrySystem = new Parry(pCamera_, player_);
	parrySystem->SetJsonData(parrySystem_->GetJsonData());
	parrySystem->Initialize();
	parrySystem->Command();

	system = parrySystem;
}

void SystemManager::CreateSheathSystem(ISystem*& system) {
	// 現在のシステムを一度削除
	if (system) { delete system; }

	// 鞘機能
	Sheath* sheathSystem = new Sheath(pCamera_, player_);
	sheathSystem->SetJsonData(sheathSystem_->GetJsonData());
	sheathSystem->Initialize();
	sheathSystem->Command();

	system = sheathSystem;
}

void SystemManager::CreateDamageResponseSystem(ISystem*& system) {
	// 現在のシステムを一度削除
	if (system) { delete system; }

	// 鞘機能
	DamageResponse* damageResponse = new DamageResponse(pCamera_, player_);
	damageResponse->SetJsonData(damageResponse_->GetJsonData());
	damageResponse->Initialize();
	// 無敵開始
	damageResponse->StartInvinsible();
	// 被弾演出開始
	damageResponse->StartEffect();
	// コンボ状態リセット
	ComboReset();

	system = damageResponse;
	systemState_ = SystemState::kDamage;
}

void SystemManager::CurrentSystemUpdate() {
	if (!currentSystem_) { return; }

	// 現在稼働している機能
	currentSystem_->Update();

	// 速度
	velocity_ = currentSystem_->GetVelocity();
	// 角度
	radian_ = currentSystem_->GetRadian();
	quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
}

void SystemManager::SwitchCurrentSystem() {
	// 鞘機能が稼働しているときは何もしない
	if (sheathSystem_->GetIsActive() && sheathSystem_->GetSheathState()->GetStateName() != "SwordDrawn") { return; }

	// 現在のシステムに何も入ってないなら移動機能を入れる
	if (!currentSystem_) {
		CreateMoveSystem(currentSystem_);
		systemState_ = SystemState::kMove;
		// コンボ初期化
		ComboReset();
	}

	// 攻撃
	if (currentSystem_->GetNextSystem(SystemState::kAttack) && systemState_ != SystemState::kAttack) {
		CreateAttackSystem(currentSystem_);
		systemState_ = SystemState::kAttack;
	}
	// 回避
	else if (currentSystem_->GetNextSystem(SystemState::kEvasion) && systemState_ != SystemState::kEvasion && coolTimer_->GetEvasionCoolTimeData().isFinish) {
		CreateEvasionSystem(currentSystem_);
		systemState_ = SystemState::kEvasion;
		// コンボ初期化
		ComboReset();
	}
	// パリィ
	else if (currentSystem_->GetNextSystem(SystemState::kParry) && systemState_ != SystemState::kParry && coolTimer_->GetParryCoolTimeData().isFinish) {
		CreateParrySystem(currentSystem_);
		systemState_ = SystemState::kParry;
		// コンボ初期化
		ComboReset();
	}
	// 鞘
	else if (currentSystem_->GetNextSystem(SystemState::kSheath) && systemState_ != SystemState::kSheath && coolTimer_->GetSheathCoolTimeData().isFinish) {
		sheathSystem_->Command();
		systemState_ = SystemState::kSheath;
		// コンボ初期化
		ComboReset();

		delete currentSystem_;
		currentSystem_ = nullptr;
	}
	// 移動
	else if (currentSystem_->GetNextSystem(SystemState::kMove) && systemState_ != SystemState::kMove) {
		CreateMoveSystem(currentSystem_);
		systemState_ = SystemState::kMove;
		// コンボ初期化
		ComboReset();
	}
}