#include "EnemyDeadParticles.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

EnemyDeadParticles::EnemyDeadParticles(const std::string& texName) {
	texName_ = texName;
}

EnemyDeadParticles::~EnemyDeadParticles() {
	for (EnemyDeadParticle* p : particles_) {
		delete p;
	}
	particles_.clear();
}

void EnemyDeadParticles::Initialize() {}

void EnemyDeadParticles::Update() {
	// 削除
	particles_.remove_if([](EnemyDeadParticle* data) {
		if (!data->GetParticleData().isAlive) {
			delete data;
			return true;
		}
		return false;
		});

	// 各パーティクルの更新
	for (std::list<EnemyDeadParticle*>::iterator it = particles_.begin(); it != particles_.end();) {
		(*it)->Update();
		it++;
	}
}

void EnemyDeadParticles::JsonDebugGui() {
	json_.DebugGUI();
}

void EnemyDeadParticles::SetJsonData(LWP::Utility::JsonIO& json) {
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

	// 重力加速度
	json.AddValue<float>("Acceleration", &jsonData_.acceleration);
	// 速度倍率
	json.AddValue<float>("Multiply", &jsonData_.multiply);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.maxElapseTime);
}

void EnemyDeadParticles::SetJsonData() {
	json_.Init("EnemyDeadParticles.json");
	json_.BeginGroup("EnemyDeadParticle");
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

	// 重力加速度
	json_.AddValue<float>("Acceleration", &jsonData_.acceleration);
	// 速度倍率
	json_.AddValue<float>("Multiply", &jsonData_.multiply);

	// パーティクルが存在できる時間
	json_.AddValue<float>("ElapseTime", &jsonData_.maxElapseTime);

	json_.EndGroup();

	json_.CheckJsonFile();
}

void EnemyDeadParticles::Add(int value) {
	for (int i = 0; i < value; i++) {
		EnemyDeadParticle* p = new EnemyDeadParticle(texName_);
		p->SetEnemyDeadParticleJsonData(jsonData_);
		p->Create(emitterPos_);
		particles_.push_back(p);
	}
}

void EnemyDeadParticles::Add(int value, LWP::Math::Vector3 pos) {
	emitterPos_ = pos;
	Add(value);
}