#pragma once
#include "../IAttackSystemState.h"

class NoneAttack : public IAttackSystemState {
public:
	// コンストラクタ
	NoneAttack(Attack* attackSystem);
	// デストラクタ
	~NoneAttack() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:

};