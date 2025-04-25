#include "Boss.h"
#include "../../../Player/Player.h"
#include "../State/BossIdle.h"

void Boss::Initialize(Player* player, const Vector3& position)
{
	model_.LoadShortPath("player/Player_Simple.gltf");
	model_.materials["material"].color = { 1.0f,0.0f,0.0f,1.0f };
	animation_.LoadFullPath("resources/model/player/Player_Simple.gltf", &model_);
	animation_.Play("Idle", true);
	SetPlayer(player);
	model_.worldTF.translation = position;
	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.8f, 0.8f, 0.8f };
	state_ = new BossIdle();
	state_->Initialize(this);
}

void Boss::Update()
{

	IEnemy::Update();

}
