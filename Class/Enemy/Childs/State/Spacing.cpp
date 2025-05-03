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

		//プレイヤーに向かうベクトルとそれをクォータニオンを利用して90度回転させたベクトルの中間ベクトルを作る
		Vector3 result{};
		result.x = -sinf(theta);
		result.z = cosf(theta);

		//プレイヤーとの間合いをあらかじめ決めておき、その範囲内に入ったら押し出しベクトルを加算するようにする
		if (length < approachDist_ && length > 0.0001f) {
			enemy_->AddRepulsiveForce(dist.Normalize() * -((approachDist_ - length) / approachDist_));
		}

		//敵の向きをプレイヤーに向かせるため、ここで向きを保存
		Vector3 to = dist.Normalize();
		dist = dist.Normalize() * LWP::Info::GetDeltaTime();
		
		enemy_->SetPosition(enemy_->GetPosition() + result * LWP::Info::GetDeltaTime() + (enemy_->GetRepulsiveForce() * LWP::Info::GetDeltaTime()));

		//プレイヤーの方向を向く
		if (dist.Length() != 0.0f) {
			//敵の基本姿勢
			Vector3 from = { 0.0f,0.0f,1.0f };

			// 回転軸をクロス積から求める
			Vector3 axis = Vector3::Cross(from, to);
			// 内積
			float dot = Vector3::Dot(from, to);
			// 完全に平行な場合、単位クォータニオンを返す
			if (dot > 0.9999f) {

				//行きたい方向のQuaternionの作成
				enemy_->SetRotation(Quaternion{ 0.0f,0.0f,0.0f,1.0f });

			}
			else {

				//逆向きのベクトルだった場合、垂直なベクトルを一つ選ぶ
				if (dot <= -1.0f) {

					if (from.x != 0.0f or from.y != 0.0f) {

						axis = { from.y, -from.x,0.0f };
						axis = axis.Normalize();
					}
					else if (from.x != 0.0f or from.z != 0.0f) {

						axis = { 0.0f, -from.z, from.x };
						axis = axis.Normalize();

					}

				}
				else {
					axis = Vector3::Cross(from, to).Normalize();
				}

				// θを求める
				float theta = std::acos(Vector3::Dot(from, to) / (from.Length() * to.Length()));

				//行きたい方向のQuaternionの作成
				enemy_->SetRotation(Quaternion::CreateFromAxisAngle(axis, theta));

			}

		}

	}

	

}
