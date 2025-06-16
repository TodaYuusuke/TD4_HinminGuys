#include "SwordDrawn.h"
#include "../Sheath.h"
#include "Collect.h"
#include "../../../../Player.h"

SwordDrawn::SwordDrawn(Sheath* sheathSystem, Player* player, std::map<int, EventOrder>* eventOrders) {
	sheathSystem_ = sheathSystem;
	player_ = player;
	eventOrders_ = eventOrders;

	// 状態の名前
	stateName_ = "SwordDrawn";

	// 鞘攻撃の当たり判定をなくす
	player_->GetSystemManager()->GetSheathAttackCollision().isActive = false;

	// 鞘を無い状態にする
	sheathSystem_->SetIsNone(true);
	sheathSystem_->SetIsSheathing(false);
	// 入力のあったシステム
	sheathSystem_->SetNextSystems(sheathSystem_->CheckNextSystems());
	// 何も入力がなければ移動システムを入れる
	if (sheathSystem_->GetNextSystems().empty()) {
		sheathSystem_->SetNextSystem(SystemState::kMove);
	}
}

SwordDrawn::~SwordDrawn() {
	// リストクリア
	sheathSystem_->ClearNextSystems();
}

void SwordDrawn::Initialize()
{
}

void SwordDrawn::Update() {
	// 入力のあったシステム
	sheathSystem_->SetNextSystems(sheathSystem_->CheckNextSystems());
	// 何も入力がなければ移動システムを入れる
	if (sheathSystem_->GetNextSystems().empty()) {
		sheathSystem_->SetNextSystem(SystemState::kMove);
	}
}

void SwordDrawn::Command() {
	sheathSystem_->ChangeState(new Collect(sheathSystem_, player_, eventOrders_));
}

void SwordDrawn::AnimCommand()
{
}

void SwordDrawn::Reset()
{
}
