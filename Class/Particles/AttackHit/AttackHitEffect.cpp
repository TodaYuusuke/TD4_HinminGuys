#include "AttackHitEffect.h"

#include <numbers>

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

AttackHitEffect::AttackHitEffect() {}

void AttackHitEffect::Initialize() {}

void AttackHitEffect::Update() {
	// 削除
	particles_.remove_if([](IParticle* data) {
		if (!data->GetParticleData().isAlive) {
			delete data;
			return true;
		}
		return false;
		});

	// 各パーティクルの更新
	for (std::list<IParticle*>::iterator it = particles_.begin(); it != particles_.end();) {
		(*it)->Update();
		it++;
	}
}

void AttackHitEffect::JsonDebugGui() {
	json_.DebugGUI();
}

void AttackHitEffect::SetJsonData(LWP::Utility::JsonIO& json) {
#pragma region Spark
	json.BeginGroup("Spark");
	json.BeginGroup("RandomValueMinMax");
	// 速度
	json.BeginGroup("Velocity");
	json.AddValue<Vector3>("Max", &jsonData_.spark.velLimit.max);
	json.AddValue<Vector3>("Min", &jsonData_.spark.velLimit.min);
	json.EndGroup();
	// サイズ
	json.BeginGroup("Scale");
	json.AddValue<Vector3>("Max", &jsonData_.spark.scaleLimit.max);
	json.AddValue<Vector3>("Min", &jsonData_.spark.scaleLimit.min);
	json.EndGroup();

	json.EndGroup();

	// ケルビン
	json.BeginGroup("Kelvin");
	json.AddValue<float>("Max", &jsonData_.spark.kelvinLimit.max);
	json.AddValue<float>("Min", &jsonData_.spark.kelvinLimit.min);
	json.EndGroup();

	// 色
	json.AddValue<Color>("Color", &jsonData_.spark.color);

	// 生成しない範囲
	json.AddValue<float>("CreateCircleRange", &jsonData_.spark.createRange);

	// 初速
	json.AddValue<Vector3>("First", &jsonData_.spark.firstVel);
	// 重力加速度
	json.AddValue<Vector3>("Acceleration", &jsonData_.spark.acceleration);
	// 速度倍率
	json.AddValue<float>("Multiply", &jsonData_.spark.multiply);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.spark.maxElapseTime);

	// 生成数
	json.AddValue<int>("Count", &jsonData_.sparkCount);
	json.EndGroup();
#pragma endregion

#pragma region Circle
	json.BeginGroup("Circle");
	json.BeginGroup("RandomValueMinMax");
	// 速度
	json.BeginGroup("Velocity");
	json.AddValue<Vector3>("Max", &jsonData_.attackHitParticle.velLimit.max);
	json.AddValue<Vector3>("Min", &jsonData_.attackHitParticle.velLimit.min);
	json.EndGroup();
	// サイズ
	json.BeginGroup("Scale");
	json.AddValue<float>("Max", &jsonData_.attackHitParticle.scaleLimit.max);
	json.AddValue<float>("Min", &jsonData_.attackHitParticle.scaleLimit.min);
	json.EndGroup();

	json.EndGroup();

	// 色
	json.AddValue<Color>("Color", &jsonData_.attackHitParticle.color);

	// 初速
	json.AddValue<Vector3>("First", &jsonData_.attackHitParticle.firstVel);
	// 重力加速度
	json.AddValue<float>("Acceleration", &jsonData_.attackHitParticle.acceleration);
	// 速度倍率
	json.AddValue<float>("Multiply", &jsonData_.attackHitParticle.multiply);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.attackHitParticle.maxElapseTime);

	// 生成数
	json.AddValue<int>("Count", &jsonData_.attackHitParticleCount);
	json.EndGroup();
#pragma endregion
}

