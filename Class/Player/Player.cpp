#include "Player.h"
#include "../Enemy/EnemyManager.h"
#include "../GameMask.h"

using namespace LWP::Utility;
using namespace GameMask;

Player::Player(LWP::Object::Camera* camera, EnemyManager* enemyManager, FollowCamera* followCamera, UIManager* uiManager)
	: aabb_(collider_.SetBroadShape(LWP::Object::Collider::AABB()))
{
	pCamera_ = camera;
	enemyManager_ = enemyManager;
	followCamera_ = followCamera;
	uiManager_ = uiManager;

	// モデルを読み込む
	model_.LoadShortPath("player/Player_Simple.gltf");
	animation_.LoadFullPath("resources/model/player/Player_Simple.gltf", &model_);
	animation_.Play("Idle");
	// 刀
	swordModel_.LoadShortPath("player/SimpleWeapon.gltf");
	// 鞘
	sheathModel_.LoadShortPath("player/Sheath.gltf");

	// 当たり判定を作成
	CreateCollision();
}

void Player::Initialize() {
	inputHandler_ = InputHandler::GetInstance();

	// 自機機能を生成
	CreateSystems();

	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.5f, 0.5f, 0.5f };

	json_.Init("Player.json");
	// 当たり判定
	json_.BeginGroup("Collider")
		.AddValue<Vector3>("Min", &aabb_.min)
		.AddValue<Vector3>("Max", &aabb_.max)
		.EndGroup()
		.CheckJsonFile();

	// 刀モデルをプレイヤーの手に追従させる
	swordModel_.GetJoint("Grip")->localTF.Parent(&model_, "WeaponAnchor");
	// 鞘モデルを刀モデルに追従
	sheathModel_.GetJoint("Sheath")->localTF.Parent(&swordModel_, "Sheath");
}

void Player::Update() {
	// 体力がないなら自機は死亡
	if (uiManager_->GetHPGauge().GetIsBelowPercent(0.0f)) { 
		isAlive_ = false; 
		return;
	}

	// 各機能
	systemManager_->Update();

	// 速度を加算
	model_.worldTF.translation += systemManager_->GetVelocity();
	// 角度を代入S
	model_.worldTF.rotation = systemManager_->GetQuat();

	// 移動制限
	LimitMoveArea();

	// 無敵時間
	InvinsibleUpdate();
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
	// WorldTransform
	model_.worldTF.DebugGUI();
	// 当たり判定
	if (ImGui::TreeNode("Collider")) {
		json_.DebugGUI();
		ImGui::TreePop();
	}
	// アニメーション
	if (ImGui::TreeNode("Animation")) {
		animation_.DebugGUI();
		ImGui::TreePop();
	}

#endif // DEBUG
}

void Player::ResetSystems() {

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
	collider_.enterLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		};
}

void Player::InvinsibleUpdate() {
	if (systemManager_->GetInvisibleTime() != 0.0f) {
		collider_.isActive = false;
	}
	else {
		collider_.isActive = true;
	}
}

void Player::LimitMoveArea() {
	//// 鞘を投げた後鞘を中心に移動制限をかける(円形)
	//if (systemManager_->GetSheathSystem()->GetSheathState()->GetStateName() == "Collect") {
	//	systemManager_->GetSheathSystem()->ClampToCircle(model_.worldTF.translation);
	//}
}