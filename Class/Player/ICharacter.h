#pragma once
#include "../Adapter/Adapter.h"

using namespace LWP;
using namespace LWP::Math;

class ICharacter {
public:
	// �R���X�g���N�^
	ICharacter() = default;
	// �f�X�g���N�^
	virtual ~ICharacter() = default;

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() = 0;

protected:
	// ���f��
	LWP::Resource::RigidModel model_;
};