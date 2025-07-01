#include "Evasion.h"
#include "../../../Player.h"
#include "../../../Command/InputConfig.h"
#include <algorithm>       // ← std::max に必要

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;
using namespace InputConfig;

Evasion::Evasion(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;
	inputHandler_ = InputHandler::GetInstance();
	// ヒットストップの管理クラス
	hitStopController_ = HitStopController::GetInstance();
}

void Evasion::Initialize() {
	isActive_ = false;
	isPreActive_ = false;

	animationPlaySpeed_.Add(&animPlaySpeed_, Vector3{ 0.05f, 0.0f, 0.0f }, 0.0f, 0.1f, LWP::Utility::Easing::Type::OutExpo)
		.Add(&animPlaySpeed_, Vector3{ 1.0f, 0.0f, 0.0f }, 0.1f, 0.5f, LWP::Utility::Easing::Type::InExpo);

	// アクションイベントを生成
	CreateEventOrder();
}

void Evasion::Update() {
	// ダッシュ条件を満たしているのかを確認
	CheckDash();

	// frameごとに起きるアクションイベント
	eventOrder_.Update();
	eventOrders_[(int)EventOrderState::kInvincible].Update();
	eventOrders_[(int)EventOrderState::kAcceleration].Update();

	// アクションイベントの確認
	CheckEvasionState();

	animationPlaySpeed_.Update();

	// 全てのアクションイベントが終了しているなら機能停止
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

void Evasion::Reset() {
	// クールタイムを設定
	player_->GetSystemManager()->SetEvasionCoolTime(jsonData_.coolTime);
}

void Evasion::DebugGUI() {
	if (ImGui::TreeNode("Evasion")) {
		// 回避のアクションイベントを保存
		if (ImGui::TreeNode("Json")) {
			// アクションイベントを実行してないときのみ変更可能
			if (eventOrder_.GetIsEnd()) {
				json_.DebugGUI();
				// アクションイベントを再登録
				eventOrder_.Initialize();
				CreateEventOrder();
			}
			else {
				ImGui::Text("Event Running!");
			}
			ImGui::TreePop();
		}

		eventOrder_.DebugGUI();

		ImGui::DragFloat3("Velocity", &velocity_.x);
		ImGui::DragFloat("pressTime", &pressTime_);

		ImGui::Checkbox("IsEvasion", &isActive_);

		ImGui::TreePop();
	}
}

void Evasion::CreateJsonFIle() {
	json_.Init("EvasionData.json");
	json_.BeginGroup("EventOrder")
		// 回避の無敵タイミングの設定
		.BeginGroup("Invinsible")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &jsonData_.invinsibleSwingTime)
		.AddValue<float>("InvinsibleTime", &jsonData_.invinsibleTime)
		.AddValue<float>("RecoveryTime", &jsonData_.invinsibleRecoveryTime)
		.EndGroup()
		.EndGroup()
		// 加速タイミングの設定
		.BeginGroup("Acceleration")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &jsonData_.accelerationSwingTime)
		.AddValue<float>("AccelerationTime", &jsonData_.accelerationTime)
		.AddValue<float>("RecoveryTime", &jsonData_.accelerationRecoveryTime)
		.EndGroup()
		.EndGroup()
		.EndGroup()
		// ダッシュに関する設定
		.BeginGroup("Dash")
		.AddValue<float>("ButtonHoldSeconds", &jsonData_.dashButtonHoldSeconds)
		.EndGroup()
		// 回避の終了時間
		.AddValue<float>("FinishTime", &jsonData_.evasionFinishTime)
		// クールタイム
		.AddValue<float>("CoolTime", &jsonData_.coolTime)
		// 回避速度の倍率
		.AddValue<float>("MoveMultiply", &jsonData_.moveMultiply)
		// 回避の移動距離
		.AddValue<Vector3>("Movement", &jsonData_.evasionMovement)
		.CheckJsonFile();
}

void Evasion::Command() {
	if (eventOrder_.GetIsEnd()) {
		eventOrders_[(int)EventOrderState::kInvincible].Start();
		eventOrders_[(int)EventOrderState::kAcceleration].Start();
		pressTime_ = 0.0f;
		isActive_ = true;

		// アニメーション再生
		AnimCommand();

		// パーティクル生成
		player_->CreateEvasionParticle(player_->GetWorldTF()->GetWorldPosition());
	}
	eventOrder_.Start();
}

void Evasion::AnimCommand() {
	animationPlaySpeed_.Start();
	player_->StopAnimation(LWP::Resource::Animation::TrackType::Main);
	player_->StopAnimation(LWP::Resource::Animation::TrackType::Blend);
	player_->SetAnimationPlaySpeed(1.0f);
	player_->SetBlendT(0.0f);
	player_->ResetAnimation();
	player_->StartAnimation("Dash", 0.15f, 0.0f);
	player_->SetIsLoopAnimation(false);
}

void Evasion::CreateInvincibleEventOrder() {
	eventOrders_[(int)EventOrderState::kInvincible].Initialize();
	// 回避の無敵発生までの時間
	eventOrders_[(int)EventOrderState::kInvincible].CreateTimeEvent(TimeEvent{ jsonData_.invinsibleSwingTime * 60.0f, "SwingTime" });
	// 回避の無敵猶予時間
	eventOrders_[(int)EventOrderState::kInvincible].CreateTimeEvent(TimeEvent{ jsonData_.invinsibleTime * 60.0f, "InvinsibleTime" });
	// 回避の無敵硬直時間
	eventOrders_[(int)EventOrderState::kInvincible].CreateTimeEvent(TimeEvent{ jsonData_.invinsibleRecoveryTime * 60.0f, "RecoveryTime" });
}

