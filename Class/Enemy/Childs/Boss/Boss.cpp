#include "Boss.h"
#include "../../../Player/Player.h"
#include "../State/BossIdle.h"

void Boss::Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera,
	EnemyManager* manager)
{
	type_ = EnemyType::kBoss;
	model_.LoadShortPath("player/Player_Simple.gltf");
	model_.materials["Material"].color = { 1.0f,0.0f,0.0f,1.0f };
	animation_.LoadFullPath("resources/model/player/Player_Simple.gltf", &model_);
	animation_.Play("Idle", 0.1f);
	SetPlayer(player);
	camera_ = camera;
	enemyManager_ = manager;
	model_.worldTF.translation = position;
	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.8f, 0.8f, 0.8f };
	state_ = new BossIdle();
	state_->Initialize(this);
	// 刀モデルをプレイヤーの手に追従させる
	swordModel_.GetJoint("Grip")->localTF.Parent(&model_, "WeaponAnchor");
	//名前設定
	collider_.name = "Boss" + std::to_string(ID_);

}

void Boss::Update()
{

	IEnemy::Update();

}
