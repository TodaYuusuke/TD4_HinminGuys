#include "Normal.h"
#include "../../../Player/Player.h"
#include "../State/NormalIdle.h"
#include "../../DirectXGame/Engine/primitive/model/Material.h"
#include "../../../GameMask.h"

using namespace LWP::Primitive;
using namespace GameMask;

void Normal::Initialize(Player* player, const Vector3& position)

{
	model_.LoadShortPath("player/Player_Simple.gltf");
	type_ = EnemyType::kNormal;
	//アニメーションロード
	animation_.LoadFullPath("resources/model/player/Player_Simple.gltf", &model_);
	model_.materials["Material"].color = { 1.0f,0.0f,0.0f,1.0f };
	SetPlayer(player);
	model_.worldTF.translation = position;
	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.5f, 0.5f, 0.5f };
	state_ = new NormalIdle();
	state_->Initialize(this);

	// 体の判定生成
	collider_.SetFollow(&model_.worldTF);
	collider_.isActive = true;
	collider_.worldTF.translation = { 0.0f, 1.0f, 0.0f };
	// 自機の所属しているマスクを設定
	collider_.mask.SetBelongFrag(GetEnemy());
	// 当たり判定をとる対象のマスクを設定
	collider_.mask.SetHitFrag(GetAttack());
	collider_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		};

}

void Normal::Update()
{

	IEnemy::Update();

	model_.DebugGUI();

}

