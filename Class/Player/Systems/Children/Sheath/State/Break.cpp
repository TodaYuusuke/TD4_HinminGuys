#include "Break.h"
#include "../../../../Player.h"
#include "../Sheath.h"
#include "Throw.h"
#include "../../../../Command/InputHandler.h"
#include "../../../../PlayerAudioNames.h"

Break::Break(FollowCamera* followCamera, Sheath* sheathSystem, Player* player, std::map<int, EventOrder>* eventOrders) {
	followCamera_ = followCamera;
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

	// 鞘の攻撃判定多重回避リストの初期化
	sheathSystem_->ClearHitTargetNames();

	// 鞘破壊状態にする
	sheathSystem_->SetIsBreak(true);
	sheathSystem_->SetIsNone(false);
	sheathSystem_->SetIsSheathing(false);
}

void Break::Initialize() {

}

void Break::Update() {
	// 鞘破壊状態終了
	if (!player_->GetUIManager()->GetSheathGauge().GetIsIncrease() && !isActive_) {
		sheathSystem_->Reset();
		sheathSystem_->ChangeState(new Throw(followCamera_, sheathSystem_, player_, eventOrders_));
		return;
	}

	if (!isActive_) { return; }

	(*eventOrders_)[(int)Sheath::SheathState::kBreak].Update();

	// ダッシュ攻撃のアクションイベントごとの処理
	CheckBreakState();

	// ダッシュ攻撃終了
	if ((*eventOrders_)[(int)Sheath::SheathState::kBreak].GetIsEnd()) {
		Reset();
		sheathSystem_->Reset();
		// 入力のあったシステム
		sheathSystem_->SetNextSystems(sheathSystem_->CheckNextSystems());
		// 何も入力がなければ移動システムを入れる
		if (sheathSystem_->GetNextSystems().empty()) {
			sheathSystem_->SetNextSystem(SystemState::kMove);
		}
		sheathSystem_->ChangeState(new Throw(followCamera_, sheathSystem_, player_, eventOrders_));
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
		player_->StartAnimation("SheathDash", 0.15f, 0.0f);

		// 攻撃力設定
		player_->GetParameter()->attackStrength_ = sheathSystem_->jsonData_.dashAttackValue;

		(*eventOrders_)[(int)Sheath::SheathState::kBreak].Start();
		isActive_ = true;
		sheathSystem_->SetIsActive(true);
		// 無敵開始
		(*eventOrders_)[(int)Sheath::SheathState::kInvinsible].Start();
		start_ = { 0,0,0 };
		end_ = sheathSystem_->jsonData_.dashAttackMovement * Matrix4x4::CreateRotateXYZMatrix(player_->GetQuat());
		t_ = 0.0f;
	}
}

void Break::AnimCommand() {

}

void Break::Reset() {
	(*eventOrders_)[(int)Sheath::SheathState::kBreak].Reset();
	velocity_ = { 0,0,0 };
	sheathSystem_->SetVelocity(velocity_);
	start_ = { 0,0,0 };
	end_ = { 0,0,0 };
	isActive_ = false;
	// 鞘攻撃の当たり判定をなくす
	player_->GetSystemManager()->GetSheathAttackCollision().isActive = false;
	t_ = 0.0f;
}

void Break::CheckBreakState() {
	// 振りかぶり時間
	if ((*eventOrders_)[(int)Sheath::SheathState::kBreak].GetCurrentTimeEvent().name == "SwingTime") {
		// 鞘攻撃の当たり判定をなくす
		player_->GetSystemManager()->GetSheathAttackCollision().isActive = false;
	}
	// ダッシュ攻撃時間
	else if ((*eventOrders_)[(int)Sheath::SheathState::kBreak].GetCurrentTimeEvent().name == "DashAttackFinishTime") {
		// 無敵時間を設定
		if (!player_->GetSystemManager()->GetSheathAttackCollision().isActive) {
			player_->GetSystemManager()->SetInvisibleTime(sheathSystem_->jsonData_.invinsibleFinishTime);
			// 音再生
			player_->PlaySE(PlayerAudio::SE::Sheath::returnSheath.fileName, PlayerAudio::SE::Sheath::returnSheath.name, PlayerAudio::SE::Sheath::returnSheath.volume);
		}

		// 当たり判定を出す
		player_->GetSystemManager()->GetSheathAttackCollision().isActive = true;

		// 回避の速度補間がなくなるまでイージングを行う
		if (t_ < sheathSystem_->jsonData_.dashAttackFinishTime * 60.0f) {
			t_+= HitStopController::GetInstance()->GetDeltaTime();
			// イージングを行う
			velocity_ = LWP::Utility::Interpolation::Lerp(start_, end_, LWP::Utility::Easing::OutExpo(t_ / sheathSystem_->jsonData_.dashAttackFinishTime * 60.0f)) * 0.1f * HitStopController::GetInstance()->GetDeltaTime();
		}

		// 移動速度からラジアンを求める
		radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, velocity_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
	}
	// 硬直時間
	else if ((*eventOrders_)[(int)Sheath::SheathState::kBreak].GetCurrentTimeEvent().name == "RecoveryTime") {
		// 鞘攻撃の当たり判定をなくす
		player_->GetSystemManager()->GetSheathAttackCollision().isActive = false;

		// 徐々に減速
		velocity_ = LWP::Utility::Interpolation::Exponential(velocity_, Vector3{ 0,0,0 }, 0.1f) * HitStopController::GetInstance()->GetDeltaTime();
	}

	sheathSystem_->SetVelocity(velocity_);
	sheathSystem_->SetRotate(radian_);
	sheathSystem_->SetRotate(quat_);
}
