#include "Idle.h"
#include "../../Move.h"
#include "../../../../Player.h"

Idle::Idle(Move* moveSystem, Player* player) {
	moveSystem_ = moveSystem;
	player_ = player;

	// 状態の名前を付与
	stateName_ = "Idle";

	player_->SetBlendT(0.0f);
	player_->SetIsLoopAnimation(false);

	if (moveSystem_->GetPreMoveState(Move::MoveState::kWalk)) {
		isPreWalkState_ = true;
	}
	if (moveSystem_->GetPreMoveState(Move::MoveState::kDash)) {
		isPreWalkState_ = true;
	}
}

void Idle::Initialize() {

}

void Idle::Update() {
	// ループしていないアニメーションが終了しているか
	if (!player_->GetAnimation()->GetPlaying() && !isActive_) {
		AnimCommand();
	}

	// 前回walk状態だったのならアニメーション開始
	if (isPreWalkState_) {
		if (!isActive_) {
			AnimCommand();
		}
	}
}

void Idle::AnimCommand() {
	// アニメーションを変更
	player_->SetBlendT(0.0f);
	player_->StopAnimation(LWP::Resource::Animation::TrackType::Main);
	player_->StopAnimation(LWP::Resource::Animation::TrackType::Blend);
	player_->StartAnimation("Idle", 0.2f, 0.0f);
	player_->SetIsLoopAnimation(true);

	isActive_ = true;
}
