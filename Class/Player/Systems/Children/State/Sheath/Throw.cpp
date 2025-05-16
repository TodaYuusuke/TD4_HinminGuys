#include "Throw.h"
#include "../../../../Player.h"
#include "../../Sheath.h"
#include "Collect.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Resource;

Throw::Throw(Sheath* sheathSystem, Player* player) {
	sheathSystem_ = sheathSystem;
	player_ = player;

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
		sheathSystem_->SetSheathPos(player_->GetWorldTF()->GetWorldPosition() + throwMovement_ * Matrix4x4::CreateRotateXYZMatrix(player_->GetSystemManager()->GetMoveSystem()->GetMoveRadian()));
		sheathSystem_->SetIsActive(true);
		sheathSystem_->ChangeState(new Collect(sheathSystem_, player_));
		isActive_ = true;
	}
}

void Throw::AnimCommand() {

}

void Throw::CheckThrowState() {

}