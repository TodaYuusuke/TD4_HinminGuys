#include "Spacing.h"
#include "NormalMove.h"
#include "NormalIdle.h"
#include "../../IEnemy.h"

int32_t Spacing::spacingTime_ = 120;
float Spacing::spaceDist_ = 3.0f;

Spacing::Spacing()
{
}

Spacing::~Spacing()
{
}

void Spacing::Initialize(IEnemy* enemy)
{
	enemy_ = enemy;
	enemy_->SetAnimation("Run", true, 0.3f);
	//時間セット
	countSpacingTime_ = spacingTime_;
	//ランダムな数字を利用して右回りかどうかを決める
	if (LWP::Utility::GenerateRandamNum(0, 1) == 0) {
		isClockwise_ = true;
	}

}

void Spacing::Update()
{

	//カウントダウン
	if (countSpacingTime_ > 0) {
		countSpacingTime_--;
	}

	//0になったら行動変化
	if (countSpacingTime_ <= 0) {

		//攻撃人数が3人未満且つ敵の中で距離の近さが3位以内の時
		if (IEnemy::GetCurrentAttackCount() < IEnemy::GetMaxAttackCount() and
			enemy_->GetClosenessCount() < IEnemy::GetMaxAttackCount()) {
			//接近状態に移行
			enemy_->AddAttackCount();
			enemy_->SetState(new NormalMove());
			return;
		}
		//そうでない場合
		else {
			//待機状態に戻る
			enemy_->SetState(new NormalIdle());
			return;
		}

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

		//プレイヤーとの間合いをあらかじめ決めておき、その範囲内に入ったら押し出しベクトルを加算するようにする
		if (length < spaceDist_ && length > 0.0001f) {
			enemy_->AddRepulsiveForce(dist.Normalize() * -((spaceDist_ - length) * 2.0f / spaceDist_));
		}

		result = result + dist.Normalize();

		result = result.Normalize();

		enemy_->SetPosition(enemy_->GetPosition() + result * LWP::Info::GetDeltaTime() + (enemy_->GetRepulsiveForce() * LWP::Info::GetDeltaTime()));
		//プレイヤーの向きに回転
		enemy_->RotateTowardsPlayer();

	}

	

}

void Spacing::DebugGUI()
{

	if (ImGui::TreeNode("Spacing")) {
		ImGui::DragFloat("spaceDist", &spaceDist_, 0.1f);
		ImGui::DragInt("spacingTime", &spacingTime_, 0.2f);
		ImGui::TreePop();
	}

}
