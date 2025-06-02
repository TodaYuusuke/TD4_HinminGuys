#include "Normal.h"
#include "../../../Player/Player.h"
#include "../State/NormalIdle.h"
#include "../../DirectXGame/Engine/primitive/model/Material.h"
#include "../../../GameMask.h"

using namespace LWP::Primitive;
using namespace GameMask;

void Normal::Initialize(Player* player, const Vector3& position, LWP::Object::Camera* camera)
{
	model_.LoadShortPath("player/Player_Simple.gltf");
	type_ = EnemyType::kNormal;
	//アニメーションロード
	animation_.LoadFullPath("resources/model/player/Player_Simple.gltf", &model_);
	swordModel_.LoadShortPath("player/SimpleWeapon.gltf");
	model_.materials["Material"].color = { 1.0f,0.0f,0.0f,1.0f };
	SetPlayer(player);
	camera_ = camera;
	model_.worldTF.translation = position;
	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.5f, 0.5f, 0.5f };
	state_ = new NormalIdle();
	state_->Initialize(this);
	// 刀モデルをプレイヤーの手に追従させる
	swordModel_.GetJoint("Grip")->localTF.Parent(&model_, "WeaponAnchor");
	//名前設定
	collider_.name = "Normal" + std::to_string(ID_);
	//刀のコライダー生成
	CreateSwordCollider();

}

void Normal::Update()
{

	IEnemy::Update();

	model_.DebugGUI();

}

