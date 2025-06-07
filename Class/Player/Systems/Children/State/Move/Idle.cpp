#include "Idle.h"
#include "../../Move.h"
#include "../../../../Player.h"

Idle::Idle(Move* moveSystem, Player* player) {
	moveSystem_ = moveSystem;
	player_ = player;

	// 状態の名前を付与
	stateName_ = "Idle";

	AnimCommand();
}

void Idle::Initialize() {

}

void Idle::Update() {
	//if(!player_->GetAnimation()->GetPlaying("Dash"))
}

void Idle::AnimCommand() {
	// アニメーションを変更
	player_->StartAnimation("Idle", 0.1f, 0.0f);
	player_->SetIsLoopAnimation(true);
	isActive_ = true;
}
