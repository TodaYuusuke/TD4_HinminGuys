#pragma once
#include "../../IEnemy.h"
#include "../../IEnemyState.h"

/// <summary>
/// ボス敵クラス
/// </summary>
class Boss : public IEnemy
{
public:
	Boss() = default;
	~Boss() override = default;

	void Initialize(Player* player, const Vector3& position) override;

	void Update() override;

private:

};


