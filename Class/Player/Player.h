#pragma once
#include "ICharacter.h"
#include "Systems/MoveSystem.h"
#include "Systems/ParrySystem.h"
#include <memory>

class Player : public ICharacter {
public:
	// �R���X�g���N�^
	Player() = default;
	// �f�X�g���N�^
	~Player() override = default;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;

	/// <summary>
	/// �S�Ă̐��l�����Z�b�g����
	/// </summary>
	void Reset();

private:
	/// <summary>
	/// ���@�@�\��S�Đ���
	/// </summary>
	void CreateSystems();

private:
	// �ړ��@�\
	std::unique_ptr<MoveSystem> moveSystem_;
	// �p���B�@�\
	std::unique_ptr<ParrySystem> parrySystem_;
};