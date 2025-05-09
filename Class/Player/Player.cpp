#include "Player.h"
#include "../Enemy/EnemyManager.h"

Player::Player(LWP::Object::Camera* camera, EnemyManager* enemyManager, FollowCamera* followCamera) {
	pCamera_ = camera; 
	enemyManager_ = enemyManager;
	followCamera_ = followCamera;

	// モデルを読み込む
	model_.LoadShortPath("player/Player_Simple.gltf");
	animation_.LoadFullPath("resources/model/player/Player_Simple.gltf", &model_);

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

	// アニメーション
	animation_.Update();

	// 速度を加算
	model_.worldTF.translation += systemManager_->GetVelocity();
	// 角度を代入
	model_.worldTF.rotation = systemManager_->GetRotate();
}

void Player::Reset() {
	// 各機能
	systemManager_->Reset();
}

void Player::DebugGUI() {
#ifdef _DEBUG
	// 各機能
	systemManager_->DebugGUI();
	// アニメーション
	if (ImGui::TreeNode("Animation")) {
		animation_.DebugGUI();
		ImGui::TreePop();
	}

	ImGui::DragFloat3("Translation", &model_.worldTF.translation.x, 0.1f, -10000, 10000);
	ImGui::DragFloat4("Quaternion", &model_.worldTF.rotation.x, 0.1f, -10000, 10000);
#endif // DEBUG
}

void Player::CreateSystems() {
	// 各機能生成
	systemManager_ = std::make_unique<SystemManager>(this, enemyManager_, followCamera_, pCamera_);
	systemManager_->Initialize();
}