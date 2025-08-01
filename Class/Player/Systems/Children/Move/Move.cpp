#include "Move.h"
#include "../../../Player.h"
#include "State/Idle.h"
#include "State/Walk.h"
#include "State/Dash.h"
#include "../../../../Camera/FollowCamera.h"
#include "../../../Math/MathFunctions.h"
#include "../../../PlayerAudioNames.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;
using namespace PlayerAudio;

Move::Move(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;
	// ヒットストップの管理クラス
	hitStopController_ = HitStopController::GetInstance();
}

Move::~Move() {
	delete state_;
}

void Move::Initialize() {
	// 移動速度
	velocity_ = { 0.0f, 0.0f, 0.0f };
	// 向いている角度
	quat_ = { 0.0f,0.0f,0.0f,1.0f };
	radian_ = { 0.0f, 0.0f, 0.0f };

	// 移動状態を生成
	state_ = new Idle(this, player_);
	state_->Initialize();

	// 移動状態
	moveState_ = MoveState::kIdle;
}

void Move::Update() {
	// 入力処理
	InputUpdate();

	// 移動状態の遷移
	CheckMoveState();

	// 左右の足が着いた瞬間の演出
	EffectFootOnGround();

	// 入力のあったシステム
	nextSystem_ = CheckNextSystems();

	isPreActive_ = isActive_;
}

void Move::Reset() {
	// 移動速度
	velocity_ = { 0.0f, 0.0f, 0.0f };
	stickStrength_ = 0;
	isMove_ = false;
}

void Move::DebugGUI() {
	if (ImGui::TreeNode("Move")) {
		if (ImGui::TreeNode("Json")) {
			json_.DebugGUI();
			ImGui::TreePop();
		}

		ImGui::DragFloat3("Velocity", &velocity_.x, 0.1f, -10000, 10000);
		ImGui::DragFloat3("Rotation", &radian_.x, 0.1f, -6.28f, 6.28f);
		ImGui::DragFloat4("Quaternion", &quat_.x, 0.1f, -1, 1);
		ImGui::Checkbox("IsMove", &isMove_);
		ImGui::TreePop();
	}
}

void Move::CreateJsonFIle() {
	// 値を保存する項目を作成
	json_.Init("MoveData.json");
	json_.AddValue<float>("WalkSpeedMultiply", &jsonData_.walkSpeedMultiply)
		.AddValue<float>("RunSpeedMultiply", &jsonData_.runSpeedMultiply)
		.AddValue<float>("DashSpeedMultiply", &jsonData_.dashSpeedMultiply)
		.AddValue<float>("MoveSpeedRate", &jsonData_.moveSpeedRate)
		.AddValue<float>("RunThreshold", &jsonData_.runThreshold)
		.CheckJsonFile();
}

void Move::Command() {
	isActive_ = true;
}

void Move::AnimCommand() {
	
}

void Move::CheckMoveState() {
	// 待機状態に移行
	if (!GetIsMove()) {
		// 待機状態
		if (GetTriggerChangeMoveState(MoveState::kIdle) && stickStrength_ == 0) {
			moveState_ = MoveState::kIdle;
			// ダッシュ状態解除
			player_->GetSystemManager()->SetIsEnableDash(false);
			ChangeState(new Idle(this, player_));
		}
	}
	// 移動状態に移行
	else {
		// 走り状態に移行
		if (player_->GetSystemManager()->GetIsEnableDash() && stickStrength_ >= jsonData_.runThreshold) {
			// 走りモーション再生中なら状態遷移しない
			if (GetTriggerChangeMoveState(MoveState::kDash)) {
				moveState_ = MoveState::kDash;
				player_->GetSystemManager()->ComboReset();
				ChangeState(new Dash(this, player_, jsonData_.dashSpeedMultiply));
			}
		}
		// 通常移動状態に移行
		else {
			if (GetTriggerChangeMoveState(MoveState::kWalk)) {
				moveState_ = MoveState::kWalk;
				player_->GetSystemManager()->ComboReset();
				// ダッシュ状態解除
				player_->GetSystemManager()->SetIsEnableDash(false);
				ChangeState(new Walk(this, player_, jsonData_.runSpeedMultiply));
			}
		}
	}

	preMoveState_ = moveState_;
}

