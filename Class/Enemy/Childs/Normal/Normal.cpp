#include "Normal.h"
#include "../../../Player/Player.h"
#include "../State/NormalIdle.h"
#include "../../DirectXGame/Engine/primitive/model/Material.h"

using namespace LWP::Primitive;

void Normal::Initialize(Player* player, const Vector3& position)
{
	model_.LoadShortPath("player/Player_Simple.gltf");
	model_.materials["material"].color = { 1.0f,0.0f,0.0f,1.0f };
	//アニメーションロード
	animation_.LoadFullPath("resources/model/player/Player_Simple.gltf", &model_);
	SetPlayer(player);
	model_.worldTF.translation = position;
	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.5f, 0.5f, 0.5f };
	state_ = new NormalIdle();
	state_->Initialize(this);
	attackHitBox_.isActive = false;
	attackHitBox_.isShowWireFrame = false;
	hitBox_.SetFollowPtr(&model_.worldTF);
}

void Normal::Update()
{

	IEnemy::Update();

}