void Evasion::CreateAccelerationEventOrder() {
	eventOrders_[(int)EventOrderState::kAcceleration].Initialize();
	// 回避の加速発生までの時間
	eventOrders_[(int)EventOrderState::kAcceleration].CreateTimeEvent(TimeEvent{ jsonData_.accelerationSwingTime * 60.0f, "SwingTime" });
	// 回避の加速時間
	eventOrders_[(int)EventOrderState::kAcceleration].CreateTimeEvent(TimeEvent{ jsonData_.accelerationTime * 60.0f, "AccelerationTime" });
	// 回避の加速硬直時間
	eventOrders_[(int)EventOrderState::kAcceleration].CreateTimeEvent(TimeEvent{ jsonData_.accelerationRecoveryTime * 60.0f, "RecoveryTime" });
}

void Evasion::CreateEventOrder() {
	// 回避終了時間
	if (eventOrder_.GetIsEnd()) {
		eventOrder_.Initialize();
		// 回避の無敵発生までの時間
		eventOrder_.CreateTimeEvent(TimeEvent{ jsonData_.evasionFinishTime * 60.0f, "FinishTime" });
	}
	// 無敵タイミング
	CreateInvincibleEventOrder();
	// 加速タイミング
	CreateAccelerationEventOrder();
}

void Evasion::CheckEvasionState() {
	// 予備動作
	if (eventOrders_[(int)EventOrderState::kAcceleration].GetCurrentTimeEvent().name == "SwingTime") {
		// 方向を取得
		LWP::Math::Vector3 dir{ 0.0f, 0.0f, 0.0f };
#pragma region ゲームパッド
		// y軸方向の移動をしないようにする
		LWP::Math::Vector3 stickMovement = { LWP::Input::Controller::GetLStick().x, 0, LWP::Input::Controller::GetLStick().y };
		dir = stickMovement;
#pragma endregion

#pragma region キーボード
		if (lwp::Keyboard::GetPress(DIK_W)) {
			dir.z = 1.0f;
		}
		if (lwp::Keyboard::GetPress(DIK_S)) {
			dir.z = -1.0f;
		}
		if (lwp::Keyboard::GetPress(DIK_A)) {
			dir.x = -1.0f;
		}
		if (lwp::Keyboard::GetPress(DIK_D)) {
			dir.x = 1.0f;
		}
#pragma endregion

		easeData_ = {
			&velocity_,
			Vector3{0,0,0},
			jsonData_.evasionMovement * Matrix4x4::CreateRotateXYZMatrix(player_->GetQuat()),
			jsonData_.accelerationTime * 60.0f,
			0.0f,
			false
		};

		// 入力が何もなかったら後ろに回避
		if (dir.Length() == 0.0f) {
			isBackStep_ = true;
		}
		else {
			isBackStep_ = false;
		}

		if (isBackStep_) {
			easeData_.end = -1.0f * jsonData_.evasionMovement * Matrix4x4::CreateRotateXYZMatrix(player_->GetSystemManager()->GetQuat());
		}
	}
	// 無敵時間
	else if (eventOrders_[(int)EventOrderState::kAcceleration].GetCurrentTimeEvent().name == "AccelerationTime") {
		// 予備動作が無いとき用
		if (GetTrigger()) {
			// 回避の挙動
			easeData_ = {
				&velocity_,
				Vector3{0,0,0},
				jsonData_.evasionMovement * Matrix4x4::CreateRotateXYZMatrix(player_->GetSystemManager()->GetQuat()),
				jsonData_.accelerationTime * 60.0f,
				0.0f,
				false
			};
		}
		Move();
	}
	// 硬直時間
	else if (eventOrders_[(int)EventOrderState::kAcceleration].GetCurrentTimeEvent().name == "RecoveryTime") {

	}

	// 無敵時間
	if (eventOrders_[(int)EventOrderState::kInvincible].GetCurrentTimeEvent().name == "InvinsibleTime") {
		if (GetTrigger()) {
			// 自機本体の無敵開始
			player_->GetSystemManager()->SetInvisibleTime(jsonData_.invinsibleTime);
		}
	}
}

void Evasion::CheckDash() {
	// 長押ししている間
	if (LWP::Input::Pad::GetPress(Command::GamePad::Evasion) || LWP::Input::Keyboard::GetPress(Command::Key::Evasion)) {
		pressTime_++;
		// ダッシュ可能かを設定
		player_->GetSystemManager()->SetIsEnableDash(GetIsDash());
		return;
	}
}

void Evasion::Move() {
	// 回避の速度補間がなくなるまでイージングを行う
	if (easeData_.t < easeData_.endSecond) {
		easeData_.t += hitStopController_->GetDeltaTime();
		// イージングを行う
		velocity_ = LWP::Utility::Interpolation::Lerp(easeData_.start, easeData_.end, LWP::Utility::Easing::InExpo(easeData_.t / easeData_.endSecond)) * jsonData_.moveMultiply * hitStopController_->GetDeltaTime();
	}
	// 徐々に減速
	else {
		velocity_ = LWP::Utility::Interpolation::Exponential(velocity_, Vector3{ 0,0,0 }, 0.1f) * hitStopController_->GetDeltaTime();
	}

	// 移動ベクトルから体の向きを算出
	// 移動速度からラジアンを求める
	if (!isBackStep_) {
		radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, velocity_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
	}
	else {
		radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, -1.0f*velocity_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
	}
}