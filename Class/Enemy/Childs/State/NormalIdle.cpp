#include "NormalIdle.h"
#include "NormalMove.h"
#include "Spacing.h"
#include "../../../Player/Player.h"
#include "../../IEnemy.h"

void NormalIdle::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
	enemy_->SetAnimation("Idle", true);
	//待機時間
	standTime_ = 60;
}

void NormalIdle::Update()
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
			enemy_->SetState(new NormalMove());

		}

	}

}
