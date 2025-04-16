#include "Player.h"

void Player::Initialize() {
	// モデルを読み込む
	//model_.LoadCube();

	model_.LoadShortPath("player/Player_Simple.gltf");

	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.5f, 0.5f, 0.5f };

	// 自機機能を生成
	CreateSystems();
}

void Player::Update() {
	// 移動機能
	moveSystem_->Update();
	// パリィ機能
	parrySystem_->Update();
	// 攻撃機能
	attackSystem_->Update();

	// 速度を加算
	model_.worldTF.translation += moveSystem_->GetMoveVel();
	// 角度を加算
	model_.worldTF.rotation = moveSystem_->GetRotate();
}

void Player::Reset() {
	// 移動機能
	moveSystem_->Reset();
	// パリィ機能
	parrySystem_->Reset();
	// 攻撃機能
	attackSystem_->Reset();
}

void Player::CreateSystems() {
	// 移動機能
	moveSystem_ = std::make_unique<MoveSystem>();
	moveSystem_->Initialize();
	// パリィ機能
	parrySystem_ = std::make_unique<ParrySystem>();
	parrySystem_->Initialize();
	// 攻撃機能
	attackSystem_ = std::make_unique<AttackSystem>();
	attackSystem_->Initialize();
}

void Player::DebugWindow() {
#ifdef _DEBUG
	//ImGui::Begin("Player");
	//ImGui::DragFloat3()
	//ImGui::End();
#endif // DEBUG
}
