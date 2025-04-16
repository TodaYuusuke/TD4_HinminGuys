#include "NormalEnemy.h"
#include "../../Player/Player.h"
#include "State/NormalEnemyIdle.h"
#include "../../DirectXGame/Engine/primitive/model/Material.h"

using namespace LWP::Primitive;

void NormalEnemy::Initialize(Player* player, const Vector3& position)
{
	model_.LoadShortPath("player/Player_Simple.gltf");
	model_.materials["material"].color = { 1.0f,0.0f,0.0f,1.0f };
	//アニメーションロード
	animation_.LoadFullPath("resources/model/player/Player_Simple.gltf", &model_);
	SetPlayer(player);
	position_ = position;
	model_.worldTF.translation = position_;
	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.5f, 0.5f, 0.5f };
	state_ = std::make_unique<NormalEnemyIdle>();
	state_->Initialize(this);
}

void NormalEnemy::Update()
{

	//デルタタイムが0.0f以下の時、更新しない
	if (LWP::Info::GetDeltaTime() <= 0.0f) {
		return;
	}

	//現在の状態を更新
	state_->Update();

}

