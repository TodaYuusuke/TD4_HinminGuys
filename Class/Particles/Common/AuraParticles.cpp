#include "AuraParticles.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

AuraParticles::AuraParticles() {}

void AuraParticles::Initialize() {

}

void AuraParticles::Update() {
	if (isActive_) {
		Add(count_);
	}

	// 削除
	particles_.remove_if([](AuraParticle* data) {
		if (!data->GetParticleData().isAlive) {
			delete data;
			return true;
		}
		return false;
		});

	// 各パーティクルの更新
	for (std::list<AuraParticle*>::iterator it = particles_.begin(); it != particles_.end();) {
		(*it)->Update();
		it++;
	}
}

void AuraParticles::JsonDebugGui() {
	json_.DebugGUI();
}

void AuraParticles::SetJsonData(LWP::Utility::JsonIO& json) {
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

	// 一度に生成する個数
	json.AddValue<int>("Count", &count_);
}

void AuraParticles::SetJsonData() {
	json_.Init("AuraParticles.json");
	json_.BeginGroup("AuraParticle");
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

	// 一度に生成する個数
	json_.AddValue<int>("Count", &count_);

	json_.EndGroup();

	json_.CheckJsonFile();
}

void AuraParticles::Start(const bool& isActive, const LWP::Math::Vector3& pos) {
	isActive_ = isActive;
	emitterPos_.translation = pos;
}

void AuraParticles::Finish() {
	isActive_ = false;
}

void AuraParticles::Add(int value) {
	for (int i = 0; i < value; i++) {
		AuraParticle* p = new AuraParticle(player_, followCamera_, texName_);
		p->SetAuraParticleJsonData(jsonData_);
		p->SetParent(&emitterPos_);
		p->Create(emitterPos_.GetWorldPosition());
		particles_.push_back(p);
	}
}