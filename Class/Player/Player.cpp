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
	// 各機能
	systemManager_->Update();

	// 速度を加算
	model_.worldTF.translation += systemManager_->GetVelocity() * LWP::Info::GetDeltaTime();
	// 角度を加算
	model_.worldTF.rotation = systemManager_->GetRotate();

	// ImGui
	DebugGUI();
}

void Player::Reset() {
	systemManager_->Reset();
}

void Player::CreateSystems() {
	systemManager_ = std::make_unique<SystemManager>(this, enemyManager_, followCamera_, pCamera_);
	systemManager_->Initialize();
}

void Player::DebugGUI() {
#ifdef _DEBUG
	ImGui::Begin("Player");
	
	systemManager_->DebugGUI();
	ImGui::DragFloat3("Translation", &model_.worldTF.translation.x, 0.1f, -10000, 10000);
	ImGui::DragFloat4("Quaternion", &model_.worldTF.rotation.x, 0.1f, -10000, 10000);

	ImGui::End();
#endif // DEBUG
}
