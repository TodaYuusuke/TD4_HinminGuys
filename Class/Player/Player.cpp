#include "Player.h"

void Player::Initialize() {
	// モデルを読み込む
	model_.LoadCube();

	// 自機機能を生成
	CreateSystems();
}

void Player::Update() {
	// 移動機能
	moveSystem_->Update();
	// パリィ機能
	parrySystem_->Update();
}

void Player::Reset() {
	// 移動機能
	moveSystem_->Reset();
	// パリィ機能
	parrySystem_->Reset();
}

void Player::CreateSystems() {
	// 移動機能
	moveSystem_ = std::make_unique<MoveSystem>();
	moveSystem_->Initialize();
	// パリィ機能
	parrySystem_ = std::make_unique<ParrySystem>();
	parrySystem_->Initialize();
}
