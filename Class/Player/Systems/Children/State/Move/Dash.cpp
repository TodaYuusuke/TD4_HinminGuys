#include "Dash.h"
#include "../../Move.h"
#include "../../../../Player.h"

Dash::Dash(Move* moveSystem, Player* player, const float& moveMultiply) {
	moveSystem_ = moveSystem;
	player_ = player;

	// 状態の名前を付与
	stateName_ = "Dash";

	// 移動速度の倍率を設定
	moveSystem_->SetMoveMultiply(moveMultiply);

	AnimCommand();
}

void Dash::Initialize() {

}

void Dash::Update() {

}

void Dash::AnimCommand() {
	// アニメーションを変更
	player_->StartAnimation("Dash", 0.1f, 0.0f);
	player_->SetAnimationPlaySpeed(1.0f);
	player_->SetIsLoopAnimation(true);
}