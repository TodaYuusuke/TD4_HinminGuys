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
	eventOrder_.Initialize();
	// 鞘投げ発生までの時間
	eventOrder_.CreateTimeEvent(TimeEvent{ 0.0f * 60.0f, "SwingTime" });
	// 鞘投げの猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ 0.5f * 60.0f, "ThrowingTime" });
	// 鞘投げの硬直時間
	eventOrder_.CreateTimeEvent(TimeEvent{ 0.0f * 60.0f, "ThrowRecoveryTime" });
	// 状態を変える
	eventOrder_.CreateTimeEvent(TimeEvent{ 0.0f * 60.0f, "ChangeState" });
}

void Throw::Update() {
	//eventOrder_.Update();

	CheckThrowState();
}

void Throw::Command() {
	//if (eventOrder_.GetIsEnd()) {
	if (!isActive_) {
		sheathSystem_->SetSheathPos(player_->GetWorldTF()->GetWorldPosition() + throwMovement_ * Matrix4x4::CreateRotateXYZMatrix(player_->GetSystemManager()->GetMoveSystem()->GetMoveRadian()));
		sheathSystem_->SetIsActive(true);
		sheathSystem_->ChangeState(new Collect(sheathSystem_, player_));
	}
	//eventOrder_.Start();
}

void Throw::CheckThrowState() {

	// 振りかぶりの時
	if (eventOrder_.GetCurrentTimeEvent().name == "SwingTime") {

	}
	else if (eventOrder_.GetCurrentTimeEvent().name == "ThrowingTime") {

	}
	// 硬直
	else if (eventOrder_.GetCurrentTimeEvent().name == "ThrowRecoveryTime") {
		
	}
	else if (eventOrder_.GetCurrentTimeEvent().name == "ChangeState") {
		
	}
}
