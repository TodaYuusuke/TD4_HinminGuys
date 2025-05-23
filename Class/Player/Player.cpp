#include "Player.h"
#include "../Enemy/EnemyManager.h"
#include "../GameMask.h"

using namespace LWP::Utility;
using namespace GameMask;

Player::Player(LWP::Object::Camera* camera, EnemyManager* enemyManager, FollowCamera* followCamera)
	: aabb_(collider_.SetBroadShape(LWP::Object::Collider::AABB()))
{
	pCamera_ = camera;
	enemyManager_ = enemyManager;
	followCamera_ = followCamera;

	// モデルを読み込む
	model_.LoadShortPath("player/Player_Simple.gltf");
	animation_.LoadFullPath("resources/model/player/Player_Simple.gltf", &model_);
	animation_.Play("Idle");

	// 当たり判定を作成
	CreateCollision();

	// HPを作成
	hp_.Initialize();
	sheathGauge_.Initialize();
}

void Player::Initialize() {
	inputHandler_ = InputHandler::GetInstance();

	// 自機機能を生成
	CreateSystems();

	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.5f, 0.5f, 0.5f };
}

void Player::Update() {
	// 各機能
	systemManager_->Update();

	// 速度を加算
	model_.worldTF.translation += systemManager_->GetVelocity();
	// 角度を代入
	model_.worldTF.rotation = systemManager_->GetRotate();

	// HP
	hp_.Update();
	sheathGauge_.Update();
}

void Player::Reset() {
	// 各機能
	systemManager_->Reset();
}

void Player::DebugGUI() {
#ifdef _DEBUG
	// 各機能
	if (ImGui::TreeNode("Systems")) {
		systemManager_->DebugGUI();
		ImGui::TreePop();
	}
	// HP
	hp_.DebugGUI();
	sheathGauge_.DebugGUI();
	// アニメーション
	if (ImGui::TreeNode("Animation")) {
		animation_.DebugGUI();
		ImGui::TreePop();
	}
	// WorldTransform
	model_.worldTF.DebugGUI();
#endif // DEBUG
}

void Player::CreateSystems() {
	// 各機能生成
	systemManager_ = std::make_unique<SystemManager>(this, enemyManager_, followCamera_, pCamera_);
	systemManager_->Initialize();
}

void Player::CreateCollision() {
	// 体の判定生成
	collider_.SetFollow(&model_.worldTF);
	collider_.isActive = true;
	collider_.worldTF.translation = { 0.0f, 1.0f, 0.0f };
	// 自機の所属しているマスクを設定
	collider_.mask.SetBelongFrag(GetPlayer());
	// 当たり判定をとる対象のマスクを設定
	collider_.mask.SetHitFrag(GetAttack());
	collider_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		};
}
