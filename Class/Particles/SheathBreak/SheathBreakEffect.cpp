#include "SheathBreakEffect.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

SheathBreakEffect::SheathBreakEffect(FollowCamera* followCamera) {
	followCamera_ = followCamera;
}

SheathBreakEffect::~SheathBreakEffect() {
	delete noticeCircle_;
	for (IParticle* p : particles_) {
		delete p;
	}
	particles_.clear();
}

void SheathBreakEffect::Initialize() {}

void SheathBreakEffect::Update() {
	// 一つ目の円が収縮しきっている時残りのパーティクル生成
	if (noticeCircle_ && isCreate_ &&
		noticeCircle_->GetParticleData().currentTime >= noticeCircle_->GetNoticeCircleJsonData().minScaleTime * 60.0f) {
		// 点滅する閃光
		for (int i = 0; i < jsonData_.flickeringFlashCount; i++) {
			FlickeringFlash* p = new FlickeringFlash(followCamera_, "Effect/Circle.png");
			p->SetFlickeringFlashJsonData(jsonData_.flickeringFlash);
			p->Create(emitterPos_);
			particles_.push_back(p);
		}
		// ひし形
		for (int i = 0; i < jsonData_.rhombusParticleCount; i++) {
			RhombusParticle* p = new RhombusParticle(followCamera_, "Effect/Star.png");
			p->SetRhombusParticleJsonData(jsonData_.rhombusParticle);
			p->SetShotRotate(shotRotate_);
			p->Create(emitterPos_);
			particles_.push_back(p);

			shotRotate_.x += (float)std::numbers::pi * 2.0f / jsonData_.rhombusParticleCount;
		}
		shotRotate_ = { 0.0f,(float)std::numbers::pi / 2.0f,0.0f };
		isCreate_ = false;
	}

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

	// リング
	//rings_->Update();
	if (noticeCircle_) {
		noticeCircle_->Update();
		if (!noticeCircle_->GetParticleData().isAlive) {
			delete noticeCircle_;
			noticeCircle_ = nullptr;
		}
	}
}

void SheathBreakEffect::JsonDebugGui() {
	json_.DebugGUI();
}

void SheathBreakEffect::SetJsonData(LWP::Utility::JsonIO& json) {
#pragma region RhombusParticle
	json.BeginGroup("RhombusParticle");
	json.BeginGroup("RandomValueMinMax");
	// 速度
	json.BeginGroup("Velocity");
	json.AddValue<Vector3>("Max", &jsonData_.rhombusParticle.velLimit.max);
	json.AddValue<Vector3>("Min", &jsonData_.rhombusParticle.velLimit.min);
	json.EndGroup();
	// サイズ
	json.BeginGroup("Scale");
	json.AddValue<float>("Max", &jsonData_.rhombusParticle.scaleLimit.max);
	json.AddValue<float>("Min", &jsonData_.rhombusParticle.scaleLimit.min);
	json.EndGroup();

	json.EndGroup();

	// 色
	json.AddValue<Color>("Color", &jsonData_.rhombusParticle.color);
	// 最大サイズ
	json.AddValue<Vector3>("MaxScale", &jsonData_.rhombusParticle.maxScale);
	// 初速
	json.AddValue<Vector3>("FirstVelocity", &jsonData_.rhombusParticle.firstVel);
	// 重力加速度
	json.AddValue<float>("Acceleration", &jsonData_.rhombusParticle.acceleration);
	// 速度倍率
	json.AddValue<float>("Multiply", &jsonData_.rhombusParticle.multiply);
	// 速度減衰率
	json.AddValue<float>("DamplingRate", &jsonData_.rhombusParticle.dampingRate);

	json.AddValue<float>("CreateRange", &jsonData_.rhombusParticle.creaateRange);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.rhombusParticle.maxElapseTime);

	// 生成数
	json.AddValue<int>("Count", &jsonData_.rhombusParticleCount);
	json.EndGroup();
#pragma endregion

#pragma region FlickeringFlash
	json.BeginGroup("FlickeringFlash");
	// 
	json.AddValue<Vector3>("MaxRotDegree", &jsonData_.flickeringFlash.maxRotDegree);
	// 大きさ
	json.AddValue<Vector3>("MaxScale", &jsonData_.flickeringFlash.maxScale);
	json.AddValue<Vector3>("MinScale", &jsonData_.flickeringFlash.minScale);
	// 色
	json.AddValue<Color>("Color", &jsonData_.flickeringFlash.color);

	// 大きさ
	json.AddValue<float>("MaxScaleTime", &jsonData_.flickeringFlash.maxScaleTime);
	json.AddValue<float>("MinScaleTime", &jsonData_.flickeringFlash.minScaleTime);
	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.flickeringFlash.maxElapseTime);

	// 生成数
	json.AddValue<int>("Count", &jsonData_.flickeringFlashCount);
	json.EndGroup();
#pragma endregion

