#include "DustClouds.h"
#include <numbers>
using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

DustClouds::DustClouds(const std::string& texName) {
	texName_ = texName;
}

void DustClouds::Initialize() {}

void DustClouds::Update() {
	// 削除
	particles_.remove_if([](DustCloud* data) {
		if (!data->GetParticleData().isAlive) {
			delete data;
			return true;
		}
		return false;
		});

	// 各パーティクルの更新
	for (std::list<DustCloud*>::iterator it = particles_.begin(); it != particles_.end();) {
		(*it)->Update();
		it++;
	}
}

void DustClouds::JsonDebugGui() {
	json_.DebugGUI();
}

void DustClouds::SetJsonData(LWP::Utility::JsonIO& json) {
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
	// 白さ具合
	json.BeginGroup("White");
	json.AddValue<int>("Max", &jsonData_.whiteLimit.max);
	json.AddValue<int>("Min", &jsonData_.whiteLimit.min);
	json.EndGroup();
	// 透明度
	json.BeginGroup("Alpha");
	json.AddValue<int>("Max", &jsonData_.alphaLimit.max);
	json.AddValue<int>("Min", &jsonData_.alphaLimit.min);
	json.EndGroup();

	json.EndGroup();

	// 初速
	json.AddValue<Vector3>("FirstVelocity", &jsonData_.firstVel);
	// 重力加速度
	json.AddValue<float>("Acceleration", &jsonData_.acceleration);
	// 速度倍率
	json.AddValue<float>("Multiply", &jsonData_.multiply);
	// 速度減衰率
	json.AddValue<float>("DamplingRate", &jsonData_.dampingRate);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.maxElapseTime);
}

void DustClouds::SetJsonData() {
	json_.Init("DustClouds.json");
	json_.BeginGroup("DustCloud");
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
	// 白さ具合
	json_.BeginGroup("White");
	json_.AddValue<int>("Max", &jsonData_.whiteLimit.max);
	json_.AddValue<int>("Min", &jsonData_.whiteLimit.min);
	json_.EndGroup();
	// 透明度
	json_.BeginGroup("Alpha");
	json_.AddValue<int>("Max", &jsonData_.alphaLimit.max);
	json_.AddValue<int>("Min", &jsonData_.alphaLimit.min);
	json_.EndGroup();

	json_.EndGroup();

	// 初速
	json_.AddValue<Vector3>("FirstVelocity", &jsonData_.firstVel);
	// 重力加速度
	json_.AddValue<float>("Acceleration", &jsonData_.acceleration);
	// 速度倍率
	json_.AddValue<float>("Multiply", &jsonData_.multiply);
	// 速度減衰率
	json_.AddValue<float>("DamplingRate", &jsonData_.dampingRate);

	// パーティクルが存在できる時間
	json_.AddValue<float>("ElapseTime", &jsonData_.maxElapseTime);

	json_.EndGroup();

	json_.CheckJsonFile();
}

void DustClouds::Add(int value, const Vector3& pos) {
	for (int i = 0; i < value; i++) {
		DustCloud* p = new DustCloud(texName_);
		p->SetDustCloudJsonData(jsonData_);
		// 角度指定
		p->SetShotRotate(shotRotate_);
		p->Create(pos);
		particles_.push_back(p);

		shotRotate_.y += (float)std::numbers::pi * 2.0f / value;
	}
	shotRotate_ = { 0,0,0 };
}