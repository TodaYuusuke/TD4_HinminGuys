#include "Player.h"
#include "../Enemy/EnemyManager.h"

Player::Player(LWP::Object::Camera* camera, EnemyManager* enemyManager, FollowCamera* followCamera) {
	pCamera_ = camera; 
	enemyManager_ = enemyManager;
	followCamera_ = followCamera;

	// モデルを読み込む
	model_.LoadShortPath("player/Player_Simple.gltf");

	// 自機機能を生成
	CreateSystems();
}

void Player::Initialize() {
	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.5f, 0.5f, 0.5f };
}

void Player::Update() {
	// 移動機能
	moveSystem_->Update();
	// パリィ機能
	parrySystem_->Update();
	// 攻撃機能
	attackSystem_->Update();
	// ロックオン機能
	lockOnSystem_->Update();

	// 速度を加算
	model_.worldTF.translation += moveSystem_->GetMoveVel();
	// 角度を加算
	model_.worldTF.rotation = moveSystem_->GetRotate();

	// ImGui
	DebugGUI();
}

void Player::Reset() {
	// 移動機能
	moveSystem_->Reset();
	// パリィ機能
	parrySystem_->Reset();
	// 攻撃機能
	attackSystem_->Reset();
	// ロックオン機能
	lockOnSystem_->Reset();
}

void Player::CreateSystems() {
	// 移動機能
	moveSystem_ = std::make_unique<Move>(pCamera_, this);
	moveSystem_->Initialize();
	// パリィ機能
	parrySystem_ = std::make_unique<Parry>(pCamera_, this);
	parrySystem_->Initialize();
	// 攻撃機能
	attackSystem_ = std::make_unique<Attack>(pCamera_, this);
	attackSystem_->Initialize();
	// ロックオン機能
	lockOnSystem_ = std::make_unique<LockOn>(pCamera_, this);
	lockOnSystem_->Initialize();
	lockOnSystem_->SetEnemyList(enemyManager_->GetEnemyListPtr());
	lockOnSystem_->SetFollowCamera(followCamera_);
}

void Player::DebugGUI() {
#ifdef _DEBUG
	ImGui::Begin("Player");
	// パリィ
	parrySystem_->DebugGUI();
	// 攻撃
	attackSystem_->DebugGUI();
	// ロックオン
	lockOnSystem_->DebugGUI();

	ImGui::DragFloat3("Translation", &model_.worldTF.translation.x, 0.1f, -10000, 10000);
	ImGui::End();
#endif // DEBUG
}
