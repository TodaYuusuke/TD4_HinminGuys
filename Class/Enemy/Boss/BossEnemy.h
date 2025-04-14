#pragma once
#include "../IEnemy.h"
#include "../IEnemyState.h"

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

	void SetState(std::unique_ptr<IEnemyState> state);

private:
	//状態
	std::unique_ptr<IEnemyState> state_;

};


