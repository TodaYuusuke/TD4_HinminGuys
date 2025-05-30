#include "Parry.h"
#include "../../Player.h"
#include "../Engine/object/core/collision/Collision.h"
#include "../../../GameMask.h"

using namespace LWP::Utility;
using namespace GameMask;

Parry::Parry(LWP::Object::Camera* camera, Player* player)
	: aabb_(collider_.SetBroadShape(LWP::Object::Collider::AABB()))
{
	pCamera_ = camera;
	player_ = player;

	// パリィ判定生成
	CreateCollision();
}

void Parry::Initialize() {
	// コマンドの登録
	inputHandler_ = InputHandler::GetInstance();
	isActive_ = false;
	isPreActive_ = false;

	// jsonで保存している値
	CreateJsonFIle();

	// フレーム単位で発生するアクションイベントを管理するクラス
	CreateEventOrder();
	// パリィ中の無敵
	CreateParryInvinsibleEventOrder();
	// ジャストパリィ成功時の無敵
	CreateJustParryInvinsibleEventOrder();
	// 弱パリィ成功時の無敵
	CreateGoodParryInvinsibleEventOrder();
}

void Parry::Update() {
	// ジャストパリィ成功時の無敵時間
	eventOrders_[(int)ParryInvinsibleState::kJust].Update();
	// 弱パリィ成功時の無敵時間
	eventOrders_[(int)ParryInvinsibleState::kGood].Update();

	// パリィ機能を使えないなら早期リターン
	if (!isActive_) { return; }

	// frameごとに起きるイベント
	eventOrder_.Update();
	// パリィ中の無敵時間
	eventOrders_[(int)ParryInvinsibleState::kRunning].Update();

	// パリィの状態確認
	CheckParryState();

	// 全てのイベントが終了しているなら機能停止
	if (eventOrder_.GetIsEnd()) {
		Reset();
	}

	isPreActive_ = isActive_;
}

void Parry::Reset() {
	isActive_ = false;
	isJustParry_ = false;
	isGoodParry_ = false;
	collider_.isActive = false;
	aabb_.isShowWireFrame = false;
	eventOrder_.Reset();
	eventOrders_[(int)ParryInvinsibleState::kRunning].Reset();
	// アニメーションを初期化
	player_->ResetAnimation();
}

void Parry::DebugGUI() {
	if (ImGui::TreeNode("Parry")) {
		// パリィのアクションイベントを保存
		if (ImGui::TreeNode("Json")) {
			json_.DebugGUI();

			// アクションイベントを実行してないときのみ変更可能
			if (eventOrder_.GetIsEnd()) {
				// アクションイベントを再登録
				eventOrder_.Initialize();
				CreateEventOrder();
			}
			// パリィ中の無敵イベントを実行してないときのみ変更可能
			if (eventOrders_[(int)ParryInvinsibleState::kRunning].GetIsEnd()) {
				// アクションイベントを再登録
				eventOrders_[(int)ParryInvinsibleState::kRunning].Initialize();
				CreateParryInvinsibleEventOrder();
			}
			// ジャストパリィ成功時無敵イベントを実行してないときのみ変更可能
			if (eventOrders_[(int)ParryInvinsibleState::kJust].GetIsEnd()) {
				// アクションイベントを再登録
				eventOrders_[(int)ParryInvinsibleState::kJust].Initialize();
				CreateJustParryInvinsibleEventOrder();
			}
			// 弱jパリィ成功時の無敵イベントを実行してないときのみ変更可能
			if (eventOrders_[(int)ParryInvinsibleState::kGood].GetIsEnd()) {
				// アクションイベントを再登録
				eventOrders_[(int)ParryInvinsibleState::kGood].Initialize();
				CreateGoodParryInvinsibleEventOrder();
			}

			ImGui::TreePop();
		}

		eventOrder_.DebugGUI();

		if (ImGui::TreeNode("Collider")) {
			collider_.DebugGUI();
			ImGui::TreePop();
		}

		ImGui::Checkbox("IsJustParry", &isJustParry_);
		ImGui::Checkbox("IsGoodParry", &isGoodParry_);

		ImGui::TreePop();
	}
}

void Parry::CreateJsonFIle() {
	json_.Init("ParryData.json");
	json_.BeginGroup("Parry")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &kSwingTime)
		.AddValue<float>("JustParry", &kJustParryTime)
		.AddValue<float>("GoodParry", &kGoodParryTime)
		.AddValue<float>("RecoveryTime", &kRecoveryTime)
		.EndGroup()
		// パリィの無敵時間
		.BeginGroup("Invinsible")
		// ジャストパリィ成功時
		.BeginGroup("Success JustParry")
		.AddValue<float>("InvinsibleTime", &successJustParryInvinsible)
		.EndGroup()
		// 弱jパリィ成功時
		.BeginGroup("Success GoodParry")
		.AddValue<float>("InvinsibleTime", &successGoodParryInvinsible)
		.EndGroup()

		.EndGroup()
		// 当たり判定
		.BeginGroup("Collider")
		.AddValue<Vector3>("Min", &aabb_.min)
		.AddValue<Vector3>("Max", &aabb_.max)
		.EndGroup()
		// 鞘ゲージの減少量
		.BeginGroup("SheathDecrement")
		.AddValue<float>("JustParry", &justParryDecrement)
		.AddValue<float>("GoodParry", &goodParryDecrement)
		.EndGroup()

		.EndGroup()
		.CheckJsonFile();
}

