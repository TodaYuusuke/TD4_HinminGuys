#pragma once
#include "ISystem.h"

/// <summary>
/// ���@�̃p���B�@�\���܂Ƃ߂��N���X
/// </summary>
class ParrySystem : public ISystem {
public:
	// �R���X�g���N�^
	ParrySystem() = default;
	// �f�X�g���N�^
	~ParrySystem() override = default;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;

	/// <summary>
	/// �S�Ă̐��l�����Z�b�g
	/// </summary>
	void Reset() override;

private:
};