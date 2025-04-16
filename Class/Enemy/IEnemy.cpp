#include "IEnemy.h"
#include "../Player/Player.h"
#include "IEnemyState.h"

Vector3 IEnemy::GetPlayerPosition()
{
    return *player_->GetModelPos();
}

void IEnemy::SetAnimation(const std::string& animName, bool isLoop)
{
    animation_.Play(animName, isLoop);
}

void IEnemy::SetState(std::unique_ptr<IEnemyState> state)
{
	//前回の状態を開放、新しい状態に置き換える
	state_.release();
	state_ = std::move(state);
	//初期化
	state_->Initialize(this);
}