void Parry::Command() {
	if (eventOrder_.GetIsEnd()) {
		// パリィ状態に移行
		player_->GetSystemManager()->SetInputState(InputState::kParry);
		eventOrders_[(int)ParryInvinsibleState::kRunning].Start();
		isActive_ = true;
		collider_.isActive = true;
		aabb_.isShowWireFrame = true;
		radian_ = player_->GetSystemManager()->GetMoveSystem()->GetMoveRadian();
		quat_ = player_->GetSystemManager()->GetMoveSystem()->GetMoveQuat();
	}
	eventOrder_.Start();
}

void Parry::AnimCommand() {
	// ガードアニメーション開始
	player_->ResetAnimation();
	player_->StartAnimation("Gaurd", 0.0f, 0.0f);
}

void Parry::CreateCollision() {
	// 攻撃判定生成
	aabb_.min = { -1.0f, -1.0f, -1.0f };
	aabb_.max = { 1.0f, 1.0f, 1.0f };
	aabb_.isShowWireFrame = false;
	collider_.SetFollow(player_->GetWorldTF());
	collider_.isActive = false;
	collider_.worldTF.translation = { 0.0f, 1.0f, 0.0f };
	collider_.mask.SetBelongFrag(GetPlayer());
	collider_.mask.SetHitFrag(GetEnemy() | GetAttack());
	collider_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		// すでにジャスパor甘パリィなら処理しない
		if (isGoodParry_ || isJustParry_) { return; }

		LWP::Math::Vector3 p2t = (hitTarget->GetWorldPosition() - player_->GetWorldTF()->GetWorldPosition()).Normalize();

		// ジャストパリィ
		if (eventOrder_.GetCurrentTimeEvent().name == "JustParry") {
			isJustParry_ = true;
			isGoodParry_ = false;
			eventOrders_[(int)ParryInvinsibleState::kJust].Start();
			// ガードアニメーション開始
			player_->ResetAnimation();
			player_->StartAnimation("WeakParry", 0.0f, 0.0f);
			// 鞘のゲージを減少
			player_->GetUIManager()->ChangeSheathGauge(justParryDecrement);
			// 相手の座標を代入
			parryTargetPos_ = hitTarget->GetWorldPosition();

			radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, p2t, LWP::Math::Vector3{ 0,1,0 });
			quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
		}
		// 甘めパリィ
		else if (eventOrder_.GetCurrentTimeEvent().name == "GoodParry") {
			isGoodParry_ = true;
			isJustParry_ = false;
			eventOrders_[(int)ParryInvinsibleState::kGood].Start();
			// ガードアニメーション開始
			player_->ResetAnimation();
			player_->StartAnimation("WeakParry", 0.0f, 0.0f);
			// 鞘のゲージを減少
			player_->GetUIManager()->ChangeSheathGauge(goodParryDecrement);
			// 相手の座標を代入
			parryTargetPos_ = hitTarget->GetWorldPosition();

			radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, p2t, LWP::Math::Vector3{ 0,1,0 });
			quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
		}
		};
}

void Parry::CreateEventOrder() {
	eventOrder_.Initialize();
	// パリィ発生までの時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kSwingTime * 60.0f, "SwingTime" });
	// ジャストパリィの猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kJustParryTime * 60.0f, "JustParry" });
	// 通常パリィの猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kGoodParryTime * 60.0f, "GoodParry" });
	// パリィの硬直時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kRecoveryTime * 60.0f, "RecoveryTime" });
}

void Parry::CreateParryInvinsibleEventOrder() {
	eventOrders_[(int)ParryInvinsibleState::kRunning].Initialize();
	// パリィ中の無敵発動までの時間
	eventOrders_[(int)ParryInvinsibleState::kRunning].CreateTimeEvent(TimeEvent{ kSwingTime * 60.0f, "SwingTime" });
	// パリィ中の無敵時間
	eventOrders_[(int)ParryInvinsibleState::kRunning].CreateTimeEvent(TimeEvent{ (kJustParryTime + kGoodParryTime) * 60.0f, "InvinsibleTime" });
	// パリィ中の無敵硬直時間
	eventOrders_[(int)ParryInvinsibleState::kRunning].CreateTimeEvent(TimeEvent{ kRecoveryTime * 60.0f, "RecoveryTime" });
}

void Parry::CreateJustParryInvinsibleEventOrder() {
	eventOrders_[(int)ParryInvinsibleState::kJust].Initialize();
	// ジャストパリィ成功時の無敵時間
	eventOrders_[(int)ParryInvinsibleState::kJust].CreateTimeEvent(TimeEvent{ successJustParryInvinsible * 60.0f, "InvinsibleTime" });
}

void Parry::CreateGoodParryInvinsibleEventOrder() {
	eventOrders_[(int)ParryInvinsibleState::kGood].Initialize();
	// 弱パリィ成功時の無敵時間
	eventOrders_[(int)ParryInvinsibleState::kGood].CreateTimeEvent(TimeEvent{ successGoodParryInvinsible * 60.0f, "InvinsibleTime" });
}

void Parry::CheckParryState() {
	// 予備動作
	if (eventOrder_.GetCurrentTimeEvent().name == "SwingTime") {
		collider_.isActive = false;
		isJustParry_ = false;
		isGoodParry_ = false;
	}
	// ジャストパリィ
	else if (eventOrder_.GetCurrentTimeEvent().name == "JustParry") {
		collider_.isActive = true;
	}
	// 甘めパリィ
	else if (eventOrder_.GetCurrentTimeEvent().name == "GoodParry") {
		collider_.isActive = true;
	}
	// 硬直
	else if (eventOrder_.GetCurrentTimeEvent().name == "RecoveryTime") {
		collider_.isActive = false;
		isJustParry_ = false;
		isGoodParry_ = false;
	}
}