#include "SwordDrawn.h"
#include "../Sheath.h"
#include "Collect.h"
#include "../../../../Player.h"
#include "../../../../Math/MathFunctions.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;

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

	// オーラ生成
	sheathSystem_->GetAuraParticles()->Start(true, sheathSystem_->GetSheathWorldTF()->GetWorldPosition());
}

SwordDrawn::~SwordDrawn() {
	// リストクリア
	sheathSystem_->ClearNextSystems();
	// オーラを消す
	sheathSystem_->GetAuraParticles()->Finish();
}

void SwordDrawn::Initialize()
{
}

void SwordDrawn::Update() {
	// 鞘を自機に向ける角度更新
	Quaternion q = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, 3.14f / 2.0f);// 横向きにする
	Vector3 dir = (sheathSystem_->GetSheathWorldTF()->GetWorldPosition() - player_->GetWorldTF()->GetWorldPosition()).Normalize();
	dir.y = 0.0f;
	sheathSystem_->SetSheathRotation(MathFunc::LookRotation(dir) * q);

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

void SwordDrawn::Reset() {

}