void AttackHitEffect::SetJsonData() {
	json_.Init("AttackHitEffect.json");
#pragma region Spark
	json_.BeginGroup("Spark");
	json_.BeginGroup("RandomValueMinMax");
	// 速度
	json_.BeginGroup("Velocity");
	json_.AddValue<Vector3>("Max", &jsonData_.spark.velLimit.max);
	json_.AddValue<Vector3>("Min", &jsonData_.spark.velLimit.min);
	json_.EndGroup();
	// サイズ
	json_.BeginGroup("Scale");
	json_.AddValue<Vector3>("Max", &jsonData_.spark.scaleLimit.max);
	json_.AddValue<Vector3>("Min", &jsonData_.spark.scaleLimit.min);
	json_.EndGroup();

	json_.EndGroup();

	// ケルビン
	json_.BeginGroup("Kelvin");
	json_.AddValue<float>("Max", &jsonData_.spark.kelvinLimit.max);
	json_.AddValue<float>("Min", &jsonData_.spark.kelvinLimit.min);
	json_.EndGroup();

	// 色
	json_.AddValue<Color>("Color", &jsonData_.spark.color);

	// 生成しない範囲
	json_.AddValue<float>("CreateCircleRange", &jsonData_.spark.createRange);

	// 初速
	json_.AddValue<Vector3>("First", &jsonData_.spark.firstVel);
	// 重力加速度
	json_.AddValue<Vector3>("Acceleration", &jsonData_.spark.acceleration);
	// 速度倍率
	json_.AddValue<float>("Multiply", &jsonData_.spark.multiply);

	// パーティクルが存在できる時間
	json_.AddValue<float>("ElapseTime", &jsonData_.spark.maxElapseTime);

	// 生成数
	json_.AddValue<int>("Count", &jsonData_.sparkCount);
	json_.EndGroup();
#pragma endregion

#pragma region Circle
	json_.BeginGroup("Circle");
	json_.BeginGroup("RandomValueMinMax");
	// 速度
	json_.BeginGroup("Velocity");
	json_.AddValue<Vector3>("Max", &jsonData_.attackHitParticle.velLimit.max);
	json_.AddValue<Vector3>("Min", &jsonData_.attackHitParticle.velLimit.min);
	json_.EndGroup();
	// サイズ
	json_.BeginGroup("Scale");
	json_.AddValue<float>("Max", &jsonData_.attackHitParticle.scaleLimit.max);
	json_.AddValue<float>("Min", &jsonData_.attackHitParticle.scaleLimit.min);
	json_.EndGroup();

	json_.EndGroup();

	// 色
	json_.AddValue<Color>("Color", &jsonData_.attackHitParticle.color);

	// 初速
	json_.AddValue<Vector3>("First", &jsonData_.attackHitParticle.firstVel);
	// 重力加速度
	json_.AddValue<float>("Acceleration", &jsonData_.attackHitParticle.acceleration);
	// 速度倍率
	json_.AddValue<float>("Multiply", &jsonData_.attackHitParticle.multiply);

	// パーティクルが存在できる時間
	json_.AddValue<float>("ElapseTime", &jsonData_.attackHitParticle.maxElapseTime);

	// 生成数
	json_.AddValue<int>("Count", &jsonData_.attackHitParticleCount);
	json_.EndGroup();
#pragma endregion

	json_.CheckJsonFile();
}

void AttackHitEffect::Add(const Vector3& pos) {
	// 攻撃ヒット時の丸いパーティクル
	for (int i = 0; i < jsonData_.attackHitParticleCount; i++) {
		AttackHitParticle* p = new AttackHitParticle("Effect/Particle.png");
		p->SetAttackHitParticleJsonData(jsonData_.attackHitParticle);
		p->Create(pos);
		particles_.push_back(p);
	}
	// 火花
	for (int i = 0; i < jsonData_.sparkCount; i++) {
		Spark* p = new Spark("Effect/Spark.png");
		p->SetSparkJsonData(jsonData_.spark);
		p->Create(pos);
		particles_.push_back(p);
	}
}