#include "IEnemy.h"
#include "../Player/Player.h"
#include "../GameMask.h"

#define UNIT16_MAX 65535

using namespace GameMask;

//実体宣言
uint16_t IEnemy::currentEnemyID_ = 0;
uint16_t IEnemy::maxAttackCount_ = 3;
std::array<const char*, int(EnemyType::kMax)> IEnemy::enemyTypeName =
{ "Saiji",  "OniHayha", "Ogre" };

IEnemy::IEnemy()
	: aabb_(collider_.SetBroadShape(LWP::Object::Collider::AABB()))
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

	//画像リセット
	for (int32_t i = 0; i < kMaxParryEffect_; i++) {
		parryEffectSprite_[i].LoadTexture("Effect/ParryFlash.png");
		parryEffectSprite_[i].anchorPoint = { 0.5f,0.5f };
		parryEffectSprite_[i].isActive = false;

		if (i == 1) {
			parryEffectSprite_[i].worldTF.rotation = LWP::Math::Quaternion::ConvertEuler({ 0.0f,0.0f,1.57f });
		}

		parryEffectSprite_[i].Init();
	}

}

IEnemy::~IEnemy()
{
	
	

}

void IEnemy::Update()
{

}

Vector3 IEnemy::GetPlayerPosition()
{
    return *player_->GetModelPos();
}

void IEnemy::SetAnimation(const std::string& animName, bool isLoop, float speed)
{
    animation_.Play(animName);
	animation_.Loop(isLoop);
	animation_.GetPlayBackSpeed() = speed;
}

void IEnemy::RotateTowardsPlayer()
{

	//プレイヤーへの向き
	Vector3 direction = GetPlayerPosition() - GetPosition();

	//プレイヤーの方向を向く
	if (direction.Length() != 0.0f) {
		//敵の基本姿勢
		Vector3 from = { 0.0f,0.0f,1.0f };
		//向ける方向
		Vector3 to = direction.Normalize();

		// 回転軸をクロス積から求める
		Vector3 axis = Vector3::Cross(from, to);
		// 内積
		float dot = Vector3::Dot(from, to);
		// 完全に平行な場合、単位クォータニオンを返す
		if (dot > 0.9999f) {

			//行きたい方向のQuaternionの作成
			SetRotation(Quaternion{ 0.0f,0.0f,0.0f,1.0f });

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
			SetRotation(Quaternion::CreateFromAxisAngle(axis, theta));

		}

	}

}

void IEnemy::StartParryEffect(const Vector3& position)
{
	parryEffectPosition_ = position;
	//パリィエフェクトのフラグ開始
	isStartParryEffect_ = true;
	//ワールド座標からスクリーン座標に変換
	LWP::Math::Matrix4x4 viewProjectionViewport =
		camera_->GetViewProjection() *
		LWP::Math::Matrix4x4::CreateViewportMatrix(0, 0, LWP::Info::GetWindowWidthF(), LWP::Info::GetWindowHeightF(), 0.0f, 1.0f);
	//画像を表示させる
	for (int32_t i = 0; i < kMaxParryEffect_; i++) {
		parryEffectSprite_[i].isActive = true;
		parryEffectSprite_[i].worldTF.translation = parryEffectPosition_ * viewProjectionViewport;
	}

	parryEffectTime_ = 0.0f;

}

void IEnemy::SetKnockBackValue(const float& knockBackValue)
{

	//プレイヤーから自身へのベクトルを作る
	knockBackVelocity_ = GetPosition() - GetPlayerPosition();
	//正規化
	knockBackVelocity_ = knockBackVelocity_.Normalize() * knockBackValue;

}

void IEnemy::UpdateParryEffect()
{

	if (parryEffectTime_ < maxParryEffectTime_) {
		//時間加算
		parryEffectTime_ += 1.0f * LWP::Info::GetDeltaTimeF();

		//時間を超過したらエフェクト終了
		if (parryEffectTime_ >= maxParryEffectTime_) {
			parryEffectTime_ = maxParryEffectTime_;
			isStartParryEffect_ = false;
			for (int32_t i = 0; i < kMaxParryEffect_; i++) {
				parryEffectSprite_[i].isActive = false;
			}
			return;
		}

		//ワールド座標からスクリーン座標に変換
		LWP::Math::Matrix4x4 viewProjectionViewport =
			camera_->GetViewProjection() *
			LWP::Math::Matrix4x4::CreateViewportMatrix(0, 0, LWP::Info::GetWindowWidthF(), LWP::Info::GetWindowHeightF(), 0.0f, 1.0f);
		
		float t = LWP::Utility::Easing::CallFunction(LWP::Utility::Easing::Type::Liner, parryEffectTime_ / maxParryEffectTime_);
		
		//画像更新
		for (int32_t i = 0; i < kMaxParryEffect_; i++) {
			parryEffectSprite_[i].worldTF.scale = LWP::Utility::Interpolation::Lerp({0.0f,1.0f,0.0f}, {100.0f,0.0f,0.0f}, t);
			parryEffectSprite_[i].worldTF.translation = parryEffectPosition_ * viewProjectionViewport;
		}

	}

}

LWP::Math::Vector3 IEnemy::CoordTransform(const LWP::Math::Vector3& vector, const LWP::Math::Matrix4x4& matrix) {

	LWP::Math::Vector3 result = {};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] +
		vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] +
		vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] +
		vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] +
		vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	if (w == 0.0f) {
		return LWP::Math::Vector3(0.0f, 0.0f, 0.0f);
	}

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;

}
