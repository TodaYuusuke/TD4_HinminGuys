#include "BossEnemy.h"

void BossEnemy::Initialize(Player* player, const Vector3& position)
{
	model_.LoadCube();
	SetPlayer(player);
	position_ = position;
}

void BossEnemy::Update()
{

	//デルタタイムが0.0f以下の時、更新しない
	if (LWP::Info::GetDeltaTime() <= 0.0f) {
		return;
	}

}

void BossEnemy::SetState(std::unique_ptr<IEnemyState> state)
{
	//前回の状態を開放、新しい状態に置き換える
	state_.release();
	state_ = std::move(state);
	//初期化
	state_->Initialize(this);
}
