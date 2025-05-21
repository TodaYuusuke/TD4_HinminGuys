#include "Throw.h"
#include "../../../../Player.h"
#include "../../Sheath.h"
#include "Collect.h"
#include "../../../../Command/InputHandler.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Resource;

Throw::Throw(Sheath* sheathSystem, Player* player, std::map<int, EventOrder>* eventOrders) {
	sheathSystem_ = sheathSystem;
	player_ = player;
	// コマンドの登録
	inputHandler_ = InputHandler::GetInstance();

	eventOrders_ = eventOrders;

	// 状態の名前
	stateName_ = "Throw";
}

void Throw::Initialize() {

}

void Throw::Update() {
	if (!isActive_) { return; }

	(*eventOrders_)[(int)Sheath::SheathState::kThrow].Update();

	CheckThrowState();

	// 全ての移動処理終了
	if ((*eventOrders_)[(int)Sheath::SheathState::kThrow].GetIsEnd()) {
		// パリィの入力禁止
		inputHandler_->GetSheathCommand()->SetBanInput(inputHandler_->GetSheathCommand()->GetBanInput() & ~(1 << GetSetBitPosition(BanMove)));
		inputHandler_->GetSheathCommand()->SetBanInput(inputHandler_->GetSheathCommand()->GetBanInput() & ~(1 << GetSetBitPosition(BanAttack)));
		inputHandler_->GetSheathCommand()->SetBanInput(inputHandler_->GetSheathCommand()->GetBanInput() & ~(1 << GetSetBitPosition(BanEvasion)));
		inputHandler_->GetSheathCommand()->SetBanInput(inputHandler_->GetSheathCommand()->GetBanInput() & ~(1 << GetSetBitPosition(BanSheath)));
		inputHandler_->GetSheathCommand()->SetBanInput(inputHandler_->GetSheathCommand()->GetBanInput() | (BanParry));

		(*eventOrders_)[(int)Sheath::SheathState::kThrow].Reset();

		sheathSystem_->ChangeState(new Collect(sheathSystem_, player_, eventOrders_));
		return;
	}
}

void Throw::Command() {
	//if (!isActive_) {
	if ((*eventOrders_)[(int)Sheath::SheathState::kThrow].GetIsEnd()) {
		// 鞘クラスの速度を自機に適用
		player_->GetSystemManager()->SetInputState(InputState::kSheath);

		isActive_ = true;
		sheathSystem_->SetIsActive(true);
		// アクションイベント開始
		(*eventOrders_)[(int)Sheath::SheathState::kThrow].Start();
		
		// イージングの始点終点を設定
		start_ = player_->GetWorldTF()->GetWorldPosition();
		end_ = player_->GetWorldTF()->GetWorldPosition() + sheathSystem_->throwMovement * Matrix4x4::CreateRotateXYZMatrix(player_->GetSystemManager()->GetMoveSystem()->GetMoveRadian());
	}
}

void Throw::AnimCommand() {

}

void Throw::CheckThrowState() {
	// 鞘回収するために自機が動いているときの処理
	if ((*eventOrders_)[(int)Sheath::SheathState::kThrow].GetCurrentTimeEvent().name == "ThrowFinishTime") {
		velocity_ = (LWP::Utility::Interpolation::Lerp(start_, end_, LWP::Utility::Easing::OutExpo((*eventOrders_)[(int)Sheath::SheathState::kThrow].GetCurrentFrame() / (sheathSystem_->collectTime * 60.0f)))/* - player_->GetWorldTF()->GetWorldPosition()*/);

		// 移動速度からラジアンを求める
		radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, velocity_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
		quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
	}

	sheathSystem_->SetSheathPos(velocity_);
}