#include "BossDeadParticles.h"
#include <numbers>

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

BossDeadParticles::BossDeadParticles() {
	sparks_ = std::make_unique<Sparks>("Effect/Spark.png");
	sparks_->Initialize();
}

BossDeadParticles::~BossDeadParticles() {
	for (DeadSmoke* p : particles_) {
		delete p;
	}
	particles_.clear();
}

void BossDeadParticles::Initialize() {}

void BossDeadParticles::Update() {
	if (currentFrame_ > 0.0f) {
		Add(emitterPos_);
	}

	// 削除
	particles_.remove_if([](DeadSmoke* data) {
		if (!data->GetParticleData().isAlive) {
			delete data;
			return true;
		}
		return false;
		});

	// 各パーティクルの更新
	for (std::list<DeadSmoke*>::iterator it = particles_.begin(); it != particles_.end();) {
		(*it)->Update();
		it++;
	}

	// 火花
	sparks_->Update();

	createSparkTiming_--;
	currentFrame_--;
}

void BossDeadParticles::JsonDebugGui() {
	json_.DebugGUI();
}

void BossDeadParticles::CreateSparks(const LWP::Math::Vector3& pos) {
	if (createSparkTiming_ > 0.0f) { return; }

	// 火花生成
	createSparkTiming_ = Utility::Random::GenerateFloat(jsonData_.sparkFrequency.min, jsonData_.sparkFrequency.max) * 60.0f;
	sparks_->Add(jsonData_.sparkCount, pos);
}

void BossDeadParticles::SetJsonData(LWP::Utility::JsonIO& json) {
	json.BeginGroup("CreatePosOffset");
	json.AddValue<float>("Max", &jsonData_.CreatePosOffset.max);
	json.AddValue<float>("Min", &jsonData_.CreatePosOffset.min);
	json.EndGroup();

#pragma region Spark
	json.BeginGroup("Spark");
	sparks_->SetJsonData(json);
	// 火花の発生頻度
	json.BeginGroup("Frequency");
	json.AddValue<float>("Max", &jsonData_.sparkFrequency.max);
	json.AddValue<float>("Min", &jsonData_.sparkFrequency.min);
	json.EndGroup();
	// 生成数
	json.AddValue<int>("Count", &jsonData_.sparkCount);
	json.EndGroup();
#pragma endregion

#pragma region Smoke
	json.BeginGroup("Smoke");
	json.BeginGroup("RandomValueMinMax");
	// 速度
	json.BeginGroup("Velocity");
	json.AddValue<Vector3>("Max", &jsonData_.deadSmoke.velLimit.max);
	json.AddValue<Vector3>("Min", &jsonData_.deadSmoke.velLimit.min);
	json.EndGroup();
	// サイズ
	json.BeginGroup("Scale");
	json.AddValue<float>("Max", &jsonData_.deadSmoke.scaleLimit.max);
	json.AddValue<float>("Min", &jsonData_.deadSmoke.scaleLimit.min);
	json.EndGroup();
	// 色
	json.BeginGroup("Color");
	json.AddValue<Color>("Max", &jsonData_.deadSmoke.colorLimit.max);
	json.AddValue<Color>("Min", &jsonData_.deadSmoke.colorLimit.min);
	json.EndGroup();

	json.EndGroup();

	// 初速
	json.AddValue<Vector3>("First", &jsonData_.deadSmoke.firstVel);
	// 重力加速度
	json.AddValue<float>("Acceleration", &jsonData_.deadSmoke.acceleration);
	// 速度倍率
	json.AddValue<float>("Multiply", &jsonData_.deadSmoke.multiply);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.deadSmoke.maxElapseTime);

	// 生成数
	json.AddValue<int>("Count", &jsonData_.deadSmokeCount);
	json.EndGroup();
#pragma endregion
}

void BossDeadParticles::SetJsonData() {
	json_.Init("BossDeadParticles.json");
	json_.BeginGroup("CreatePosOffset");
	json_.AddValue<float>("Max", &jsonData_.CreatePosOffset.max);
	json_.AddValue<float>("Min", &jsonData_.CreatePosOffset.min);
	json_.EndGroup();

#pragma region Spark
	json_.BeginGroup("Spark");
	sparks_->SetJsonData(json_);
	// 火花の発生頻度
	json_.BeginGroup("Frequency");
	json_.AddValue<float>("Max", &jsonData_.sparkFrequency.max);
	json_.AddValue<float>("Min", &jsonData_.sparkFrequency.min);
	json_.EndGroup();
	// 生成数
	json_.AddValue<int>("Count", &jsonData_.sparkCount);
	json_.EndGroup();
#pragma endregion

#pragma region Smoke
	json_.BeginGroup("Smoke");
	json_.BeginGroup("RandomValueMinMax");
	// 速度
	json_.BeginGroup("Velocity");
	json_.AddValue<Vector3>("Max", &jsonData_.deadSmoke.velLimit.max);
	json_.AddValue<Vector3>("Min", &jsonData_.deadSmoke.velLimit.min);
	json_.EndGroup();
	// サイズ
	json_.BeginGroup("Scale");
	json_.AddValue<float>("Max", &jsonData_.deadSmoke.scaleLimit.max);
	json_.AddValue<float>("Min", &jsonData_.deadSmoke.scaleLimit.min);
	json_.EndGroup();
	// 色
	json_.BeginGroup("Color");
	json_.AddValue<Color>("Max", &jsonData_.deadSmoke.colorLimit.max);
	json_.AddValue<Color>("Min", &jsonData_.deadSmoke.colorLimit.min);
	json_.EndGroup();

	json_.EndGroup();

	// 初速
	json_.AddValue<Vector3>("First", &jsonData_.deadSmoke.firstVel);
	// 重力加速度
	json_.AddValue<float>("Acceleration", &jsonData_.deadSmoke.acceleration);
	// 速度倍率
	json_.AddValue<float>("Multiply", &jsonData_.deadSmoke.multiply);

	// パーティクルが存在できる時間
	json_.AddValue<float>("ElapseTime", &jsonData_.deadSmoke.maxElapseTime);

	// 生成数
	json_.AddValue<int>("Count", &jsonData_.deadSmokeCount);
	json_.EndGroup();
#pragma endregion

	json_.CheckJsonFile();
}

void BossDeadParticles::Start(const float& createTime, const LWP::Math::Vector3& pos) {
	currentFrame_ = createTime * 60.0f;
	emitterPos_ = pos;
}

void BossDeadParticles::Add(const Vector3& pos) {
	// 生成時の座標をずらす
	Vector3 createPos = {
		LWP::Utility::Random::GenerateFloat(jsonData_.CreatePosOffset.min, jsonData_.CreatePosOffset.max),
		LWP::Utility::Random::GenerateFloat(jsonData_.CreatePosOffset.min, jsonData_.CreatePosOffset.max),
		LWP::Utility::Random::GenerateFloat(jsonData_.CreatePosOffset.min, jsonData_.CreatePosOffset.max)
	};

	// 死亡時の煙
	for (int i = 0; i < jsonData_.deadSmokeCount; i++) {
		DeadSmoke* p = new DeadSmoke("Effect/Smoke.png");
		p->SetDeadSmokeJsonData(jsonData_.deadSmoke);
		p->Create(pos);
		particles_.push_back(p);
	}

	// 火花
	CreateSparks(pos + createPos);
}