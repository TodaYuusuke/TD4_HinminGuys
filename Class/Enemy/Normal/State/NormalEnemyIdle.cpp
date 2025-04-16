#include "NormalEnemyIdle.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"

void NormalEnemyIdle::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
}

void NormalEnemyIdle::Update()
{

	//プレイヤーが存在する場合
	if (enemy_->GetPlayer()) {

	}

}
