#include "Walk.h"
#include "../../Move.h"
#include "../../../../Player.h"

Walk::Walk(Move* moveSystem, Player* player, const float& moveMultiply) {
	moveSystem_ = moveSystem;
	player_ = player;

	// 状態の名前を付与
	stateName_ = "Walk";

	// 移動速度の倍率を設定
	moveSystem_->SetMoveMultiply(moveMultiply);

	AnimCommand();
}

Walk::~Walk() {
	player_->StopAnimation(LWP::Resource::Animation::TrackType::Blend);
	player_->SetAnimationPlaySpeed(1.0f);
}

void Walk::Initialize() {

}

void Walk::Update() {
	player_->SetBlendT(moveSystem_->GetStickStrength());
}

void Walk::AnimCommand() {
	if (!player_->GetAnimation()->GetPlaying("Walk", LWP::Resource::Animation::TrackType::Main)) {
		player_->StartAnimation("Walk", 0.1f, 0.0f, LWP::Resource::Animation::TrackType::Main);
		player_->SetIsLoopAnimation(true, LWP::Resource::Animation::TrackType::Main);
	}
	if (!player_->GetAnimation()->GetPlaying("Run", LWP::Resource::Animation::TrackType::Blend)) {
		// アニメーションを変更
		player_->StartAnimation("Run", 0.1f, 0.0f, LWP::Resource::Animation::TrackType::Blend);
		player_->SetIsLoopAnimation(true, LWP::Resource::Animation::TrackType::Blend);
	}
	player_->GetAnimation()->Sync();
	player_->SetBlendT(moveSystem_->GetStickStrength());
}