#include "EvasionEffect.h"
#include "../../../../Player.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

EvasionEffect::EvasionEffect(Player* player) {
	player_ = player;
}

void EvasionEffect::Generate(LWP::Object::Particle::Data& data) {
	// 速度
	Vector3 dir = { 0,0,-1 };
	Matrix4x4 rotMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(player_->GetRadian());
	dir = (dir * rotMatrix).Normalize();
	dir.y = 0.0f;
	data.velocity = dir - LWP::Utility::Random::GenerateVector3(jsonData_.velocity.min, jsonData_.velocity.max);
	data.velocity *= LWP::Utility::Random::GenerateFloat(0.2f, 0.5f);
	// スケール
	Vector3 scale = LWP::Utility::Random::GenerateVector3(jsonData_.scale.min, jsonData_.scale.max);
	data.m.worldTF.scale = scale;

	// パーティクルが存在できる時間
	data.elapsedTime = jsonData_.maxElapseTime * 60.0f;
}

bool EvasionEffect::UpdateParticle(LWP::Object::Particle::Data& data) {
	if (data.elapsedTime <= 0.0f) { return true; }

	data.elapsedTime--;

	data.velocity.y += -9.8f / 6000.0f;

	data.m.worldTF.translation += data.velocity;
	return false;
}

void EvasionEffect::DebugGui() {
	DebugGUI();
}

void EvasionEffect::SetJsonData(LWP::Utility::JsonIO& json) {
	json.BeginGroup("RandomValueMinMax");

	// 速度
	json.BeginGroup("Velocity");
	json.AddValue<Vector3>("Max", &jsonData_.velocity.max);
	json.AddValue<Vector3>("Min", &jsonData_.velocity.min);
	json.EndGroup();
	// サイズ
	json.BeginGroup("Scale");
	json.AddValue<Vector3>("Max", &jsonData_.scale.max);
	json.AddValue<Vector3>("Min", &jsonData_.scale.min);
	json.EndGroup();

	json.EndGroup();

	// 加速度
	json.AddValue<Vector3>("Acceleration", &jsonData_.acceleration);
	json.AddValue<Vector3>("FirstVelocity", &jsonData_.firstVel);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.maxElapseTime);

	// パーティクルの数
	json.AddValue<int>("Count", &jsonData_.count);
}
