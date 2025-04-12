#include "BossEnemy.h"

void BossEnemy::Initialize(Player* player, const Vector3& position)
{
	model_.LoadCube();
	SetPlayer(player);
	position_ = position;
}

void BossEnemy::Update()
{
}
