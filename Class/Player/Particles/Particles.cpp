#include "Particles.h"
#include "../Player.h"

using namespace LWP;
using namespace LWP::Math;

Particles::Particles(Player* player, FollowCamera* followCamera) {
	player_ = player;
	followCamera_ = followCamera;

	// パリィ
	parryEffect_ = std::make_unique<ParryEffect>(player_, followCamera_);
	parryEffect_->Initialize();
	// 回避
	evasionEffect_ = std::make_unique<EvasionEffect>(player_);
	evasionEffect_->model.LoadCube();
	// 移動
	moveEffect_ = std::make_unique<MoveEffect>(player_, followCamera_);
	moveEffect_->Initialize();




	largeFlashes_ = std::make_unique<LargeFlashes>(player_, followCamera_);
	shortFlashes_ = std::make_unique<ShortFlashes>(player_, followCamera_);
	rings_ = std::make_unique<Rings>(player_, followCamera_);
	sparks_ = std::make_unique<Sparks>(player_, followCamera_);
	floatParticle_ = std::make_unique<FloatParticle>(player_);
	floatParticle_->model.LoadCube();
}

void Particles::Initialize() {
	// JSONデータを作成
	CreateJsonData();
}

void Particles::Update() {
	largeFlashes_->Update();
	shortFlashes_->Update();
	rings_->Update();
	sparks_->Update();
	shortFlashes_->Update();


	//parryEffect_->Update();
	//moveEffect_->Update();

	isPreJustParry_ = player_->GetSystemManager()->GetIsJustParry();
	isPreGoodParry_ = player_->GetSystemManager()->GetIsGoodParry();
}

void Particles::CreateJsonData() {
	json_.Init("Particles.json");

	// パリィ時のパーティクル
	json_.BeginGroup("Parry");
	parryEffect_->SetJsonData(json_);
	json_.EndGroup();

	// 回避時のパーティクル
	json_.BeginGroup("Evasion");
	evasionEffect_->SetJsonData(json_);
	json_.EndGroup();

	// パリィ時のパーティクル
	json_.BeginGroup("Move");
	moveEffect_->SetJsonData(json_);
	json_.EndGroup();

	json_.BeginGroup("LargeFlash");
	largeFlashes_->SetJsonData(json_);
	json_.EndGroup();
	json_.BeginGroup("ShortFlash");
	shortFlashes_->SetJsonData(json_);
	json_.EndGroup();
	json_.BeginGroup("Ring");
	rings_->SetJsonData(json_);
	json_.EndGroup();
	json_.BeginGroup("Spark");
	sparks_->SetJsonData(json_);
	json_.EndGroup();
	json_.BeginGroup("FloatParticle");
	floatParticle_->SetJsonData(json_);
	json_.EndGroup();

	json_.CheckJsonFile();
}

void Particles::DebugGui() {
	// 調整項目
	json_.DebugGUI();

	ImGui::DragFloat3("EmitterPos(Debug)", &debugEmitterPos_.x, 0.01f);
	// パリィ時のパーティクル生成
	if (ImGui::Button("Create JustParry Particle")) {
		CreateJustParryParticle(debugEmitterPos_);
	}
	if (ImGui::Button("Create GoodParry Particle")) {
		CreateGoodParryParticle(debugEmitterPos_);
	}
	// 回避時のパーティクル生成
	if (ImGui::Button("Create Evasion Particle")) {
		CreateEvasionParticle(debugEmitterPos_);
	}
	// 移動時のパーティクル生成
	if (ImGui::Button("Create Move Particle")) {
		CreateMoveParticle(debugEmitterPos_);
	}

	if (ImGui::Button("Create Large Particle")) {
		largeFlashes_->Add(3, debugEmitterPos_);
	}
	if (ImGui::Button("Create Short Particle")) {
		shortFlashes_->Add(12, debugEmitterPos_);
	}
	if (ImGui::Button("Create Ring Particle")) {
		rings_->Add(1, debugEmitterPos_);
	}
	if (ImGui::Button("Create Spark Particle")) {
		sparks_->Add(50, debugEmitterPos_);
	}
	if (ImGui::Button("Create Float Particle")) {
		floatParticle_->Add(10, debugEmitterPos_);
	}

	// パーティクルの詳細
	if (ImGui::TreeNode("Detail")) {
		// パリィ
		if (ImGui::TreeNode("Parry")) {
			parryEffect_->DebugGui();
			ImGui::TreePop();
		}
		// 回避
		if (ImGui::TreeNode("Evasion")) {
			evasionEffect_->DebugGui();
			ImGui::TreePop();
		}
		// 移動
		if (ImGui::TreeNode("Move")) {
			moveEffect_->DebugGui();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void Particles::CreateJustParryParticle(const LWP::Math::Vector3& pos) {
	parryEffect_->CreateJustParticles(pos);
}

void Particles::CreateGoodParryParticle(const LWP::Math::Vector3& pos) {
	parryEffect_->CreateGoodParticles(pos);
}

void Particles::CreateParryParticle(const LWP::Math::Vector3& pos) {
	// ジャストパリィ
	if (player_->GetSystemManager()->GetIsJustParry() && !isPreJustParry_) {
		parryEffect_->CreateJustParticles(pos);
	}
	// 弱パリィ
	if (player_->GetSystemManager()->GetIsGoodParry() && !isPreGoodParry_) {
		parryEffect_->CreateGoodParticles(pos);
	}
}

void Particles::CreateEvasionParticle(const LWP::Math::Vector3& pos) {
	evasionEffect_->Add(evasionEffect_->GetParticleJsonData().count, pos);
}

void Particles::CreateMoveParticle(const LWP::Math::Vector3& pos) {
	moveEffect_->CreateDustClouds(pos);
}