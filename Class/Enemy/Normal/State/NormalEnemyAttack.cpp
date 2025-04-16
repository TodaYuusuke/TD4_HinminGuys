#include "NormalEnemyAttack.h"
#include "NormalEnemyIdle.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"

void NormalEnemyAttack::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
	enemy_->SetAnimation("Attack", false);
}

void NormalEnemyAttack::Update()
{

	//攻撃が終了した時
	if (not enemy_->GetAnimation()->GetPlaying()) {

		//待機状態に移行
		enemy_->SetState(std::make_unique<NormalEnemyIdle>());

	}

}
