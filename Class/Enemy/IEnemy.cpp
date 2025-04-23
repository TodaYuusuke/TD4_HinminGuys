#include "IEnemy.h"
#include "../Player/Player.h"
#include "IEnemyState.h"
#define UNIT16_MAX 65535

//実体宣言
uint16_t IEnemy::currentEnemyID_ = 0;

IEnemy::IEnemy()
{
	//IDをセット
	ID_ = currentEnemyID_;

	//最大値になったら0にリセット
	if (currentEnemyID_ >= UNIT16_MAX) {
		currentEnemyID_ = 0;
	}
	//カウント増加
	else {
		currentEnemyID_++;
	}

}

IEnemy::~IEnemy()
{
	
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

void IEnemy::Debug()
{

	if (ImGui::TreeNode(std::to_string(ID_).c_str())) {
		ImGui::Text(std::to_string(distFromPlayer_).c_str());
		ImGui::TreePop();
	}

}
