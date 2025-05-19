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
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &kEvasionSwingTime)
		.AddValue<float>("InvinsibleTime", &kInvinsibleTime)
		.AddValue<float>("RecoveryTime", &kEvasionRecoveryTime)
		.EndGroup()
		.EndGroup()
		.AddValue<float>("DashButtonHoldSeconds", &dashButtonHoldSeconds)
		.AddValue<float>("MoveMultiply", &moveMultiply)
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
	evasionEndPos_.translation = { 0,0,0 };
	t_ = 0;
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

void Evasion::Command() {
	if (eventOrder_.GetIsEnd()) {
		// 回避状態に移行
		player_->GetSystemManager()->SetInputState(InputState::kEvasion);
		pressTime_ = 0.0f;
		isActive_ = true;
		animationPlaySpeed_.Start();
		player_->ResetAnimation();
		player_->StartAnimation("Dash", 1.0f, 0.0f);
		player_->SetIsLoopAnimation(true);
	}
	eventOrder_.Start();
}

void Evasion::CreateEventOrder() {
	eventOrder_.Initialize();
	// 回避発生までの時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kEvasionSwingTime * 60.0f, "EvasionSwingTime" });
	// 回避の無敵猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kInvinsibleTime * 60.0f, "InvinsibleTime" });
	// 回避の硬直時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kEvasionRecoveryTime * 60.0f, "EvasionRecoveryTime" });
}

void Evasion::CheckEvasionState() {
	// 予備動作
	if (eventOrder_.GetCurrentTimeEvent().name == "EvasionSwingTime") {
		//collider_.isActive = false;
		//isJustParry_ = false;
		//isGoodParry_ = false;
	}
	// 無敵時間
	else if (eventOrder_.GetCurrentTimeEvent().name == "InvinsibleTime") {
		//collider_.isActive = false;
		//isJustParry_ = false;
		//isGoodParry_ = false;
	}
	// 硬直時間
	else if (eventOrder_.GetCurrentTimeEvent().name == "EvasionRecoveryTime") {
		//collider_.isActive = false;
		//isJustParry_ = false;
		//isGoodParry_ = false;
	}
}

void Evasion::CheckDash() {
	// 長押ししている間
	if (LWP::Input::Pad::GetPress(Command::GamePad::Evasion) || LWP::Input::Keyboard::GetPress(Command::Key::Evasion)) {
		pressTime_++;
		return;
	}
}

float Evasion::SmoothDampF(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime) {
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
LWP::Math::Vector3 Evasion::SmoothDamp(LWP::Math::Vector3 current, LWP::Math::Vector3 target, LWP::Math::Vector3& currentVelocity, float smoothTime, float maxSpeed, float deltaTime) {
	LWP::Math::Vector3 result = {
		SmoothDampF(current.x, target.x, currentVelocity.x,smoothTime, maxSpeed, deltaTime),
		SmoothDampF(current.y, target.y, currentVelocity.y,smoothTime, maxSpeed, deltaTime),
		SmoothDampF(current.z, target.z, currentVelocity.z,smoothTime, maxSpeed, deltaTime)
	};

	return result;
}

void Evasion::Move() {
	// 
	if (GetTrigger()) {
		evasionEndPos_.translation = player_->GetWorldTF()->GetWorldPosition() + evasionMovement * Matrix4x4::CreateRotateXYZMatrix(player_->GetSystemManager()->GetMoveSystem()->GetMoveQuat());
		evasionStartPos_ = player_->GetWorldTF()->GetWorldPosition();

		easeData_ = {
			&velocity_,
			evasionStartPos_,
			evasionStartPos_ + evasionMovement * Matrix4x4::CreateRotateXYZMatrix(player_->GetSystemManager()->GetMoveSystem()->GetMoveQuat()),
			false
		};
	}

	// 回避の速度補間がなくなるまでイージングを行う
	if (easeData_.t < 30.0f) {
		easeData_.t++;
		// イージングを行う
		velocity_ = LWP::Utility::Interpolation::Lerp(easeData_.start, easeData_.end, LWP::Utility::Easing::OutExpo(easeData_.t / 30.0f)) - player_->GetWorldTF()->GetWorldPosition();
	}

	// 移動ベクトルから体の向きを算出(入力があるときのみ処理する)
	// 移動速度からラジアンを求める
	radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, velocity_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
}