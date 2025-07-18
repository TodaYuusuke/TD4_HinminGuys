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
	if (!player_->GetAnimation()->GetPlaying("Dash", LWP::Resource::Animation::TrackType::Main) &&
		!player_->GetAnimation()->GetPlaying("Walk", LWP::Resource::Animation::TrackType::Main) &&
		!player_->GetAnimation()->GetPlaying("Run", LWP::Resource::Animation::TrackType::Blend)) {
		moveEffectType_ = MoveEffectType::kNone;
		currentMoveFrame_ = 0.0f;
		return;
	}

	float t = 0.0f;
	currentMoveFrame_ += HitStopController::GetInstance()->GetDeltaTime();
	if (player_->GetAnimation()->GetPlaying("Dash", LWP::Resource::Animation::TrackType::Main) || player_->GetAnimation()->GetPlaying("Walk", LWP::Resource::Animation::TrackType::Main)) {
		t = player_->GetAnimation()->GetTotalSeconds(LWP::Resource::Animation::TrackType::Main) / player_->GetAnimation()->GetPlayBackSpeed(LWP::Resource::Animation::TrackType::Main) * 60.0f;
	}
	else if (player_->GetAnimation()->GetPlaying("Run", LWP::Resource::Animation::TrackType::Blend)) {
		t = player_->GetAnimation()->GetTotalSeconds(LWP::Resource::Animation::TrackType::Blend) / player_->GetAnimation()->GetPlayBackSpeed(LWP::Resource::Animation::TrackType::Blend) * 60.0f;
	}

	// 地面につく足は左足
	if (t / 1.05f <= currentMoveFrame_) {
		currentMoveFrame_ += -t;
		moveEffectType_ = MoveEffectType::kLeft;
	}
	// 地面につく足は右足
	else if (t / 2.0f <= currentMoveFrame_) {
		moveEffectType_ = MoveEffectType::kRight;
	}

	// パーティクル生成
	if (moveEffectType_ == MoveEffectType::kLeft && preMoveEffectType_ != MoveEffectType::kLeft) {
		Vector3 pos = player_->GetModel()->GetJoint("Foot.R")->localTF.translation + player_->GetWorldTF()->GetWorldPosition();
		pos.y = 0.0f;
		CreateMoveParticle(pos);
	}
	else if (moveEffectType_ == MoveEffectType::kRight && preMoveEffectType_ != MoveEffectType::kRight) {
		Vector3 pos = player_->GetModel()->GetJoint("Foot.L")->localTF.translation + player_->GetWorldTF()->GetWorldPosition();
		pos.y = 0.0f;
		CreateMoveParticle(pos);
	}

	preMoveEffectType_ = moveEffectType_;
}