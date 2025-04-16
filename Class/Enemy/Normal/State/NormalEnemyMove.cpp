#include "NormalEnemyMove.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"

void NormalEnemyMove::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
}

void NormalEnemyMove::Update()
{

	//プレイヤーが存在する場合
	if (enemy_->GetPlayer()) {

	}

}
