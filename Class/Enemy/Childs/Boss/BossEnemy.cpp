#include "BossEnemy.h"
#include "../../../Player/Player.h"
#include "State/BossEnemyIdle.h"

void BossEnemy::Initialize(Player* player, const Vector3& position)
{
	model_.LoadShortPath("player/Player_Simple.gltf");
	model_.materials["material"].color = { 1.0f,0.0f,0.0f,1.0f };
	animation_.LoadFullPath("resources/model/player/Player_Simple.gltf", &model_);
	animation_.Play("Idle", true);
	SetPlayer(player);
	model_.worldTF.translation = position;
	// 大きさを一時的に調整
	model_.worldTF.scale = { 0.8f, 0.8f, 0.8f };
	state_ = std::make_unique<BossEnemyIdle>();
	state_->Initialize(this);
}

void BossEnemy::Update()
{

	//デルタタイムが0.0f以下の時、更新しない
	if (LWP::Info::GetDeltaTime() <= 0.0f) {
		return;
	}

	//現在の状態を更新
	state_->Update();

	//プレイヤーとの距離を計算
	Vector3 velocity = GetPlayerPosition() - GetPosition();
	SetDistFromPlayer(velocity.Length());

}
