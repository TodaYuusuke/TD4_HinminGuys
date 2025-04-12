#pragma once
#include "../IEnemy.h"

/// <summary>
/// ボス敵クラス
/// </summary>
class BossEnemy : public IEnemy
{
public:
	BossEnemy() = default;
	~BossEnemy() override = default;

	void Initialize(Player* player, const Vector3& position) override;

	void Update() override;

private:

};


