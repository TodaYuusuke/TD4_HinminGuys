#include "Idle.h"
#include "../../Move.h"
#include "../../../../Player.h"

Idle::Idle(Move* moveSystem, Player* player) {
	moveSystem_ = moveSystem;
	player_ = player;

	// 状態の名前を付与
	stateName_ = "Idle";

	// アニメーションを変更
	player_->StartAnimation("Idle", 0.25f, 0.0f);
	player_->SetAnimationPlaySpeed(1.0f);
	player_->SetIsLoopAnimation(true);
}

void Idle::Initialize() {

}

void Idle::Update()
{
}
