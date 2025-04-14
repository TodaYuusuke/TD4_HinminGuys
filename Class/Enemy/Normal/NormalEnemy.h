#pragma once
#include "../IEnemy.h"
#include "../IEnemyState.h"

/// <summary>
/// 雑魚敵クラス
/// </summary>
class NormalEnemy : public IEnemy
{
public:
	NormalEnemy() = default;
	~NormalEnemy() override = default;

	void Initialize(Player* player, const Vector3& position) override;

	void Update() override;

	void SetState(std::unique_ptr<IEnemyState> state);

private:
	//状態
	std::unique_ptr<IEnemyState> state_;

};


