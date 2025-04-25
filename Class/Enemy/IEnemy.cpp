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
	
	//ステートが存在していたら削除
	if (state_) {
		delete state_;
	}

}

void IEnemy::Update()
{

	//デルタタイムが0.0f以下の時、更新しない
	if (LWP::Info::GetDeltaTime() <= 0.0f) {
		return;
	}

	//現在の状態を更新
	state_->Update();

	//反発力リセット
	repulsiveForce_ = { 0.0f,0.0f,0.0f };

	//プレイヤーとの距離を計算
	Vector3 diff = GetPlayerPosition() - GetPosition();
	distFromPlayer_ = diff.Length();

}

Vector3 IEnemy::GetPlayerPosition()
{
    return *player_->GetModelPos();
}

void IEnemy::SetAnimation(const std::string& animName, bool isLoop)
{
    animation_.Play(animName, isLoop);
}

void IEnemy::SetState(IEnemyState* state)
{
	//前回の状態を開放、新しい状態に置き換える
	if (state_) {
		delete state_;
	}
	state_ = state;
	//初期化
	state_->Initialize(this);

}

void IEnemy::DebugGUI()
{

	if (ImGui::TreeNode(std::to_string(ID_).c_str())) {
		ImGui::Text(std::to_string(distFromPlayer_).c_str());
		ImGui::TreePop();
	}

}
