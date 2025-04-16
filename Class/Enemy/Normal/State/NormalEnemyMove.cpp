#include "NormalEnemyMove.h"
#include "NormalEnemyAttack.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"

void NormalEnemyMove::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
	enemy_->SetAnimation("Run", true);
	//走り時間
	runTime_ = 60;
}

void NormalEnemyMove::Update()
{

	//プレイヤーが存在する場合
	if (enemy_->GetPlayer()) {

		//カウントダウン
		if (runTime_ > 0) {
			runTime_--;
		}

		//0になったら状態切り替え
		if (runTime_ <= 0) {

			//攻撃状態に移行
			enemy_->SetState(std::make_unique<NormalEnemyAttack>());

		}

		//プレイヤーの方向を向く
		

	}

}
