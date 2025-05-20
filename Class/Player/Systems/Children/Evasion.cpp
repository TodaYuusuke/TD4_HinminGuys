#include "Evasion.h"

#include <algorithm>       // ← std::max に必要
#include "../../Player.h"
#include "../../Command/InputConfig.h"

using namespace LWP;
using namespace LWP::Math;
using namespace InputConfig;

Evasion::Evasion(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;
}

void Evasion::Initialize() {
	isActive_ = false;
	isPreActive_ = false;

	json_.Init("EvasionData.json");
	json_.BeginGroup("EventOrder")
		// 回避の無敵タイミングの設定
		.BeginGroup("Invinsible")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &invinsibleSwingTime)
		.AddValue<float>("InvinsibleTime", &invinsibleTime)
		.AddValue<float>("RecoveryTime", &invinsibleRecoveryTime)
		.EndGroup()
		.EndGroup()
		// 加速タイミングの設定
		.BeginGroup("Acceleration")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &accelerationSwingTime)
		.AddValue<float>("AccelerationTime", &accelerationTime)
		.AddValue<float>("RecoveryTime", &accelerationRecoveryTime)
		.EndGroup()
		.EndGroup()
		.EndGroup()
		// ダッシュに関する設定
		.BeginGroup("Dash")
		.AddValue<float>("ButtonHoldSeconds", &dashButtonHoldSeconds)
		.EndGroup()
		// 回避の終了時間
		.AddValue<float>("FinishTime", &evasionFinishTime)
		// 回避速度の倍率
		.AddValue<float>("MoveMultiply", &moveMultiply)
		// 回避の移動距離
		.AddValue<Vector3>("Movement", &evasionMovement)
		.CheckJsonFile();

	animationPlaySpeed_.Add(&animPlaySpeed_, Vector3{ 0.05f, 0.0f, 0.0f }, 0.0f, 0.1f, LWP::Utility::Easing::Type::OutExpo)
		.Add(&animPlaySpeed_, Vector3{ 1.0f, 0.0f, 0.0f }, 0.1f, 0.5f, LWP::Utility::Easing::Type::InExpo);

	// アクションイベントを生成
	CreateEventOrder();
}

void Evasion::Update() {
	if (!isActive_) { return; }

	// ダッシュ条件を満たしているのかを確認
	CheckDash();

	// frameごとに起きるアクションイベント
	eventOrder_.Update();

	// 移動処理
	Move();

	// アクションイベントの確認
	CheckEvasionState();

	animationPlaySpeed_.Update();
	player_->SetAnimationPlaySpeed(animPlaySpeed_.x);

	// 全てのアクションイベントが終了しているなら機能停止
	if (eventOrder_.GetIsEnd()) {
		Reset();
	}

	isPreActive_ = isActive_;
}

void Evasion::Reset() {
	eventOrder_.Reset();
	isActive_ = false;
	isPreActive_ = false;
	// 回避時の速度
	velocity_ = { 0.0f, 0.0f, 0.0f };
	// 回避時の角度(ラジアン)
	radian_ = { 0.0f, 0.0f, 0.0f };
	// アニメーションを初期化
	player_->ResetAnimation();
	animPlaySpeed_ = { 1.0f, 0.0f, 0.0f };
	player_->SetAnimationPlaySpeed(animPlaySpeed_.x);
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
		ImGui::DragFloat3("AnimSpeed", &animPlaySpeed_.x);

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
		.AddValue<float>("SwingTime", &invinsibleSwingTime)
		.AddValue<float>("InvinsibleTime", &invinsibleTime)
		.AddValue<float>("RecoveryTime", &invinsibleRecoveryTime)
		.EndGroup()
		.EndGroup()
		// 加速タイミングの設定
		.BeginGroup("Acceleration")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &accelerationSwingTime)
		.AddValue<float>("AccelerationTime", &accelerationTime)
		.AddValue<float>("RecoveryTime", &accelerationRecoveryTime)
		.EndGroup()
		.EndGroup()
		.EndGroup()
		// ダッシュに関する設定
		.BeginGroup("Dash")
		.AddValue<float>("ButtonHoldSeconds", &dashButtonHoldSeconds)
		.EndGroup()
		// 回避の終了時間
		.AddValue<float>("FinishTime", &evasionFinishTime)
		// 回避速度の倍率
		.AddValue<float>("MoveMultiply", &moveMultiply)
		// 回避の移動距離
		.AddValue<Vector3>("Movement", &evasionMovement)
		.CheckJsonFile();
}

