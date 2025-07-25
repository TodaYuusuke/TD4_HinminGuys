#include "FloatParticle.h"
#include "../../../Player.h"

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
	// 速度
	//Vector3 dir = { 0,0,-1 };
	//Matrix4x4 rotMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(player_->GetRadian());
	//dir = (dir * rotMatrix).Normalize();
	//dir.y = 0.0f;
	//data.velocity = dir - LWP::Utility::Random::GenerateVector3(jsonData_.velocity.min, jsonData_.velocity.max);
	//data.velocity *= LWP::Utility::Random::GenerateFloat(0.2f, 0.5f);
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
	if (data.elapsedTime <= 0.0f) { return true; }

	// 移動状態の確認と更新
	StateUpdate(data);

	data.m.worldTF.translation += data.velocity;
	return false;
}

void FloatParticle::Floating(LWP::Object::Particle::Data& data) {
	float aa = (jsonData_.maxFloatTime * 60.0f + jsonData_.maxDropingTime * 60.0f) - data.elapsedTime;
	float bb = (jsonData_.maxFloatTime * 60.0f);
	float currentFrame = aa / bb;
	data.m.worldTF.translation.y = LerpF(0.0f, datas_[data.idNumber].height,  Easing::InOutCubic(currentFrame));

	data.velocity = { 0.0f, 0.0f, 0.0f };
	data.elapsedTime--;
}

void FloatParticle::Hover(LWP::Object::Particle::Data& data) {

}

void FloatParticle::Droping(LWP::Object::Particle::Data& data) {
	data.velocity.y += -9.8f / 6000.0f;

	data.elapsedTime--;
}

void FloatParticle::StateUpdate(LWP::Object::Particle::Data& data) {
	if(!player_->GetSystemManager()->GetSheathSystem()->GetIsSheathing()) {
		isStartDrop_ = true;
	}

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
	// サイズ
	json.BeginGroup("Scale");
	json.AddValue<float>("Max", &jsonData_.scaleLimit.max);
	json.AddValue<float>("Min", &jsonData_.scaleLimit.min);
	json.EndGroup();

	json.EndGroup();

	// 係数
	json.AddValue<float>("Multiply", &jsonData_.multiply);

	// 加速度
	json.AddValue<Vector3>("Acceleration", &jsonData_.acceleration);

	// 浮き終わり時間
	json.AddValue<float>("FloatTime", &jsonData_.maxFloatTime);
	// 落下終了時間
	json.AddValue<float>("DropingTime", &jsonData_.maxDropingTime);
}