#pragma once
#include "../../IEnemyState.h"

/// <summary>
/// 雑魚敵攻撃状態クラス
/// </summary>
class NormalAttack : public IEnemyState
{
public:

	~NormalAttack() override = default;

	void Initialize(IEnemy* enemy) override;

	void Update() override;

private:

	LWP::Object::TransformQuat attackTF{};

};

