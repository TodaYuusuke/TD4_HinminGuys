#include "Collect.h"
#include "../../../../Player.h"
#include "../../Sheath.h"
#include "Throw.h"
#include "../../../../Command/InputHandler.h"

Collect::Collect(Sheath* sheathSystem, Player* player, std::map<int, EventOrder>* eventOrders) {
	sheathSystem_ = sheathSystem;
	player_ = player;
	// コマンドの登録
	inputHandler_ = InputHandler::GetInstance();

	eventOrders_ = eventOrders;

	// 状態の名前
	stateName_ = "Collect";

	Command();
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
		player_->StartAnimation("SheathDash", 0.0f, 0.0f);

		// 鞘回収機能を開始
		isActive_ = true;
		// 攻撃判定を出す
		player_->GetSystemManager()->GetSheathAttackCollision().isActive = true;
		sheathSystem_->SetIsActive(true);

		// 無敵開始
		(*eventOrders_)[(int)Sheath::SheathState::kInvinsible].Start();
		// アクションイベント開始
		(*eventOrders_)[(int)Sheath::SheathState::kCollect].Start();

		// イージングの始点終点を設定
		start_ = player_->GetWorldTF()->GetWorldPosition();
		end_ = sheathSystem_->GetSheathWorldTF().GetWorldPosition();

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

	t_ = 0.0f;
}

void Collect::CollectMove() {
	// 鞘回収するために自機が動いているときの処理
	if ((*eventOrders_)[(int)Sheath::SheathState::kCollect].GetCurrentTimeEvent().name == "CollectFinishTime") {
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
	}
}

float Collect::SmoothDampF(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime) {
	// Based on Game Programming Gems 4 Chapter 1.10
	float limitTime;
	limitTime = max(0.0001f, smoothTime);
	float omega = 2.0f / limitTime;

	float x = omega * deltaTime;
	float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
	float change = current - target;
	float originalTo = target;

	// Clamp maximum speed
	float maxChange = maxSpeed * limitTime;
	change = std::clamp<float>(change, -maxChange, maxChange);
	float tValue = current - change;

	float temp = (currentVelocity + omega * change) * deltaTime;
	currentVelocity = (currentVelocity - omega * temp) * exp;
	float output = tValue + (change + temp) * exp;

	// Prevent overshooting
	if (originalTo - current > 0.0f == output > originalTo)
	{
		output = originalTo;
		currentVelocity = (output - originalTo) / deltaTime;
	}

	return output;
}
LWP::Math::Vector3 Collect::SmoothDamp(LWP::Math::Vector3 current, LWP::Math::Vector3 target, LWP::Math::Vector3& currentVelocity, float smoothTime, float maxSpeed, float deltaTime) {
	LWP::Math::Vector3 result = {
		SmoothDampF(current.x, target.x, currentVelocity.x,smoothTime, maxSpeed, deltaTime),
		SmoothDampF(current.y, target.y, currentVelocity.y,smoothTime, maxSpeed, deltaTime),
		SmoothDampF(current.z, target.z, currentVelocity.z,smoothTime, maxSpeed, deltaTime)
	};

	return result;
}
