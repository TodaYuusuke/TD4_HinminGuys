#pragma once
#include "ISystem.h"

/// <summary>
/// ���@�̈ړ��@�\���܂Ƃ߂��N���X
/// </summary>
class MoveSystem : public ISystem {
public:
	// �R���X�g���N�^
	MoveSystem() = default;
	// �f�X�g���N�^
	~MoveSystem() override = default;

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

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// �ړ����x���擾
	/// </summary>
	/// <returns></returns>
	Vector3 GetMoveVel() { return moveVel_; }
	/// <summary>
	/// �����Ă���������擾
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetRotate() { return rotate_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// �ړ����x��ݒ�
	/// </summary>
	/// <param name="moveVel">�ړ����x</param>
	void SetMoveVel(const LWP::Math::Vector3& moveVel) { moveVel_ = moveVel; }
	/// <summary>
	/// �����Ă��������ݒ�
	/// </summary>
	/// <param name="rotate">�����������</param>
	void SetRotate(const LWP::Math::Quaternion& rotate) { rotate_ = rotate; }
#pragma endregion

private:// �v���C�x�[�g�ȕϐ�
	// �ړ����x
	LWP::Math::Vector3 moveVel_;
	// �����Ă���p�x
	LWP::Math::Quaternion rotate_;
};