void Move::InputUpdate() {
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

	stickStrength_ = dir.Length();

	// カメラが向いている方向に進む
	// 回転行列を求める(X成分は0にする)
	Vector3 radian = player_->GetFollowCamera()->GetRadian();
	radian.x = 0.0f;
	Matrix4x4 rotMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(radian);
	// 方向ベクトルを求める
	velocity_ = LWP::Utility::Interpolation::Exponential(velocity_, dir * moveMultiply_ * rotMatrix * player_->GetParameter()->moveSpeedMultiply_, jsonData_.moveSpeedRate) * hitStopController_->GetDeltaTime();
	velocity_.y = 0.0f;

	// 移動状態
	state_->Update();

	isMove_ = false;
	// 移動ベクトルから体の向きを算出(入力があるときのみ処理する)
	if (LWP::Math::Vector3::Dot(MathFunc::Abs(dir), LWP::Math::Vector3{ 1,1,1 }) != 0) {
		// 移動速度からラジアンを求める
		radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, velocity_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
		quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
		isMove_ = true;
	}
}

void Move::EffectFootOnGround() {
	// 移動に関連するアニメーションでなければ処理しない
	if (!player_->GetAnimation()->GetPlaying("Dash", LWP::Resource::Animation::TrackType::Main) &&
		!player_->GetAnimation()->GetPlaying("Walk", LWP::Resource::Animation::TrackType::Main) &&
		!player_->GetAnimation()->GetPlaying("Run", LWP::Resource::Animation::TrackType::Blend)) {
		footState_ = FootState::kNone;
		currentMoveFrame_ = 0.0f;
		return;
	}

	float t = 0.0f;
	currentMoveFrame_ += HitStopController::GetInstance()->GetDeltaTime();
	if (player_->GetAnimation()->GetPlaying("Dash", LWP::Resource::Animation::TrackType::Main) || player_->GetAnimation()->GetPlaying("Walk", LWP::Resource::Animation::TrackType::Main)) {
		t = player_->GetAnimation()->GetTotalSeconds(LWP::Resource::Animation::TrackType::Main) / player_->GetAnimation()->GetPlayBackSpeed(LWP::Resource::Animation::TrackType::Main) * 60.0f;
	}
	else if (player_->GetAnimation()->GetPlaying("Run", LWP::Resource::Animation::TrackType::Blend)) {
		t = player_->GetAnimation()->GetTotalSeconds(LWP::Resource::Animation::TrackType::Blend) / player_->GetAnimation()->GetPlayBackSpeed(LWP::Resource::Animation::TrackType::Blend) * 60.0f;
	}

	// 地面につく足は左足
	if (t / 1.05f <= currentMoveFrame_) {
		currentMoveFrame_ += -t;
		footState_ = FootState::kLeft;
	}
	// 地面につく足は右足
	else if (t / 2.0f <= currentMoveFrame_) {
		footState_ = FootState::kRight;
	}

	// 左足
	if (footState_ == FootState::kLeft && preFootState_ != FootState::kLeft) {
		// パーティクル生成
		Vector3 pos = player_->GetModel()->GetJointWorldPosition("Foot.R");
		pos.y = 0.0f;
		player_->GetParticles()->CreateMoveParticle(pos);

		// ランダムで選択
		int audioNum = Utility::Random::GenerateInt(0, (int)SE::move.size() - 1);
		// 効果音再生
		player_->PlaySE(SE::move[audioNum].fileName, SE::move[audioNum].name, SE::move[audioNum].volume);
	}
	// 右足
	else if (footState_ == FootState::kRight && preFootState_ != FootState::kRight) {
		// パーティクル生成
		Vector3 pos = player_->GetModel()->GetJointWorldPosition("Foot.L");
		pos.y = 0.0f;
		player_->GetParticles()->CreateMoveParticle(pos);

		// ランダムで選択
		int audioNum = Utility::Random::GenerateInt(0, (int)SE::move.size() - 1);
		// 効果音再生
		player_->PlaySE(SE::move[audioNum].fileName, SE::move[audioNum].name, SE::move[audioNum].volume);
	}

	preFootState_ = footState_;
}

void Move::ChangeState(IMoveSystemState* pState) {
	delete state_;
	state_ = pState;
}