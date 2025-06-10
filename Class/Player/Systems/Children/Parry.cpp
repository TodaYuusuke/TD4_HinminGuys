#include "Parry.h"
#include "../../Player.h"
#include "../Engine/object/core/collision/Collision.h"
#include "../../../GameMask.h"

using namespace GameMask;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

Parry::Parry(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;

	// パリィ判定生成
	CreateCollision();
}

Parry::~Parry() {
	
}

void Parry::Initialize() {
	// コマンドの登録
	inputHandler_ = InputHandler::GetInstance();

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
	// ノックバック
	KnockBackUpdate();

	// ジャストパリィ成功時の無敵時間
	eventOrders_[(int)ParryInvinsibleState::kJust].Update();
	// 弱パリィ成功時の無敵時間
	eventOrders_[(int)ParryInvinsibleState::kGood].Update();

	// ジャストパリィ成功時の無敵時間が終了
	if (eventOrders_[(int)ParryInvinsibleState::kJust].GetIsEnd()) {
		isJustParry_ = false;
		velocity_ = { 0,0,0 };
		t_ = 0.0f;
	}
	if (eventOrders_[(int)ParryInvinsibleState::kGood].GetIsEnd()) {
		isGoodParry_ = false;
	}

	// frameごとに起きるイベント
	eventOrder_.Update();
	// パリィ中の無敵時間
	eventOrders_[(int)ParryInvinsibleState::kRunning].Update();

	// パリィの状態確認
	CheckParryState();

	// 全てのイベントが終了しているなら機能停止
	if (eventOrder_.GetIsEnd()) {
		// 入力のあったシステム
		nextSystem_ = CheckNextSystems();
		// 何も入力がなければ移動システムを入れる
		if (nextSystem_.empty()) {
			nextSystem_[SystemState::kMove] = true;
		}
		Reset();
	}

	isPreActive_ = isActive_;
}

void Parry::Reset() {
	player_->GetSystemManager()->GetParryCollision().isActive = false;

	// クールタイム設定
	if (!isJustParry_ || !isGoodParry_) {
		player_->GetSystemManager()->SetParryCoolTime(jsonData_.coolTime);
	}
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

		//if (ImGui::TreeNode("Collider")) {
		//	collider_.DebugGUI();
		//	ImGui::TreePop();
		//}

		ImGui::Checkbox("IsJustParry", &isJustParry_);
		ImGui::Checkbox("IsGoodParry", &isGoodParry_);

		ImGui::TreePop();
	}
}

void Parry::CreateJsonFIle() {
	json_.Init("ParryData.json");
	json_.BeginGroup("Parry")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &jsonData_.kSwingTime)
		.AddValue<float>("JustParry", &jsonData_.kJustParryTime)
		.AddValue<float>("GoodParry", &jsonData_.kGoodParryTime)
		.AddValue<float>("RecoveryTime", &jsonData_.kRecoveryTime)
		.EndGroup()
		// パリィの無敵時間
		.BeginGroup("Invinsible")
		// ジャストパリィ成功時
		.BeginGroup("Success JustParry")
		.AddValue<float>("InvinsibleTime", &jsonData_.successJustParryInvinsible)
		.EndGroup()
		// 弱jパリィ成功時
		.BeginGroup("Success GoodParry")
		.AddValue<float>("InvinsibleTime", &jsonData_.successGoodParryInvinsible)
		.EndGroup()

		.EndGroup()
		//// 当たり判定
		//.BeginGroup("Collider")
		//.AddValue<Vector3>("Min", &aabb_.min)
		//.AddValue<Vector3>("Max", &aabb_.max)
		//.EndGroup()
		// 鞘ゲージの減少量
		.BeginGroup("SheathDecrement")
		.AddValue<float>("JustParry", &jsonData_.justParryDecrement)
		.AddValue<float>("GoodParry", &jsonData_.goodParryDecrement)
		.EndGroup()

		.EndGroup()
		.CheckJsonFile();
}

void Parry::Command() {
	if (eventOrder_.GetIsEnd()) {
		eventOrder_.Start();
		eventOrders_[(int)ParryInvinsibleState::kRunning].Start();
		isActive_ = true;
		radian_ = player_->GetRadian();
		quat_ = player_->GetQuat();

		// アニメーション再生
		AnimCommand();
	}

}

void Parry::AnimCommand() {
	// ガードアニメーション開始
	player_->StopAnimation(LWP::Resource::Animation::TrackType::Main);
	player_->StopAnimation(LWP::Resource::Animation::TrackType::Blend);
	player_->SetAnimationPlaySpeed(1.0f);
	player_->SetBlendT(0.0f);
	player_->ResetAnimation();
	player_->StartAnimation("Gaurd", 0.0f, 0.0f);
}

