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
	// ヒットストップの管理クラス
	hitStopController_ = HitStopController::GetInstance();

	// パーティクルの管理クラス
	particles_ = std::make_unique<Particles>(this, followCamera_);
	particles_->Initialize();

	// パラメータ管理クラス生成
	playerParameter_ = std::make_unique<PlayerParameter>(this);
	playerParameter_->Initialize();

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

	// パラメータ
	playerParameter_->Update();

	// 速度を加算
	model_.worldTF.translation += systemManager_->GetVelocity();
	// 角度を代入S
	model_.worldTF.rotation = LWP::Utility::Interpolation::SlerpQuaternion(model_.worldTF.rotation, systemManager_->GetQuat(), 0.25f);

	// 移動制限
	LimitMoveArea();

	// 無敵時間
	InvinsibleUpdate();

	// パーティクル管理クラス
	particles_->Update();
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
	if (ImGui::TreeNode("Particles")) {
		particles_->DebugGui();
		ImGui::TreePop();
	}
	// パラメータ
	if (ImGui::TreeNode("Parameter")) {
		playerParameter_->DebugGui();
		ImGui::TreePop();
	}

	if (ImGui::Button("Take Damage")) {
		TakeDamage(10.0f);
	}
#endif // DEBUG
}

void Player::TakeDamage(const float& damageValue) {
	// 自機が無敵中ならダメージ判定をとらない
	if (!collider_.isActive) { return; }
	// 全ての機能をリセット
	Reset();
	// HPゲージ変動
	uiManager_->ChangeHPGauge(damageValue, playerParameter_->defenseMultiply_);
	// ダメージ機能を生成しすべての行動キャンセル
	systemManager_->StartDamageResponse();
}

void Player::TakeSheathDamage(const float& damageValue, const float& multiply) {
	// 鞘破壊中ならゲージ減少はなし
	if (systemManager_->GetSheathSystem()->GetIsBreak()) { return; }

	// 鞘ゲージ変動
	uiManager_->ChangeSheathGauge(damageValue, multiply);
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
	if (systemManager_->GetInvisibleTime() >= 0.0f) {
		collider_.isActive = false;
	}
	else {
		collider_.isActive = true;
	}
}

void Player::LimitMoveArea() {
	// 鞘を投げた後鞘を中心に移動制限をかける(円形)
	if (systemManager_->GetSheathSystem()->GetSheathState()->GetStateName() == "SwordDrawn") {
		systemManager_->GetSheathSystem()->ClampToCircle(model_.worldTF.translation);
	}
}

void Player::CreateParryParticle(const LWP::Math::Vector3& pos) {
	particles_->CreateParryParticle(pos);
}

void Player::CreateEvasionParticle(const LWP::Math::Vector3& pos) {
	particles_->CreateEvasionParticle(pos);
}
