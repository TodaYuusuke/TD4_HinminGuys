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
	CheckThrowState();
}

void Throw::Command() {
	if (!isActive_) {
		// 鞘クラスの速度を自機に適用
		player_->GetSystemManager()->SetInputState(InputState::kSheath);

		sheathSystem_->SetSheathPos(player_->GetWorldTF()->GetWorldPosition() + sheathSystem_->throwMovement * Matrix4x4::CreateRotateXYZMatrix(player_->GetSystemManager()->GetMoveSystem()->GetMoveRadian()));
		sheathSystem_->SetIsActive(true);
		inputHandler_->GetSheathCommand()->SetBanInput(inputHandler_->GetSheathCommand()->GetBanInput() & ~(1 << GetSetBitPosition(BanMove)));
		inputHandler_->GetSheathCommand()->SetBanInput(inputHandler_->GetSheathCommand()->GetBanInput() & ~(1 << GetSetBitPosition(BanAttack)));
		inputHandler_->GetSheathCommand()->SetBanInput(inputHandler_->GetSheathCommand()->GetBanInput() & ~(1 << GetSetBitPosition(BanEvasion)));
		inputHandler_->GetSheathCommand()->SetBanInput(inputHandler_->GetSheathCommand()->GetBanInput() & ~(1 << GetSetBitPosition(BanSheath)));
		inputHandler_->GetSheathCommand()->SetBanInput(inputHandler_->GetSheathCommand()->GetBanInput() | (BanParry));
		sheathSystem_->ChangeState(new Collect(sheathSystem_, player_, eventOrders_));
	}
}

void Throw::AnimCommand() {

}

void Throw::CheckThrowState() {

}