#pragma region NoticeCircle
	json.BeginGroup("NoticeCircle");

	json.BeginGroup("EasingData");
	json.BeginGroup("Scale");
	// 最大サイズ
	json.AddValue<Vector3>("Max", &jsonData_.noticeCircle.maxScale);
	// 最小サイズ
	json.AddValue<Vector3>("Min", &jsonData_.noticeCircle.minScale);
	json.AddValue<float>("MaxTime", &jsonData_.noticeCircle.maxScaleTime);
	json.AddValue<float>("MinTime", &jsonData_.noticeCircle.minScaleTime);
	json.EndGroup();
	json.EndGroup();

	// 色
	json.AddValue<Color>("Color", &jsonData_.noticeCircle.color);

	// パーティクルが存在できる時間
	json.AddValue<float>("ElapseTime", &jsonData_.noticeCircle.maxElapseTime);
	json.EndGroup();
#pragma endregion
}

void SheathBreakEffect::SetJsonData() {
	json_.Init("SheathBreakEffect.json");

#pragma region RhombusParticle
	json_.BeginGroup("RhombusParticle");
	json_.BeginGroup("RandomValueMinMax");
	// 速度
	json_.BeginGroup("Velocity");
	json_.AddValue<Vector3>("Max", &jsonData_.rhombusParticle.velLimit.max);
	json_.AddValue<Vector3>("Min", &jsonData_.rhombusParticle.velLimit.min);
	json_.EndGroup();
	// サイズ
	json_.BeginGroup("Scale");
	json_.AddValue<float>("Max", &jsonData_.rhombusParticle.scaleLimit.max);
	json_.AddValue<float>("Min", &jsonData_.rhombusParticle.scaleLimit.min);
	json_.EndGroup();

	json_.EndGroup();

	// 色
	json_.AddValue<Color>("Color", &jsonData_.rhombusParticle.color);
	// 最大サイズ
	json_.AddValue<Vector3>("MaxScale", &jsonData_.rhombusParticle.maxScale);
	// 初速
	json_.AddValue<Vector3>("FirstVelocity", &jsonData_.rhombusParticle.firstVel);
	// 重力加速度
	json_.AddValue<float>("Acceleration", &jsonData_.rhombusParticle.acceleration);
	// 速度倍率
	json_.AddValue<float>("Multiply", &jsonData_.rhombusParticle.multiply);
	// 速度減衰率
	json_.AddValue<float>("DamplingRate", &jsonData_.rhombusParticle.dampingRate);

	json_.AddValue<float>("CreateRange", &jsonData_.rhombusParticle.creaateRange);

	// パーティクルが存在できる時間
	json_.AddValue<float>("ElapseTime", &jsonData_.rhombusParticle.maxElapseTime);

	// 生成数
	json_.AddValue<int>("Count", &jsonData_.rhombusParticleCount);
	json_.EndGroup();
#pragma endregion

#pragma region FlickeringFlash
	json_.BeginGroup("FlickeringFlash");
	// 
	json_.AddValue<Vector3>("MaxRotDegree", &jsonData_.flickeringFlash.maxRotDegree);
	// 大きさ
	json_.AddValue<Vector3>("MaxScale", &jsonData_.flickeringFlash.maxScale);
	json_.AddValue<Vector3>("MinScale", &jsonData_.flickeringFlash.minScale);
	// 色
	json_.AddValue<Color>("Color", &jsonData_.flickeringFlash.color);

	// 大きさ
	json_.AddValue<float>("MaxScaleTime", &jsonData_.flickeringFlash.maxScaleTime);
	json_.AddValue<float>("MinScaleTime", &jsonData_.flickeringFlash.minScaleTime);
	// パーティクルが存在できる時間
	json_.AddValue<float>("ElapseTime", &jsonData_.flickeringFlash.maxElapseTime);

	// 生成数
	json_.AddValue<int>("Count", &jsonData_.flickeringFlashCount);
	json_.EndGroup();
#pragma endregion

#pragma region NoticeCircle
	json_.BeginGroup("NoticeCircle");

	json_.BeginGroup("EasingData");
	json_.BeginGroup("Scale");
	// 最大サイズ
	json_.AddValue<Vector3>("Max", &jsonData_.noticeCircle.maxScale);
	// 最小サイズ
	json_.AddValue<Vector3>("Min", &jsonData_.noticeCircle.minScale);
	json_.AddValue<float>("MaxTime", &jsonData_.noticeCircle.maxScaleTime);
	json_.AddValue<float>("MinTime", &jsonData_.noticeCircle.minScaleTime);
	json_.EndGroup();
	json_.EndGroup();

	// 色
	json_.AddValue<Color>("Color", &jsonData_.noticeCircle.color);

	// パーティクルが存在できる時間
	json_.AddValue<float>("ElapseTime", &jsonData_.noticeCircle.maxElapseTime);
	json_.EndGroup();
#pragma endregion

	json_.CheckJsonFile();
}

void SheathBreakEffect::Add(const Vector3& pos) {
	emitterPos_ = pos;
	// リング
	// すでに生成しているなら解放してから生成
	if (noticeCircle_) {
		delete noticeCircle_;
		noticeCircle_ = nullptr;
	}
	noticeCircle_ = new NoticeCircle();
	noticeCircle_->SetNoticeCircleJsonData(jsonData_.noticeCircle);
	noticeCircle_->Create(pos);

	isCreate_ = true;
}