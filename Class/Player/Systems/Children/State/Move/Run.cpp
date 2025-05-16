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
	//if (!isActive_ && player_->GetAnimation()->GetProgress(LWP::Resource::Animation::TrackType::Main) >= 1.0f) {
	//	isActive_ = true;
	//	player_->StopAnimation(LWP::Resource::Animation::TrackType::Main);
	//	// アニメーションを変更
	//	player_->StartAnimation("Run", 0.1f, player_->GetAnimation()->GetProgress(LWP::Resource::Animation::TrackType::Blend), LWP::Resource::Animation::TrackType::Main);
	//	player_->SetIsLoopAnimation(true);
	//}
}

void Run::Update() {

}

void Run::AnimCommand() {
	// アニメーションを変更
	player_->StartAnimation("Run", 0.1f, 0.0f);
	player_->SetAnimationPlaySpeed(1.0f);
	player_->SetIsLoopAnimation(true);

	//if (!player_->GetAnimation()->GetPlaying()) {
	//	// アニメーションを変更
	//	player_->StartAnimation("Run", 0.1f, 0.0f);
	//	player_->SetIsLoopAnimation(true);
	//}
	//else {
	//	// アニメーションを変更
	//	player_->StartAnimation("Run", 0.1f, 0.0f, LWP::Resource::Animation::TrackType::Blend);
	//	player_->SetIsLoopAnimation(true, LWP::Resource::Animation::TrackType::Blend);
	//	player_->SetIsLoopAnimation(false, LWP::Resource::Animation::TrackType::Main);
	//}
}