#include "Walk.h"
#include "../../Move.h"
#include "../../../../Player.h"

Walk::Walk(Move* moveSystem, Player* player) {
	moveSystem_ = moveSystem;
	player_ = player;

	// 状態の名前を付与
	stateName_ = "Walk";

	// アニメーションを変更
	player_->StartAnimation("Run", 1.0f, 0.0f);
	player_->SetAnimationPlaySpeed(1.0f);
	player_->SetIsLoopAnimation(true);
}

void Walk::Initialize() {

}

void Walk::Update() {

}
