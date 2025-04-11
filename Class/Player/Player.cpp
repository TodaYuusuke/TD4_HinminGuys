#include "Player.h"

void Player::Initialize() {
	// ���f����ǂݍ���
	model_.LoadCube();

	// ���@�@�\�𐶐�
	CreateSystems();
}

void Player::Update() {
	// �ړ��@�\
	moveSystem_->Update();
	// �p���B�@�\
	parrySystem_->Update();
}

void Player::Reset() {
	// �ړ��@�\
	moveSystem_->Reset();
	// �p���B�@�\
	parrySystem_->Reset();
}

void Player::CreateSystems() {
	// �ړ��@�\
	moveSystem_ = std::make_unique<MoveSystem>();
	moveSystem_->Initialize();
	// �p���B�@�\
	parrySystem_ = std::make_unique<ParrySystem>();
	parrySystem_->Initialize();
}
