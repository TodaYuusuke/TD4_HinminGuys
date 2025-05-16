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
	//if (!isActive_ && player_->GetAnimation()->GetProgress(LWP::Resource::Animation::TrackType::Main) >= 1.0f) {
	//	isActive_ = true;
	//	player_->StopAnimation(LWP::Resource::Animation::TrackType::Main);
	//	// アニメーションを変更
	//	player_->StartAnimation("Idle", 0.1f, player_->GetAnimation()->GetProgress(LWP::Resource::Animation::TrackType::Blend), LWP::Resource::Animation::TrackType::Main);
	//	player_->SetIsLoopAnimation(true);
	//}
}

void Idle::AnimCommand() {
	//if (!player_->GetAnimation()->GetPlaying()) {
		// アニメーションを変更
		player_->StartAnimation("Idle", 0.1f, 0.0f);
		player_->SetIsLoopAnimation(true);
		isActive_ = true;
	//}
	//else {
	//	// アニメーションを変更
	//	player_->StartAnimation("Idle", 0.1f, 0.0f, LWP::Resource::Animation::TrackType::Blend);
	//	player_->SetIsLoopAnimation(true, LWP::Resource::Animation::TrackType::Blend);
	//	player_->SetIsLoopAnimation(false, LWP::Resource::Animation::TrackType::Main);
	//}
}
