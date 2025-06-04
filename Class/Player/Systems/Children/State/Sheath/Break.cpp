#include "Break.h"
#include "../../../../Player.h"
#include "../../Sheath.h"
#include "Throw.h"
#include "../../../../Command/InputHandler.h"

Break::Break(Sheath* sheathSystem, Player* player, std::map<int, EventOrder>* eventOrders) {
	sheathSystem_ = sheathSystem;
	player_ = player;
	// コマンドの登録
	inputHandler_ = InputHandler::GetInstance();

	eventOrders_ = eventOrders;

	// 状態の名前
	stateName_ = "Break";

	isActive_ = true;

	// 鞘モデルを非表示
	sheathSystem_->SetIsSheathModelActive(false);
	// 本体のモデルも非表示
	player_->SetIsSheathModelActive(false);
}

void Break::Initialize() {

}

void Break::Update() {
	// 鞘破壊状態終了
	if (!player_->GetUIManager()->GetSheathGauge().GetIsIncrease() && !isActive_) {
		sheathSystem_->Reset();
		sheathSystem_->ChangeState(new Throw(sheathSystem_, player_, eventOrders_));
		return;
	}

	if (!isActive_) { return; }

	(*eventOrders_)[(int)Sheath::SheathState::kBreak].Update();

	// ダッシュ攻撃のアクションイベントごとの処理
	CheckBreakState();

	// ダッシュ攻撃終了
	if ((*eventOrders_)[(int)Sheath::SheathState::kBreak].GetIsEnd()) {
		Reset();
	}
}

void Break::Command() {
	// 現在アクションイベントが進行していないならコマンド成立
	if ((*eventOrders_)[(int)Sheath::SheathState::kBreak].GetIsEnd()) {
		// 鞘アニメーション開始
		player_->StopAnimation(LWP::Resource::Animation::TrackType::Blend);
		player_->SetAnimationPlaySpeed(1.0f);
		player_->SetBlendT(0.0f);
		player_->ResetAnimation();
		player_->StartAnimation("SheathDash", 0.0f, 0.0f);

		// 鞘クラスの速度を自機に適用
		player_->GetSystemManager()->SetInputState(InputState::kSheath);
		(*eventOrders_)[(int)Sheath::SheathState::kBreak].Start();
		isActive_ = true;
		sheathSystem_->SetIsActive(true);
		// 攻撃判定を出す
		sheathSystem_->SetIsCollision(true);
		// 無敵開始
		(*eventOrders_)[(int)Sheath::SheathState::kInvinsible].Start();
		start_ = { 0,0,0 };
		end_ = sheathSystem_->dashAttackMovement * Matrix4x4::CreateRotateXYZMatrix(player_->GetSystemManager()->GetRotate());
		t_ = 0.0f;
		// ロックオン以外何もできないようにする
		inputHandler_->GetSheathCommand()->SetBanInput(inputHandler_->GetSheathCommand()->GetBanInput() | (BanMove));
		inputHandler_->GetSheathCommand()->SetBanInput(inputHandler_->GetSheathCommand()->GetBanInput() | (BanAttack));
		inputHandler_->GetSheathCommand()->SetBanInput(inputHandler_->GetSheathCommand()->GetBanInput() | (BanParry));
		inputHandler_->GetSheathCommand()->SetBanInput(inputHandler_->GetSheathCommand()->GetBanInput() | (BanEvasion));
	}
}

void Break::AnimCommand() {

}

void Break::Reset() {
	(*eventOrders_)[(int)Sheath::SheathState::kBreak].Reset();
	velocity_ = { 0,0,0 };
	start_ = { 0,0,0 };
	end_ = { 0,0,0 };
	isActive_ = false;
	sheathSystem_->SetIsCollision(false);
	t_ = 0.0f;
}

void Break::CheckBreakState() {
	// 振りかぶり時間
	if ((*eventOrders_)[(int)Sheath::SheathState::kBreak].GetCurrentTimeEvent().name == "SwingTime") {
		sheathSystem_->SetIsCollision(false);
	}
	// ダッシュ攻撃時間
	else if ((*eventOrders_)[(int)Sheath::SheathState::kBreak].GetCurrentTimeEvent().name == "DashAttackFinishTime") {
		// 当たり判定を出す
		sheathSystem_->SetIsCollision(true);

		// 回避の速度補間がなくなるまでイージングを行う
		if (t_ < sheathSystem_->dashAttackFinishTime * 60.0f) {
			t_++;
			// イージングを行う
			velocity_ = LWP::Utility::Interpolation::Lerp(start_, end_, LWP::Utility::Easing::OutExpo(t_ / sheathSystem_->dashAttackFinishTime * 60.0f)) * 0.1f;
		}

		// 移動速度からラジアンを求める
		radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, velocity_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
	}
	// 硬直時間
	else if ((*eventOrders_)[(int)Sheath::SheathState::kBreak].GetCurrentTimeEvent().name == "RecoveryTime") {
		sheathSystem_->SetIsCollision(false);

		// 徐々に減速
		velocity_ = LWP::Utility::Interpolation::Exponential(velocity_, Vector3{ 0,0,0 }, 0.1f);
	}

	sheathSystem_->SetVelocity(velocity_);
	sheathSystem_->SetRotate(radian_);
	sheathSystem_->SetRotate(quat_);
}
