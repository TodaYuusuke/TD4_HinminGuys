#include "NormalEnemy.h"
#include "../../Player/Player.h"

void NormalEnemy::Initialize(Player* player, const Vector3& position)
{
	model_.LoadCube();
	SetPlayer(player);
	position_ = position;
}

void NormalEnemy::Update()
{

	//デルタタイムが0.0f以下の時、更新しない
	if (LWP::Info::GetDeltaTime() <= 0.0f) {
		return;
	}

	//現在の状態を更新
	state_->Update();

}

void NormalEnemy::SetState(std::unique_ptr<IEnemyState> state)
{
	//前回の状態を開放、新しい状態に置き換える
	state_.release();
	state_ = std::move(state);
	//初期化
	state_->Initialize(this);
}
