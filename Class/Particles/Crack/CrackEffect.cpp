#include "CrackEffect.h"
#include <numbers>

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

CrackEffect::CrackEffect(const std::string& texName) {
	texName_ = texName;
	// 土煙
	dustClouds_ = std::make_unique<DustClouds>("Effect/Smoke.png");
	dustClouds_->Initialize();
}

CrackEffect::~CrackEffect() {
	for (IParticle* p : particles_) {
		delete p;
	}
	particles_.clear();
}

void CrackEffect::Initialize() {}

void CrackEffect::Update() {
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
	// 土煙
	dustClouds_->Update();
}

void CrackEffect::JsonDebugGui() {
	json_.DebugGUI();
}

void CrackEffect::SetJsonData(LWP::Utility::JsonIO& json) {
	json.BeginGroup("Smoke");
	// 土煙
	dustClouds_->SetJsonData(json);
	// 生成数
	json.AddValue<int>("Count", &jsonData_.smokeCount);
	json.EndGroup();

	// 割れた平面
	json.BeginGroup("CrackPlane");
	json.AddValue<Vector3>("Scale", &jsonData_.crackPlane.scale);
	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.crackPlane.maxElapseTime);
	json.EndGroup();

#pragma region Circle
	json.BeginGroup("Circle");
	json.BeginGroup("RandomValueMinMax");
	// 速度
	json.BeginGroup("Velocity");
	json.AddValue<Vector3>("Max", &jsonData_.rockParticle.velLimit.max);
	json.AddValue<Vector3>("Min", &jsonData_.rockParticle.velLimit.min);
	json.EndGroup();
	// サイズ
	json.BeginGroup("Scale");
	json.AddValue<float>("Max", &jsonData_.rockParticle.scaleLimit.max);
	json.AddValue<float>("Min", &jsonData_.rockParticle.scaleLimit.min);
	json.EndGroup();

	json.EndGroup();

	// 色
	json.AddValue<Color>("Color", &jsonData_.rockParticle.color);

	// 初速
	json.AddValue<Vector3>("First", &jsonData_.rockParticle.firstVel);
	// 重力加速度
	json.AddValue<float>("Acceleration", &jsonData_.rockParticle.acceleration);
	// 速度倍率
	json.AddValue<float>("Multiply", &jsonData_.rockParticle.multiply);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.rockParticle.maxElapseTime);

	// 生成数
	json.AddValue<int>("Count", &jsonData_.rockParticleCount);
	json.EndGroup();
#pragma endregion
}

void CrackEffect::SetJsonData() {
	json_.Init("CrackEffect.json");
	json_.BeginGroup("Smoke");
	// 土煙
	dustClouds_->SetJsonData(json_);
	// 生成数
	json_.AddValue<int>("Count", &jsonData_.smokeCount);
	json_.EndGroup();

	// 割れた平面
	json_.BeginGroup("CrackPlane");
	json_.AddValue<Vector3>("Scale", &jsonData_.crackPlane.scale);
	// パーティクルが存在できる時間
	json_.AddValue<float>("ElapseTime", &jsonData_.crackPlane.maxElapseTime);
	json_.EndGroup();

#pragma region Circle
	json_.BeginGroup("Circle");
	json_.BeginGroup("RandomValueMinMax");
	// 速度
	json_.BeginGroup("Velocity");
	json_.AddValue<Vector3>("Max", &jsonData_.rockParticle.velLimit.max);
	json_.AddValue<Vector3>("Min", &jsonData_.rockParticle.velLimit.min);
	json_.EndGroup();
	// サイズ
	json_.BeginGroup("Scale");
	json_.AddValue<float>("Max", &jsonData_.rockParticle.scaleLimit.max);
	json_.AddValue<float>("Min", &jsonData_.rockParticle.scaleLimit.min);
	json_.EndGroup();

	json_.EndGroup();

	// 色
	json_.AddValue<Color>("Color", &jsonData_.rockParticle.color);

	// 初速
	json_.AddValue<Vector3>("First", &jsonData_.rockParticle.firstVel);
	// 重力加速度
	json_.AddValue<float>("Acceleration", &jsonData_.rockParticle.acceleration);
	// 速度倍率
	json_.AddValue<float>("Multiply", &jsonData_.rockParticle.multiply);

	// パーティクルが存在できる時間
	json_.AddValue<float>("ElapseTime", &jsonData_.rockParticle.maxElapseTime);

	// 生成数
	json_.AddValue<int>("Count", &jsonData_.rockParticleCount);
	json_.EndGroup();
#pragma endregion

	json_.CheckJsonFile();
}

void CrackEffect::Add(const Vector3& pos) {
	// 土煙
	dustClouds_->Add(jsonData_.smokeCount, pos);

	// 岩
	for (int i = 0; i < jsonData_.rockParticleCount; i++) {
		AttackHitParticle* p = new AttackHitParticle("Effect/Rock.png");
		p->SetAttackHitParticleJsonData(jsonData_.rockParticle);
		p->Create(pos);
		particles_.push_back(p);
	}

	// 割れた地面
	CrackPlane* p = new CrackPlane("Effect/Crack.png");
	p->SetCrackPlaneJsonData(jsonData_.crackPlane);
	p->Create(pos);
	particles_.push_back(p);
}