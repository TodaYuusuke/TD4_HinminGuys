#include "FloatParticle.h"
#include "../../../Player/Player.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

FloatParticle::FloatParticle(Player* player) {
	player_ = player;
}

void FloatParticle::Generate(LWP::Object::Particle::Data& data) {
	isStartDrop_ = false;

	// パーティクルの浮く高さをランダムで設定
	datas_[data.idNumber].amplitude = LWP::Utility::Random::GenerateFloat(jsonData_.amplitudeLimit.min, jsonData_.amplitudeLimit.max);
	datas_[data.idNumber].height = LWP::Utility::Random::GenerateFloat(jsonData_.heightLimit.min, jsonData_.heightLimit.max);

	// 座標
	float posMin = jsonData_.posLimit.min;
	float posMax = jsonData_.posLimit.max;
	data.m.worldTF.translation += LWP::Utility::Random::GenerateVector3(Vector3{ posMin ,posMin ,posMin }, Vector3{ posMax ,posMax ,posMax });
	data.m.worldTF.translation.y = 0.0f;
	// スケール
	float scale = LWP::Utility::Random::GenerateFloat(jsonData_.scaleLimit.min, jsonData_.scaleLimit.max);
	data.m.worldTF.scale = {
		scale,
		scale,
		scale
	};

	// パーティクルが存在できる時間
	data.elapsedTime = jsonData_.maxFloatTime * 60.0f + jsonData_.maxDropingTime * 60.0f;
}

bool FloatParticle::UpdateParticle(LWP::Object::Particle::Data& data) {
	if (data.elapsedTime <= 0.0f) { 
		datas_.erase(data.idNumber);
		return true;
	}

	// 移動状態の確認と更新
	StateUpdate(data);

	data.m.worldTF.translation += data.velocity;
	return false;
}

void FloatParticle::Floating(LWP::Object::Particle::Data& data) {
	isHover_ = false;
	float aa = (jsonData_.maxFloatTime * 60.0f + jsonData_.maxDropingTime * 60.0f) - data.elapsedTime;
	float bb = (jsonData_.maxFloatTime * 60.0f);
	float currentFrame = aa / bb;
	data.m.worldTF.translation.y = LerpF(0.0f, datas_[data.idNumber].height,  Easing::InOutCubic(currentFrame));

	data.velocity = { 0.0f, 0.0f, 0.0f };
	data.elapsedTime--;
}

void FloatParticle::Hover(LWP::Object::Particle::Data& data) {
	isHover_ = true;
	float theta = datas_[data.idNumber].currentFrame / 60.0f;

	data.m.worldTF.translation.y += std::sinf(theta) * datas_[data.idNumber].amplitude;

	datas_[data.idNumber].currentFrame++;
}

void FloatParticle::Droping(LWP::Object::Particle::Data& data) {
	isHover_ = false;
	data.velocity.y += Interpolation::Exponential(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 0.0f, jsonData_.acceleration, 0.0f }, 0.05f).y;

	// 地面に反射する
	if (data.m.worldTF.translation.y <= 0.0f) {
		data.velocity.x *= 0.8f;
		data.velocity.y *= -0.8f;
		data.velocity.z *= 0.8f;
	}

	data.elapsedTime--;
}

void FloatParticle::StateUpdate(LWP::Object::Particle::Data& data) {
	// 落下状態
	if (isStartDrop_) {
		Droping(data);
	}
	else {
		// 浮遊状態
		if (((jsonData_.maxFloatTime * 60.0f + jsonData_.maxDropingTime * 60.0f) - data.elapsedTime) <= (jsonData_.maxFloatTime * 60.0f)) {
			Floating(data);
		}
		// 滞空
		else {
			Hover(data);
		}
	}
}

void FloatParticle::DebugGui() {
	DebugGUI();
}

void FloatParticle::SetJsonData(LWP::Utility::JsonIO& json) {
	json.BeginGroup("RandomValueMinMax");

	// 速度
	json.BeginGroup("Amplitude");
	json.AddValue<float>("Max", &jsonData_.amplitudeLimit.max);
	json.AddValue<float>("Min", &jsonData_.amplitudeLimit.min);
	json.EndGroup();
	// 高さ
	json.BeginGroup("Height");
	json.AddValue<float>("Max", &jsonData_.heightLimit.max);
	json.AddValue<float>("Min", &jsonData_.heightLimit.min);
	json.EndGroup();
	// 座標
	json.BeginGroup("Pos");
	json.AddValue<float>("Max", &jsonData_.posLimit.max);
	json.AddValue<float>("Min", &jsonData_.posLimit.min);
	json.EndGroup();
	// サイズ
	json.BeginGroup("Scale");
	json.AddValue<float>("Max", &jsonData_.scaleLimit.max);
	json.AddValue<float>("Min", &jsonData_.scaleLimit.min);
	json.EndGroup();

	json.EndGroup();

	// 係数
	json.AddValue<float>("Multiply", &jsonData_.multiply);

	// 加速度
	json.AddValue<float>("Acceleration", &jsonData_.acceleration);

	// 浮き終わり時間
	json.AddValue<float>("FloatTime", &jsonData_.maxFloatTime);
	// 落下終了時間
	json.AddValue<float>("DropingTime", &jsonData_.maxDropingTime);
}