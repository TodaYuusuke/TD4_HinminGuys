#include "IEnemy.h"
#include "../Player/Player.h"
#include "IEnemyState.h"

//実体宣言
uint32_t IEnemy::currentEnemyID_ = 0;

IEnemy::IEnemy()
{
	//IDをセットして、ナンバーを加算
	ID_ = currentEnemyID_;
	currentEnemyID_++;
}

IEnemy::~IEnemy()
{
	//ナンバー減算
	currentEnemyID_--;
}

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
