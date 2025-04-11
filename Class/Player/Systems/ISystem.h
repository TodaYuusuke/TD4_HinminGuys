#pragma once
#include "../Adapter/Adapter.h"

/// <summary>
/// ���@�̋@�\�N���X�̊��N���X
/// </summary>
class ISystem {
public:
	// �R���X�g���N�^
	ISystem() = default;
	// �f�X�g���N�^
	virtual ~ISystem() = 0;

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �S�Ă̐��l�����Z�b�g
	/// </summary>
	virtual void Reset() = 0;

protected:

};