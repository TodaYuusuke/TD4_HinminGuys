#include "WaitingForAttack.h"
#include "NormalAttack.h"
#include "../../IEnemy.h"

uint16_t WaitingForAttack::attackCount_ = 0;
uint16_t WaitingForAttack::nextAttackCount_ = 0;

WaitingForAttack::WaitingForAttack()
{
	//現在の攻撃カウントから順番を決める
	attackID_ = attackCount_;
	//攻撃の順番を決める数字を上昇させる
	attackCount_++;

}

WaitingForAttack::~WaitingForAttack()
{

	//次攻撃する番号を上昇させる
	nextAttackCount_++;

}

void WaitingForAttack::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
	enemy_->SetAnimation("Idle", true);

	//ランダムな数字を利用して右回りかどうかを決める
	if (LWP::Utility::GenerateRandamNum(0, 1) == 0) {
		isClockwise_ = true;
	}

}

void WaitingForAttack::Update()
{

	//
	//一部間合いを取るクラスのコピペをしているので後々修正する
	//

	//誰も攻撃しておらず、順番が回ってきたら攻撃に移行
	if (not enemy_->GetIsAttack() and attackID_ == nextAttackCount_) {
		//攻撃状態に移行
		enemy_->SetState(new NormalAttack());
		return;
	}

	//プレイヤーが存在する場合
	if (enemy_->GetPlayer()) {

		//移動
		Vector3 dist = enemy_->GetPlayerPosition() - enemy_->GetPosition();

		//y軸の移動ベクトルを消す
		dist.y = 0.0f;
		//距離
		float length = dist.Length();
		//正規化
		dist = dist.Normalize();

		//方向ベクトル
		Vector3 direction = dist * -1.0f;

		//角度を求める
		float theta = std::acosf(Vector3::Dot({ 1.0f,0.0f,0.0f }, direction));
		//外積を求めて正負判定(direction.zの値がそのまま正負になる)
		if (direction.z < 0.0f) {
			theta *= -1.0f;
		}

		//円周を沿うような移動ベクトルにする
		Vector3 result{};

		result.x = -sinf(theta);
		result.z = cosf(theta);

		//右回りならベクトルを逆にする
		if (isClockwise_) {
			result *= -1.0f;
		}

		enemy_->SetPosition(enemy_->GetPosition() + result * LWP::Info::GetDeltaTime() + (enemy_->GetRepulsiveForce() * LWP::Info::GetDeltaTime()));
		//プレイヤーの向きに回転
		enemy_->RotateTowardsPlayer();

	}

}
