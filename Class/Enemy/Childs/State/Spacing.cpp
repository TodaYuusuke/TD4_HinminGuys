#include "Spacing.h"
#include "../../IEnemy.h"

Spacing::Spacing()
{
}

Spacing::~Spacing()
{
}

void Spacing::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
	enemy_->SetAnimation("Run", true);

	//ランダムな数字を利用して右回りかどうかを決める
	if (LWP::Utility::GenerateRandamNum(0, 1) == 0) {
		isClockwise_ = true;
	}

}

void Spacing::Update()
{

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

		//プレイヤーとの間合いをあらかじめ決めておき、その範囲内に入ったら押し出しベクトルを加算するようにする
		if (length < approachDist_ && length > 0.0001f) {
			enemy_->AddRepulsiveForce(dist.Normalize() * -((approachDist_ - length) / approachDist_));
		}

		result = result + dist.Normalize();

		result = result.Normalize();

		enemy_->SetPosition(enemy_->GetPosition() + result * LWP::Info::GetDeltaTime() + (enemy_->GetRepulsiveForce() * LWP::Info::GetDeltaTime()));
		//プレイヤーの向きに回転
		enemy_->RotateTowardsPlayer();

	}

	

}
