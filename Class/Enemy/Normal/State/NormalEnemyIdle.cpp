#include "NormalEnemyIdle.h"
#include "NormalEnemyMove.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"

void NormalEnemyIdle::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
	enemy_->SetAnimation("Idle", true);
	//待機時間
	standTime_ = 60;
}

void NormalEnemyIdle::Update()
{

	//プレイヤーが存在する場合
	if (enemy_->GetPlayer()) {
		//カウントダウン
		if (standTime_ > 0) {
			standTime_--;
		}

		//0になったら状態切り替え
		if (standTime_ <= 0) {

			//攻撃状態に移行
			enemy_->SetState(std::make_unique<NormalEnemyMove>());

		}

	}

}
