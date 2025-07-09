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
	evasionEffect_->Initialize();
	evasionEffect_->model.LoadCube();
	// 移動
	moveEffect_ = std::make_unique<MoveEffect>(player_, followCamera_);
	moveEffect_->Initialize();
}

void Particles::Initialize() {
	// JSONデータを作成
	CreateJsonData();
}

void Particles::Update() {
	parryEffect_->Update();
	moveEffect_->Update();

	// 移動時のパーティクル発生のタイミング
	CreateMoveParticleTiming();

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

void Particles::CreateMoveParticleTiming() {
	if (player_->GetAnimation()->GetPlaying("Run", LWP::Resource::Animation::TrackType::Blend) || player_->GetAnimation()->GetPlaying("Dash", LWP::Resource::Animation::TrackType::Main)) {
		// 右足
		if (player_->GetAnimation()->GetProgress(LWP::Resource::Animation::TrackType::Blend) >= 0.5f || player_->GetAnimation()->GetProgress(LWP::Resource::Animation::TrackType::Main) >= 0.5f && moveEffectType_ != MoveEffectType::kLeft) {
			Vector3 pos = player_->GetModel()->GetJoint("Foot.R")->localTF.translation + player_->GetWorldTF()->GetWorldPosition();
			pos.y = 0.0f;
			CreateMoveParticle(pos);
			moveEffectType_ = MoveEffectType::kLeft;
		}
		// 左足
		if (player_->GetAnimation()->GetProgress(LWP::Resource::Animation::TrackType::Blend) >= 0.95f || player_->GetAnimation()->GetProgress(LWP::Resource::Animation::TrackType::Main) >= 0.95f && moveEffectType_ != MoveEffectType::kRight) {
			Vector3 pos = player_->GetModel()->GetJoint("Foot.L")->localTF.translation + player_->GetWorldTF()->GetWorldPosition();
			pos.y = 0.0f;
			CreateMoveParticle(pos);
			moveEffectType_ = MoveEffectType::kRight;
		}
	}
}