void Parry::CreateCollision() {
	// 当たった時の処理
	player_->GetSystemManager()->SetParryOnHitFunc(
		[this](LWP::Object::Collision* hitTarget) {
			// すでにジャスパor甘パリィなら処理しない
			if (isGoodParry_ || isJustParry_) { return; }

			LWP::Math::Vector3 p2t = (hitTarget->GetWorldPosition() - player_->GetWorldTF()->GetWorldPosition()).Normalize();

			// ジャストパリィ
			if (eventOrder_.GetCurrentTimeEvent().name == "JustParry") {
				isJustParry_ = true;
				isGoodParry_ = false;
				eventOrders_[(int)ParryInvinsibleState::kJust].Start();

				// 無敵時間を設定
				player_->GetSystemManager()->SetInvisibleTime(jsonData_.successJustParryInvinsible * 60.0f);

				// 鞘のゲージを減少
				player_->GetUIManager()->ChangeSheathGauge(jsonData_.justParryDecrement);

				// 相手の座標を代入
				parryTargetPos_ = hitTarget->GetWorldPosition();

				// ノックバック量を決定
				justParryKnockBack_ = (parryTargetPos_ - player_->GetWorldTF()->GetWorldPosition()).Normalize() * -jsonData_.justParryKnockBackMovement;
				start_ = player_->GetWorldTF()->GetWorldPosition();
				end_ = player_->GetWorldTF()->GetWorldPosition() + justParryKnockBack_;

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

				// 無敵時間を設定
				player_->GetSystemManager()->SetInvisibleTime(jsonData_.successGoodParryInvinsible * 60.0f);

				// 鞘のゲージを減少
				player_->GetUIManager()->ChangeSheathGauge(jsonData_.goodParryDecrement);

				// 相手の座標を代入
				parryTargetPos_ = hitTarget->GetWorldPosition();

				radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, p2t, LWP::Math::Vector3{ 0,1,0 });
				quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
			}
		}
	);
}

void Parry::CreateEventOrder() {
	eventOrder_.Initialize();
	// パリィ発生までの時間
	eventOrder_.CreateTimeEvent(TimeEvent{ jsonData_.kSwingTime * 60.0f, "SwingTime" });
	// ジャストパリィの猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ jsonData_.kJustParryTime * 60.0f, "JustParry" });
	// 通常パリィの猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ jsonData_.kGoodParryTime * 60.0f, "GoodParry" });
	// パリィの硬直時間
	eventOrder_.CreateTimeEvent(TimeEvent{ jsonData_.kRecoveryTime * 60.0f, "RecoveryTime" });
}

void Parry::CreateParryInvinsibleEventOrder() {
	eventOrders_[(int)ParryInvinsibleState::kRunning].Initialize();
	// パリィ中の無敵発動までの時間
	eventOrders_[(int)ParryInvinsibleState::kRunning].CreateTimeEvent(TimeEvent{ jsonData_.kSwingTime * 60.0f, "SwingTime" });
	// パリィ中の無敵時間
	eventOrders_[(int)ParryInvinsibleState::kRunning].CreateTimeEvent(TimeEvent{ (jsonData_.kJustParryTime + jsonData_.kGoodParryTime) * 60.0f, "InvinsibleTime" });
	// パリィ中の無敵硬直時間
	eventOrders_[(int)ParryInvinsibleState::kRunning].CreateTimeEvent(TimeEvent{ jsonData_.kRecoveryTime * 60.0f, "RecoveryTime" });
}

void Parry::CreateJustParryInvinsibleEventOrder() {
	eventOrders_[(int)ParryInvinsibleState::kJust].Initialize();
	// ジャストパリィ成功時の無敵時間
	eventOrders_[(int)ParryInvinsibleState::kJust].CreateTimeEvent(TimeEvent{ jsonData_.successJustParryInvinsible * 60.0f, "InvinsibleTime" });
}

void Parry::CreateGoodParryInvinsibleEventOrder() {
	eventOrders_[(int)ParryInvinsibleState::kGood].Initialize();
	// 弱パリィ成功時の無敵時間
	eventOrders_[(int)ParryInvinsibleState::kGood].CreateTimeEvent(TimeEvent{ jsonData_.successGoodParryInvinsible * 60.0f, "InvinsibleTime" });
}

void Parry::CheckParryState() {
	// 予備動作
	if (eventOrder_.GetCurrentTimeEvent().name == "SwingTime") {
		player_->GetSystemManager()->GetParryCollision().isActive = false;
		isJustParry_ = false;
		isGoodParry_ = false;
	}
	// ジャストパリィ
	else if (eventOrder_.GetCurrentTimeEvent().name == "JustParry") {
		// 自機本体の無敵開始
		if (!player_->GetSystemManager()->GetParryCollision().isActive) {
			// 無敵時間を設定
			player_->GetSystemManager()->SetInvisibleTime((jsonData_.kJustParryTime + jsonData_.kGoodParryTime) * 60.0f);
		}
		player_->GetSystemManager()->GetParryCollision().isActive = true;
	}
	// 甘めパリィ
	else if (eventOrder_.GetCurrentTimeEvent().name == "GoodParry") {
		player_->GetSystemManager()->GetParryCollision().isActive = true;
	}
	// 硬直
	else if (eventOrder_.GetCurrentTimeEvent().name == "RecoveryTime") {
		player_->GetSystemManager()->GetParryCollision().isActive = false;
		isJustParry_ = false;
		isGoodParry_ = false;
	}
}

void Parry::KnockBackUpdate() {
	// ジャストパリィ時のみ
	if (!isJustParry_) { return; }
	t_++;

	velocity_ = Lerp(start_, end_, Easing::OutExpo(t_ / jsonData_.justParryKnockBackFinishTime)) - player_->GetWorldTF()->GetWorldPosition();
	velocity_.y = 0.0f;

	if (t_ == 1.0f) {
		// ガードアニメーション開始
		player_->ResetAnimation();
		player_->StartAnimation("StrongParry", 0.0f, 0.0f);
	}
}