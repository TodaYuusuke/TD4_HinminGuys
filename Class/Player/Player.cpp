#include "Player.h"

Player::Player(LWP::Object::Camera* camera) { 
	pCamera_ = camera; 

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

	// 速度を加算
	model_.worldTF.translation += moveSystem_->GetMoveVel();
	// 角度を加算
	model_.worldTF.rotation = moveSystem_->GetRotate();

	DebugGui();
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
	moveSystem_ = std::make_unique<Move>(pCamera_);
	moveSystem_->Initialize();
	// パリィ機能
	parrySystem_ = std::make_unique<Parry>(pCamera_);
	parrySystem_->Initialize();
	// 攻撃機能
	attackSystem_ = std::make_unique<Attack>(pCamera_);
	attackSystem_->Initialize();
}

void Player::DebugGui() {
#ifdef _DEBUG
	ImGui::Begin("Player");
	parrySystem_->DebugGui();
	ImGui::End();
#endif // DEBUG
}
