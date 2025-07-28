#include "EnemySpawnParticle.h"
#include "../../../Math/MathFunctions.h"
#include <numbers>

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

EnemySpawnParticle::EnemySpawnParticle(const std::string& texName) {
	texName_ = texName;
}

void EnemySpawnParticle::Update() {
	if (particleData_.currentTime >= particleData_.lifeTime) {
		particleData_.isAlive = false;
		return;
	}

	// パーティクル挙動
	UpdateParticle();

	// 生存時間
	particleData_.currentTime++;
}

void EnemySpawnParticle::Create(const LWP::Math::Vector3& pos) {
	// パーティクルの種類
	//particle.type = (int)ParticleType::kLine;

	// ビルボード生成
	plane_.anchorPoint = { 0.5f, 0.5f };
	plane_.material.texture = LWP::Resource::LoadTexture(texName_);

	// 速度
	Vector3 vel = LWP::Utility::Random::GenerateVector3(jsonData_.velLimit.min, jsonData_.velLimit.max);
	particleData_.vel = vel + jsonData_.firstVel;
	// 生存可能時間
	particleData_.lifeTime = jsonData_.maxElapseTime * 60.0f;
	particleData_.currentTime = 0;
	// 大きさ
	float scale = LWP::Utility::Random::GenerateFloat(jsonData_.scaleLimit.min, jsonData_.scaleLimit.max);
	randomScale_ = { scale, scale, scale };
	// 角度
	particleData_.euler.z = Random::GenerateFloat(0.0f, std::numbers::pi * 2.0f);

#pragma region 平面に反映
	// 色
	plane_.material.color = jsonData_.color;
	// 座標
	plane_.worldTF.translation = pos + jsonData_.createRange * vel.Normalize();
	plane_.worldTF.translation.y = pos.y;
	// 大きさ
	plane_.worldTF.scale = randomScale_;
	// 角度
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, particleData_.euler.z) * LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, particleData_.euler.x);
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, particleData_.euler.y) * plane_.worldTF.rotation;
#pragma endregion
}

void EnemySpawnParticle::UpdateParticle() {
	// 重力加速
	particleData_.vel.y += jsonData_.acceleration * jsonData_.multiply;

	// 移動処理
	plane_.worldTF.translation += particleData_.vel;

	// 回転
	particleData_.euler.z += 0.04f;
	// 角度
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, particleData_.euler.z) * LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, particleData_.euler.x);
	plane_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, particleData_.euler.y) * plane_.worldTF.rotation;

	// スケールのイージング
	plane_.worldTF.scale = Lerp(randomScale_, Vector3{ 0,0,0 }, particleData_.currentTime / particleData_.lifeTime);
	// 色のイージング
	int alpha = (int)LerpF((float)jsonData_.color.A, 0.0f, particleData_.currentTime / particleData_.lifeTime);
	plane_.material.color.A = alpha;
}
