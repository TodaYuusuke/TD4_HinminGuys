#include "EnemySpawnParticles.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

EnemySpawnParticles::EnemySpawnParticles(const std::string& texName) {
	texName_ = texName;
}

EnemySpawnParticles::~EnemySpawnParticles() {
	for (EnemySpawnParticle* p : particles_) {
		delete p;
	}
	particles_.clear();
}

void EnemySpawnParticles::Initialize() {}

void EnemySpawnParticles::Update() {
	if (isActive_) {
		Add(count_);
	}

	// 削除
	particles_.remove_if([](EnemySpawnParticle* data) {
		if (!data->GetParticleData().isAlive) {
			delete data;
			return true;
		}
		return false;
		});

	// 各パーティクルの更新
	for (std::list<EnemySpawnParticle*>::iterator it = particles_.begin(); it != particles_.end();) {
		(*it)->Update();
		it++;
	}
}

void EnemySpawnParticles::JsonDebugGui() {
	json_.DebugGUI();
}

void EnemySpawnParticles::SetJsonData(LWP::Utility::JsonIO& json) {
	json.BeginGroup("RandomValueMinMax");
	// 速度
	json.BeginGroup("Velocity");
	json.AddValue<Vector3>("Max", &jsonData_.velLimit.max);
	json.AddValue<Vector3>("Min", &jsonData_.velLimit.min);
	json.EndGroup();
	// サイズ
	json.BeginGroup("Scale");
	json.AddValue<float>("Max", &jsonData_.scaleLimit.max);
	json.AddValue<float>("Min", &jsonData_.scaleLimit.min);
	json.EndGroup();

	json.EndGroup();

	// 色
	json.AddValue<LWP::Utility::Color>("Color", &jsonData_.color);

	// 生成しない範囲
	json.AddValue<float>("CreateCircleRange", &jsonData_.createRange);
	// 重力加速度
	json.AddValue<float>("Acceleration", &jsonData_.acceleration);
	// 初速
	json.AddValue<Vector3>("FirstVelocity", &jsonData_.firstVel);
	// 速度倍率
	json.AddValue<float>("Multiply", &jsonData_.multiply);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.maxElapseTime);

	// 一度に生成する個数
	json.AddValue<int>("Count", &count_);
}

void EnemySpawnParticles::SetJsonData() {
	json_.Init("EnemySpawnParticles.json");
	json_.BeginGroup("EnemySpawnParticle");
	json_.BeginGroup("RandomValueMinMax");
	// 速度
	json_.BeginGroup("Velocity");
	json_.AddValue<Vector3>("Max", &jsonData_.velLimit.max);
	json_.AddValue<Vector3>("Min", &jsonData_.velLimit.min);
	json_.EndGroup();
	// サイズ
	json_.BeginGroup("Scale");
	json_.AddValue<float>("Max", &jsonData_.scaleLimit.max);
	json_.AddValue<float>("Min", &jsonData_.scaleLimit.min);
	json_.EndGroup();

	json_.EndGroup();

	// 色
	json_.AddValue<LWP::Utility::Color>("Color", &jsonData_.color);

	// 生成しない範囲
	json_.AddValue<float>("CreateCircleRange", &jsonData_.createRange);
	// 重力加速度
	json_.AddValue<float>("Acceleration", &jsonData_.acceleration);
	// 初速
	json_.AddValue<Vector3>("FirstVelocity", &jsonData_.firstVel);
	// 速度倍率
	json_.AddValue<float>("Multiply", &jsonData_.multiply);

	// パーティクルが存在できる時間
	json_.AddValue<float>("ElapseTime", &jsonData_.maxElapseTime);

	// 一度に生成する個数
	json_.AddValue<int>("Count", &count_);

	json_.EndGroup();

	json_.CheckJsonFile();
}

void EnemySpawnParticles::Start(const bool& isActive, const LWP::Math::Vector3& pos) {
	isActive_ = isActive;
	emitterPos_ = pos;
}

void EnemySpawnParticles::Finish() {
	isActive_ = false;
}

void EnemySpawnParticles::Add(int value) {
	for (int i = 0; i < value; i++) {
		EnemySpawnParticle* p = new EnemySpawnParticle(texName_);
		p->SetEnemySpawnParticleJsonData(jsonData_);
		p->Create(emitterPos_);
		particles_.push_back(p);
	}
}