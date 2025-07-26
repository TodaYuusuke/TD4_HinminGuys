#include "Collect.h"
#include "../../../../Player.h"
#include "../Sheath.h"
#include "Throw.h"
#include "../../../../Command/InputHandler.h"

Collect::Collect(Sheath* sheathSystem, Player* player, std::map<int, EventOrder>* eventOrders) {
	sheathSystem_ = sheathSystem;
	player_ = player;
	// コマンドの登録
	inputHandler_ = InputHandler::GetInstance();

	eventOrders_ = eventOrders;
	sheathSystem_->CreateCollectEventOrder();

	// 攻撃に当たった相手の名前リストをクリア
	sheathSystem_->ClearHitTargetNames();

	// 状態の名前
	stateName_ = "Collect";

	Command();
}

Collect::~Collect() {
	sheathSystem_->GetFloatParticle()->SetIsDrop(true);
}

void Collect::Initialize() {

}

void Collect::Update() {
	if (!isActive_) { return; }

	(*eventOrders_)[(int)Sheath::SheathState::kCollect].Update();

	// 鞘回収の移動処理
	CollectMove();

	// 全ての移動処理終了
	if ((*eventOrders_)[(int)Sheath::SheathState::kCollect].GetIsEnd()) {
		// 入力のあったシステム
		sheathSystem_->SetNextSystems(sheathSystem_->CheckNextSystems());
		// 何も入力がなければ移動システムを入れる
		if (sheathSystem_->GetNextSystems().empty()) {
			sheathSystem_->SetNextSystem(SystemState::kMove);
		}

		sheathSystem_->Reset();
		(*eventOrders_)[(int)Sheath::SheathState::kCollect].Reset();

		// 鎖の表示をしない
		sheathSystem_->chain_->SetIsActive(false);
		sheathSystem_->chain_->Reset();

		// 投げる用の鞘モデルを非表示
		sheathSystem_->SetIsSheathModelActive(false);
		// 本体のモデルも非表示
		player_->SetIsSheathModelActive(true);	
		// 鞘攻撃の当たり判定をなくす
		player_->GetSystemManager()->GetSheathAttackCollision().isActive = false;

		// クールタイム開始
		player_->GetSystemManager()->SetSheathCoolTime(sheathSystem_->jsonData_.coolTime);

		// 鞘がある状態にする
		sheathSystem_->SetIsNone(false);
		sheathSystem_->SetIsBreak(false);
		sheathSystem_->SetIsSheathing(true);

		// 攻撃に当たった相手の名前リストをクリア
		sheathSystem_->ClearHitTargetNames();

		// リストクリア
		sheathSystem_->ClearNextSystems();
		// 投げ可能状態に変更
		sheathSystem_->ChangeState(new Throw(sheathSystem_, player_, eventOrders_));
		return;
	}
}

void Collect::Command() {
	if ((*eventOrders_)[(int)Sheath::SheathState::kCollect].GetIsEnd()) {
		// 鞘アニメーション開始
		player_->StopAnimation(LWP::Resource::Animation::TrackType::Main);
		player_->StopAnimation(LWP::Resource::Animation::TrackType::Blend);
		player_->SetAnimationPlaySpeed(1.0f);
		player_->SetBlendT(0.0f);
		player_->ResetAnimation();
		player_->StartAnimation("SheathDash", 0.15f, 0.0f);

		// 鞘回収機能を開始
		isActive_ = true;
		sheathSystem_->SetIsActive(true);

		// 鞘ゲージの減少量設定
		float decrementValue = sheathSystem_->jsonData_.collectAttackSheathDecrementPercent / 100.0f * player_->GetUIManager()->GetSheathGauge().GetMaxValue();
		player_->GetParameter()->sheathDamegeStrength_ = decrementValue;
		// 攻撃力設定
		player_->GetParameter()->attackStrength_ = sheathSystem_->jsonData_.collectAttackValue;

		// 無敵開始
		(*eventOrders_)[(int)Sheath::SheathState::kInvinsible].Start();
		// アクションイベント開始
		(*eventOrders_)[(int)Sheath::SheathState::kCollect].Start();

		// イージングの始点終点を設定
		start_ = player_->GetWorldTF()->GetWorldPosition();
		end_ = sheathSystem_->GetSheathWorldTF()->GetWorldPosition();
		end_.y = player_->GetWorldTF()->GetWorldPosition().y;			// 地面と平行に移動させる

		// イージングの始点終点から角度を求める
		radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, (end_ - start_).Normalize(), LWP::Math::Vector3{ 0,1,0 });
		quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);

		sheathSystem_->SetVelocity(velocity_);
		sheathSystem_->SetRotate(radian_);
		sheathSystem_->SetRotate(quat_);
	}
}

void Collect::AnimCommand() {

}

void Collect::Reset() {
	// 投げる用の鞘モデルを非表示
	sheathSystem_->SetIsSheathModelActive(true);
	// 本体のモデルも非表示
	player_->SetIsSheathModelActive(false);
	sheathSystem_->SetIsActive(false);
	// 鞘攻撃の当たり判定をなくす
	player_->GetSystemManager()->GetSheathAttackCollision().isActive = false;
	// 無敵開始
	(*eventOrders_)[(int)Sheath::SheathState::kInvinsible].Reset();
	// アクションイベント開始
	(*eventOrders_)[(int)Sheath::SheathState::kCollect].Reset();

	velocity_ = { 0,0,0 };
	start_ = { 0,0,0 };
	end_ = { 0,0,0 };
}

void Collect::CollectMove() {
	// 鞘回収するために自機が動いているときの処理
	if ((*eventOrders_)[(int)Sheath::SheathState::kCollect].GetCurrentTimeEvent().name == "CollectFinishTime") {
		// 無敵時間を設定
		if (!player_->GetSystemManager()->GetSheathAttackCollision().isActive) {
			player_->GetSystemManager()->SetInvisibleTime(sheathSystem_->jsonData_.invinsibleFinishTime);
		}
		// 攻撃判定を出す
		player_->GetSystemManager()->GetSheathAttackCollision().isActive = true;

		// 速度を算出
		velocity_ = (LWP::Utility::Interpolation::Lerp(start_, end_, LWP::Utility::Easing::OutExpo((*eventOrders_)[(int)Sheath::SheathState::kCollect].GetCurrentFrame() / (sheathSystem_->jsonData_.collectTime * 60.0f))) - player_->GetWorldTF()->GetWorldPosition());

		// 移動速度からラジアンを求める
		radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, velocity_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
		quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);

		sheathSystem_->SetVelocity(velocity_);
		sheathSystem_->SetRotate(radian_);
		sheathSystem_->SetRotate(quat_);
	}
	else {
		// 鞘攻撃の当たり判定をなくす
		player_->GetSystemManager()->GetSheathAttackCollision().isActive = false;

		velocity_ = { 0,0,0 };
		sheathSystem_->SetVelocity(velocity_);
	}
}