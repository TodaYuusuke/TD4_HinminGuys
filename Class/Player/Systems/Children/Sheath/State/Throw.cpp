#include "Throw.h"
#include "../../../../Player.h"
#include "../Sheath.h"
#include "SwordDrawn.h"
#include "../../../../Command/InputHandler.h"
#include "../../../../../Components/HitStopController.h"
#include "../../../../Math/MathFunctions.h"

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

	// 鞘破壊状態でなくす
	sheathSystem_->SetIsNone(false);
	sheathSystem_->SetIsBreak(false);
	sheathSystem_->SetIsSheathing(true);
}

void Throw::Initialize() {

}

void Throw::Update() {
	if (!isActive_) { return; }

	(*eventOrders_)[(int)Sheath::SheathState::kThrow].Update();

	CheckThrowState();

	// 全ての移動処理終了
	if ((*eventOrders_)[(int)Sheath::SheathState::kThrow].GetIsEnd()) {
		(*eventOrders_)[(int)Sheath::SheathState::kThrow].Reset();
		// 鞘判定をとらない
		player_->GetSystemManager()->GetSheathCollision().isActive = false;
		// 鞘なし状態に移行
		sheathSystem_->ChangeState(new SwordDrawn(sheathSystem_, player_, eventOrders_));
		return;
	}
}

void Throw::Command() {
	if ((*eventOrders_)[(int)Sheath::SheathState::kThrow].GetIsEnd()) {
		// 鞘アニメーション開始
		player_->StopAnimation(LWP::Resource::Animation::TrackType::Main);
		player_->StopAnimation(LWP::Resource::Animation::TrackType::Blend);
		player_->SetAnimationPlaySpeed(1.0f);
		player_->SetBlendT(0.0f);
		player_->ResetAnimation();
		player_->StartAnimation("SheathThrow", 0.15f, 0.0f);
		isActive_ = true;
		sheathSystem_->SetIsActive(true);

		// 鞘ゲージの減少量設定(鞘自体の攻撃力参照)
		player_->GetParameter()->sheathDamegeStrength_ = sheathSystem_->jsonData_.sheathAttackValue;
		// 攻撃力設定
		player_->GetParameter()->attackStrength_ = sheathSystem_->jsonData_.sheathAttackValue;

		// アクションイベント開始
		(*eventOrders_)[(int)Sheath::SheathState::kThrow].Start();

		// イージングの始点終点を設定
		start_ = player_->GetWorldTF()->GetWorldPosition();
		end_ = player_->GetWorldTF()->GetWorldPosition() + sheathSystem_->jsonData_.throwMovement * Matrix4x4::CreateRotateXYZMatrix(player_->GetRadian());

		// 自機の角度を最後に向いている方向に固定
		sheathSystem_->SetRotate(player_->GetRadian());
		sheathSystem_->SetRotate(player_->GetQuat());
	}
}

void Throw::AnimCommand() {

}

void Throw::Reset() {
	// 投げる用の鞘モデルを非表示
	sheathSystem_->SetIsSheathModelActive(false);
	// 本体のモデルも非表示
	player_->SetIsSheathModelActive(true);
	sheathSystem_->SetIsActive(false);
	// 鞘攻撃の当たり判定をなくす
	player_->GetSystemManager()->GetSheathAttackCollision().isActive = false;
	(*eventOrders_)[(int)Sheath::SheathState::kThrow].Reset();

	velocity_ = { 0,0,0 };
	start_ = { 0,0,0 };
	end_ = { 0,0,0 };

	// 経過時間
	currentFrame_ = 0.0f;

	t_ = 0.0f;
}

void Throw::CheckThrowState() {
	if ((*eventOrders_)[(int)Sheath::SheathState::kThrow].GetCurrentTimeEvent().name == "SwingTime") {
		sheathSystem_->SetIsSheathModelActive(false);
		// 本体のモデルも非表示
		player_->SetIsSheathModelActive(true);
	}
	// 鞘回収するために自機が動いているときの処理
	else if ((*eventOrders_)[(int)Sheath::SheathState::kThrow].GetCurrentTimeEvent().name == "ThrowFinishTime") {
		// 鎖生成
		if (!sheathSystem_->GetIsSheathModelActive()) {
			sheathSystem_->chain_->SetIsActive(isActive_);
			sheathSystem_->chain_->Reset();
			sheathSystem_->chain_->Initialize();
		}

		// 鞘判定をとれるようにする
		player_->GetSystemManager()->GetSheathCollision().isActive = true;
		sheathSystem_->SetIsSheathModelActive(true);
		// 本体のモデルも非表示
		player_->SetIsSheathModelActive(false);

		// 速度
		velocity_ = LWP::Utility::Interpolation::Lerp(start_, end_, LWP::Utility::Easing::OutExpo((*eventOrders_)[(int)Sheath::SheathState::kThrow].GetCurrentFrame() / (sheathSystem_->jsonData_.collectTime * 60.0f)));

		// 移動速度からラジアンを求める
		radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, velocity_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
		quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);

		// 座標更新
		sheathSystem_->SetSheathPos(velocity_ + sheathSystem_->kSheathDefaultPos);// 地面から鞘を離す

		// 鞘を自機に向ける角度更新
		Quaternion q = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, 3.14f / 2.0f);// 横向きにする
		Vector3 dir = (sheathSystem_->GetSheathWorldTF()->GetWorldPosition() - player_->GetWorldTF()->GetWorldPosition()).Normalize();
		dir.y = 0.0f;
		sheathSystem_->SetSheathRotation(MathFunc::LookRotation(dir) * q);
	}
	else {
		// 鞘判定をとらない
		player_->GetSystemManager()->GetSheathCollision().isActive = false;
	}
}