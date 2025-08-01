#include "Particles.h"
#include "../Player/Player.h"

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
	dustClouds_ = std::make_unique<DustClouds>("Effect/Particle.png");
	dustClouds_->Initialize();




	largeFlashes_ = std::make_unique<LargeFlashes>(followCamera_, "Effect/Spark.png");
	shortFlashes_ = std::make_unique<ShortFlashes>(followCamera_, "Effect/Spark.png");
	rings_ = std::make_unique<Rings>("Effect/CircleParticle.png");
	sparks_ = std::make_unique<Sparks>("Effect/Spark.png");
	floatParticle_ = std::make_unique<FloatParticle>(player_);
	floatParticle_->model.LoadCube();
	enemySpawnParticles_ = std::make_unique<EnemySpawnParticles>("Effect/Rock.png");
	enemySpawnParticles_->Initialize();
	enemyDeadParticles_ = std::make_unique<EnemyDeadParticles>("Effect/Particle.png");
	enemyDeadParticles_->Initialize();
	attackHitEffect_ = std::make_unique<AttackHitEffect>();
	attackHitEffect_->Initialize();
	crackEffect_ = std::make_unique<CrackEffect>("Effect/Crack.png");
	crackEffect_->Initialize();
	weakCrackEffect_ = std::make_unique<CrackEffect>("Effect/Crack.png");
	weakCrackEffect_->Initialize();
}

Particles::~Particles() {
	parryEffect_.reset();
	evasionEffect_.reset();
	dustClouds_.reset();
	largeFlashes_.reset();
	shortFlashes_.reset();
	rings_.reset();
	sparks_.reset();
	floatParticle_.reset();
	enemySpawnParticles_.reset();
	enemyDeadParticles_.reset();
	attackHitEffect_.reset();
	crackEffect_.reset();
	weakCrackEffect_.reset();
	for (EnemySpawnParticles* p : spawnParticles_) {
		delete p;
	}
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
	enemySpawnParticles_->Update();
	spawnParticles_.remove_if([](EnemySpawnParticles* p) {
		if (p->GetCurrentFrame() <= 0.0f && p->GetEmptyParticles()) {
			delete p;
			return true;
		}
		return false;
		});
	for (EnemySpawnParticles* p : spawnParticles_) {
		p->Update();
	}
	enemyDeadParticles_->Update();
	attackHitEffect_->Update();
	crackEffect_->Update();
	weakCrackEffect_->Update();
	parryEffect_->Update();
	dustClouds_->Update();

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

	// 移動時の土煙
	json_.BeginGroup("Move");
	dustClouds_->SetJsonData(json_);
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
	json_.BeginGroup("EnemySpawnParticle");
	enemySpawnParticles_->SetJsonData(json_);
	json_.EndGroup();
	json_.BeginGroup("EnemyDeadParticle");
	enemyDeadParticles_->SetJsonData(json_);
	json_.EndGroup();
	json_.BeginGroup("AttackHitEffect");
	attackHitEffect_->SetJsonData(json_);
	json_.EndGroup();
	json_.BeginGroup("CrackEffect");
	crackEffect_->SetJsonData(json_);
	json_.EndGroup();
	json_.BeginGroup("WeakCrackEffect");
	weakCrackEffect_->SetJsonData(json_);
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
	if (ImGui::Button("Create Spawn Particle")) {
		//enemySpawnParticles_->Start(60.0f, debugEmitterPos_);
		CreateEnemySpawnParticles(0.5f, debugEmitterPos_);
	}
	if (ImGui::Button("Create Dead Particle")) {
		enemyDeadParticles_->Add(5, debugEmitterPos_);
	}
	if (ImGui::Button("Create Attack Hit Effect")) {
		attackHitEffect_->Add(debugEmitterPos_);
	}
	if (ImGui::Button("Create Crack Effect")) {
		crackEffect_->Add(debugEmitterPos_);
	}
	if (ImGui::Button("Create Weak Crack Effect")) {
		weakCrackEffect_->Add(debugEmitterPos_);
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
			//dustClouds_->DebugGui();
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

void Particles::CreateFootSparks(LWP::Math::Vector3 pos) {
	parryEffect_->CreateFootSparks(pos);
}

void Particles::CreateEvasionParticle(const LWP::Math::Vector3& pos) {
	evasionEffect_->Add(evasionEffect_->GetParticleJsonData().count, pos);
}

void Particles::CreateMoveParticle(const LWP::Math::Vector3& pos) {
	dustClouds_->Add(3, pos);
}