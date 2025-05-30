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

Run::~Run() {
	player_->StopAnimation(LWP::Resource::Animation::TrackType::Blend);
}

void Run::Initialize() {

}

void Run::Update() {
	player_->SetBlendT(moveSystem_->GetStickStrength());
}

void Run::AnimCommand() {
	if (!player_->GetAnimation()->GetPlaying("Walk", LWP::Resource::Animation::TrackType::Main)) {
		player_->StartAnimation("Walk", 0.1f, 0.0f, LWP::Resource::Animation::TrackType::Main);
		player_->SetIsLoopAnimation(true, LWP::Resource::Animation::TrackType::Main);
		player_->GetAnimation()->Sync();
	}
	if (!player_->GetAnimation()->GetPlaying("Run", LWP::Resource::Animation::TrackType::Blend)) {
		player_->StartAnimation("Run", 0.1f, 0.0f, LWP::Resource::Animation::TrackType::Blend);
		player_->SetIsLoopAnimation(true, LWP::Resource::Animation::TrackType::Blend);
	}
	player_->SetBlendT(moveSystem_->GetStickStrength());
}