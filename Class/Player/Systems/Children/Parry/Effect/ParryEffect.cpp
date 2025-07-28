#include "ParryEffect.h"
#include "../../../../../Camera/FollowCamera.h"
#include "../../../../../Player/Player.h"
#include "../../../../Math/MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

ParryEffect::ParryEffect(Player* player, FollowCamera* followCamera) {
	player_ = player;
	followCamera_ = followCamera;

	largeFlashes_ = std::make_unique<LargeFlashes>(followCamera_,"Effect/Spark.png");
	shortFlashes_ = std::make_unique<ShortFlashes>(followCamera_,"Effect/Spark.png");
	rings_ = std::make_unique<Rings>("Effect/CircleParticle.png");
	sparks_ = std::make_unique<Sparks>("Effect/Spark.png");
}

void ParryEffect::Initialize() {}

void ParryEffect::Update() {
	largeFlashes_->Update();
	shortFlashes_->Update();
	rings_->Update();
	sparks_->Update();
}

void ParryEffect::DebugGui() {
	// パーティクル数
	//jsonData_.count = lineParticleCount + circleParticleCount;
	//DebugGUI();
}

void ParryEffect::CreateJustParticles(Vector3 pos) {
	// 発生地点
	emitterPos_ = pos;

	// 大きい閃光
	largeFlashes_->Add(3, emitterPos_);
	// 小さい閃光
	shortFlashes_->Add(12, emitterPos_);
	// リング
	rings_->Add(1, emitterPos_);
	// 火花
	sparks_->Add(50, emitterPos_);
}

void ParryEffect::CreateGoodParticles(Vector3 pos) {
	// 発生地点
	emitterPos_ = pos;

	// 小さい閃光
	shortFlashes_->Add(12, emitterPos_);
	// 火花
	sparks_->Add(50, emitterPos_);
}

void ParryEffect::SetJsonData(LWP::Utility::JsonIO& json) {
	json.BeginGroup("LargeFlash");
	largeFlashes_->SetJsonData(json);
	json.EndGroup();
	json.BeginGroup("ShortFlash");
	shortFlashes_->SetJsonData(json);
	json.EndGroup();
	json.BeginGroup("Ring");
	rings_->SetJsonData(json);
	json.EndGroup();
	json.BeginGroup("Spark");
	sparks_->SetJsonData(json);
	json.EndGroup();
}
