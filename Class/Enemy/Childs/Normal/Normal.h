#pragma once
#include "../../IEnemy.h"
#include "../../IEnemyState.h"

/// <summary>
/// 雑魚敵クラス
/// </summary>
class Normal : public IEnemy
{
public:
	Normal() = default;
	~Normal() override = default;

	void Initialize(Player* player, const Vector3& position) override;

	void Update() override;

private:
	

};