void Evasion::Command() {
	if (eventOrder_.GetIsEnd()) {
		// 回避状態に移行
		player_->GetSystemManager()->SetInputState(InputState::kEvasion);
		pressTime_ = 0.0f;
		isActive_ = true;
	}
	eventOrder_.Start();
}

void Evasion::AnimCommand() {
	animationPlaySpeed_.Start();
	player_->ResetAnimation();
	player_->StartAnimation("Dash", 0.1f, 0.0f);
	player_->SetIsLoopAnimation(true);
}

void Evasion::CreateInvincibleEventOrder() {
	eventOrders_[(int)EventOrderState::kInvincible].Initialize();
	// 回避の無敵発生までの時間
	eventOrders_[(int)EventOrderState::kInvincible].CreateTimeEvent(TimeEvent{ invinsibleSwingTime * 60.0f, "SwingTime" });
	// 回避の無敵猶予時間
	eventOrders_[(int)EventOrderState::kInvincible].CreateTimeEvent(TimeEvent{ invinsibleTime * 60.0f, "InvinsibleTime" });
	// 回避の無敵硬直時間
	eventOrders_[(int)EventOrderState::kInvincible].CreateTimeEvent(TimeEvent{ invinsibleRecoveryTime * 60.0f, "RecoveryTime" });
}

void Evasion::CreateAccelerationEventOrder() {
	eventOrders_[(int)EventOrderState::kAcceleration].Initialize();
	// 回避の加速発生までの時間
	eventOrders_[(int)EventOrderState::kAcceleration].CreateTimeEvent(TimeEvent{ accelerationSwingTime * 60.0f, "SwingTime" });
	// 回避の加速時間
	eventOrders_[(int)EventOrderState::kAcceleration].CreateTimeEvent(TimeEvent{ accelerationTime * 60.0f, "AccelerationTime" });
	// 回避の加速硬直時間
	eventOrders_[(int)EventOrderState::kAcceleration].CreateTimeEvent(TimeEvent{ accelerationRecoveryTime * 60.0f, "RecoveryTime" });
}

void Evasion::CreateEventOrder() {
	// 回避終了時間
	if (eventOrder_.GetIsEnd()) {
		eventOrder_.Initialize();
		// 回避の無敵発生までの時間
		eventOrder_.CreateTimeEvent(TimeEvent{ evasionFinishTime * 60.0f, "FinishTime" });
	}
	// 無敵タイミング
	CreateInvincibleEventOrder();
	// 加速タイミング
	CreateAccelerationEventOrder();
}

void Evasion::CheckEvasionState() {
	//// 予備動作
	//if (eventOrder_.GetCurrentTimeEvent().name == "EvasionSwingTime") {

	//}
	//// 無敵時間
	//else if (eventOrder_.GetCurrentTimeEvent().name == "InvinsibleTime") {

	//}
	//// 硬直時間
	//else if (eventOrder_.GetCurrentTimeEvent().name == "EvasionRecoveryTime") {

	//}
}

void Evasion::CheckDash() {
	// 長押ししている間
	if (LWP::Input::Pad::GetPress(Command::GamePad::Evasion) || LWP::Input::Keyboard::GetPress(Command::Key::Evasion)) {
		pressTime_++;
		return;
	}
}

void Evasion::Move() {
	// 回避開始した瞬間
	if (GetTrigger()) {
		easeData_ = {
			&velocity_,
			Vector3{0,0,0},
			evasionMovement * Matrix4x4::CreateRotateXYZMatrix(player_->GetSystemManager()->GetRotate()),
			accelerationTime * 60.0f,
			0.0f,
			false
		};
	}

	// 回避の速度補間がなくなるまでイージングを行う
	if (easeData_.t < easeData_.endSecond) {
		easeData_.t++;
		// イージングを行う
		velocity_ = LWP::Utility::Interpolation::Lerp(easeData_.start, easeData_.end, LWP::Utility::Easing::OutExpo(easeData_.t / easeData_.endSecond)) * moveMultiply;
	}
	// 徐々に減速
	else {
		velocity_ = LWP::Utility::Interpolation::Exponential(velocity_, Vector3{ 0,0,0 }, 0.1f);
	}

	// 移動ベクトルから体の向きを算出(入力があるときのみ処理する)
	// 移動速度からラジアンを求める
	radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, velocity_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
}