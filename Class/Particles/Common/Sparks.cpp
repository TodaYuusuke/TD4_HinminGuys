#include "Sparks.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

Sparks::Sparks(const std::string& texName) {
	texName_ = texName;
}

void Sparks::Initialize() {

}

void Sparks::Update() {
	// 削除
	particles_.remove_if([](Spark* data) {
		if (!data->GetParticleData().isAlive) {
			delete data;
			return true;
		}
		return false;
		});

	// 各パーティクルの更新
	for (std::list<Spark*>::iterator it = particles_.begin(); it != particles_.end();) {
		(*it)->Update();
		it++;
	}
}

void Sparks::SetJsonData(LWP::Utility::JsonIO& json) {
	json.BeginGroup("RandomValueMinMax");
	// 速度
	json.BeginGroup("Velocity");
	json.AddValue<Vector3>("Max", &jsonData_.velLimit.max);
	json.AddValue<Vector3>("Min", &jsonData_.velLimit.min);
	json.EndGroup();
	// サイズ
	json.BeginGroup("Scale");
	json.AddValue<Vector3>("Max", &jsonData_.scaleLimit.max);
	json.AddValue<Vector3>("Min", &jsonData_.scaleLimit.min);
	json.EndGroup();

	json.EndGroup();

	// ケルビン
	json.BeginGroup("Kelvin");
	json.AddValue<float>("Max", &jsonData_.kelvinLimit.max);
	json.AddValue<float>("Min", &jsonData_.kelvinLimit.min);
	json.EndGroup();

	// 色
	json.AddValue<Color>("Color", &jsonData_.color);

	// 生成しない範囲
	json.AddValue<float>("CreateCircleRange", &jsonData_.createRange);

	// 初速
	json.AddValue<Vector3>("First", &jsonData_.firstVel);
	// 重力加速度
	json.AddValue<Vector3>("Acceleration", &jsonData_.acceleration);
	// 速度倍率
	json.AddValue<float>("Multiply", &jsonData_.multiply);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.maxElapseTime);
}

void Sparks::Add(int value) {
	for (int i = 0; i < value; i++) {
		Spark* p = new Spark(texName_);
		p->SetParticleData(GetParticleData());
		p->SetSparkJsonData(jsonData_);
		p->Create(emitterPos_);
		particles_.push_back(p);
	}
}

void Sparks::Add(int value, LWP::Math::Vector3 position) {
	emitterPos_ = position;
	Add(value);
}
