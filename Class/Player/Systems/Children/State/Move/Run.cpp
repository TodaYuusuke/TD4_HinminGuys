#include "Run.h"
#include "../../Move.h"
#include "../../../../Player.h"

Run::Run(Move* moveSystem, Player* player, const float& moveMultiply) {
	moveSystem_ = moveSystem;
	player_ = player;

	// 状態の名前を付与
	stateName_ = "Run";

	// 移動速度の倍率を設定
	moveSystem_->SetMoveMultiply(moveMultiply);

	AnimCommand();
}

void Run::Initialize() {

}

void Run::Update() {

}

void Run::AnimCommand() {
	// アニメーションを変更
	player_->StartAnimation("Run", 0.1f, 0.0f);
	player_->SetAnimationPlaySpeed(1.0f);
	player_->SetIsLoopAnimation(true);
}