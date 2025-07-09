#include "MoveEffect.h"
#include "../../../../../Camera/FollowCamera.h"
#include "../../../../../Player/Player.h"
#include "../../../../Math/MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

MoveEffect::MoveEffect(Player* player, FollowCamera* followCamera) {
	player_ = player;
	followCamera_ = followCamera;
}

void MoveEffect::Initialize() {

}

void MoveEffect::Update() {
	IEffect::Update();
}

void MoveEffect::DebugGui() {

}

void MoveEffect::SetJsonData(LWP::Utility::JsonIO& json) {
#pragma region Circle
	json.BeginGroup("Circle");
	json.BeginGroup("RandomValueMinMax");
	// 速度
	json.BeginGroup("Velocity");
	json.AddValue<Vector3>("Max", &dustCloudData_.velocity.max);
	json.AddValue<Vector3>("Min", &dustCloudData_.velocity.min);
	json.EndGroup();
	// サイズ
	json.BeginGroup("Scale");
	json.AddValue<Vector3>("Max", &dustCloudData_.scale.max);
	json.AddValue<Vector3>("Min", &dustCloudData_.scale.min);
	json.EndGroup();

	json.EndGroup();

	// 初速
	json.AddValue<Vector3>("First", &dustCloudData_.firstVel);
	// 重力加速度
	json.AddValue<Vector3>("Acceleration", &dustCloudData_.acceleration);
	// 速度倍率
	json.AddValue<float>("Multiply", &dustCloudData_.multiply);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &dustCloudData_.maxElapseTime);

	// パーティクルの数
	json.AddValue<int>("Count", &dustCloudData_.count);
	json.EndGroup();
#pragma endregion
}

ParticleData MoveEffect::CreateDustCloud(LWP::Math::Vector3 pos) {
	ParticleData particle;

	// パーティクルの種類
	particle.type = (int)ParticleType::kCircle;

	// ビルボード生成
	particle.billboard.anchorPoint = { 0.5f, 0.5f };
	particle.billboard.material.enableLighting = false;
	particle.billboard.material.texture = LWP::Resource::LoadTexture("Effect/Particle.png");
	particle.billboard.Init();
	// 平面生成(非表示)
	particle.plane.LoadShortPath("BothPlane.obj");
	particle.plane.isActive = false;

	// 速度
	Vector3 vel = LWP::Utility::Random::GenerateVector3(dustCloudData_.velocity.min, dustCloudData_.velocity.max);
	Vector3 rot = { 0.0f, createRotateY_, 0.0f };
	Vector3 dir = Vector3{ 0.0f, 0.0f, 1.0f } * Math::Matrix4x4::CreateRotateXYZMatrix(rot);
	particle.vel = vel + dir * dustCloudData_.multiply;
	particle.vel.y += dustCloudData_.firstVel.y;

	// 座標
	particle.billboard.worldTF.translation = pos;
	// 大きさ
	Vector3 scale = LWP::Utility::Random::GenerateVector3(dustCloudData_.scale.min, dustCloudData_.scale.max);
	particle.billboard.worldTF.scale = {
		scale.x,
		scale.x,
		scale.x
	};

	// 生存可能時間
	particle.lifeTime = dustCloudData_.maxElapseTime * 60.0f;
	particle.currentTime = 0;

	// 更新処理を設定
	particle.updateFunc = std::bind(&MoveEffect::DustCloudUpdate, this, std::placeholders::_1);

	return particle;
}

void MoveEffect::CreateDustClouds(LWP::Math::Vector3 pos) {
	// 発生地点
	emitterPos_ = pos;

	// パーティクル発生
	particles_.splice(particles_.end(), Emission(pos, createRotateY_));

	// 使用するテクスチャ設定
	for (std::list<ParticleData>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		switch ((*particleIterator).type) {
			// 円
		case (int)ParticleType::kCircle:
			//(*particleIterator).billboard.material.texture = LWP::Resource::LoadTexture("Effect/Particle.png");
			break;
		}
		// ライティングなし
		(*particleIterator).billboard.material.enableLighting = false;

		particleIterator++;
	}
}

std::list<ParticleData> MoveEffect::Emission(LWP::Math::Vector3 pos, float& createRotateY) {
	std::list<ParticleData> particles;

	// 土煙
	for (int32_t count = 0; count < dustCloudData_.count; ++count) {
		particles.push_back(CreateDustCloud(pos));
		createRotateY += (float)std::numbers::pi * 2.0f / dustCloudData_.count;
	}
	createRotateY = 0.0f;
	return particles;
}

void MoveEffect::DustCloudUpdate(ParticleData& data) {
	data.vel.y += dustCloudData_.acceleration.y;
	// 移動処理
	data.billboard.worldTF.translation += data.vel;
}