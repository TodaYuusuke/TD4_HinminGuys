#pragma once
#include "../IEnemy.h"

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

private:

};


