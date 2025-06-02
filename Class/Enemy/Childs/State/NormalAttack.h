#pragma once
#include "../../IEnemyState.h"

/// <summary>
/// 雑魚敵攻撃状態クラス
/// </summary>
class NormalAttack : public IEnemyState
{
public:

	NormalAttack();

	~NormalAttack() override = default;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

	static void DebugGUI();

private:

	LWP::Object::TransformQuat attackTF{};
	//攻撃コライダー半径
	float attackRadius_ = 0.1f;
	//

};

