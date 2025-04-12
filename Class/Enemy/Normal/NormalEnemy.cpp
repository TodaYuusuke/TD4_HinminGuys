#include "NormalEnemy.h"

void NormalEnemy::Initialize(Player* player, const Vector3& position)
{
	model_.LoadCube();
	SetPlayer(player);
	position_ = position;
}

void NormalEnemy::Update()
{
}
