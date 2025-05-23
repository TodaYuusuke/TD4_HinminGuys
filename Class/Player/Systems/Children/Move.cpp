#include "Move.h"
#include "../../Player.h"
#include "State/Move/Idle.h"
#include "State/Move/Walk.h"
#include "State/Move/Run.h"
#include "State/Move/Dash.h"
#include "State/Move/None.h"
#include "State/Move/AttackRecovery.h"
#include "../../../Camera/FollowCamera.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;

Move::Move(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;
}

Move::~Move() {
	delete state_;
}

void Move::Initialize() {
	// 移動速度
	moveVel_ = { 0.0f, 0.0f, 0.0f };
	// 向いている角度
	quat_ = { 0.0f,0.0f,0.0f,1.0f };
	radian_ = { 0.0f, 0.0f, 0.0f };

	// jsonで保存している値
	CreateJsonFIle();

	// 移動状態を生成
	state_ = new Idle(this, player_);
	state_->Initialize();

	// 移動状態
	moveState_ = MoveState::kIdle;
	//preMoveState_ = moveState_;

	enableInput_ = true;
}

void Move::Update() {
	// 機能を使えないなら早期リターン
	if (!isActive_) {
		Reset();
		return;
	}

	// 入力処理
	InputUpdate();

	CheckMoveState();

	isPreActive_ = isActive_;
}

void Move::Reset() {
	// 移動速度
	moveVel_ = { 0.0f, 0.0f, 0.0f };
	stickStrength_ = 0;
	isMove_ = false;

	// 移動状態をなくす
	if (GetTriggerChangeMoveState(MoveState::kNone)) {
		moveState_ = MoveState::kNone;
		state_ = new None(this, player_);
		preMoveState_ = moveState_;
	}

}

void Move::DebugGUI() {
	if (ImGui::TreeNode("Move")) {
		if (ImGui::TreeNode("Json")) {
			json_.DebugGUI();
			ImGui::TreePop();
		}

		ImGui::DragFloat3("Velocity", &moveVel_.x, 0.1f, -10000, 10000);
		ImGui::DragFloat3("Rotation", &radian_.x, 0.1f, -6.28f, 6.28f);
		ImGui::DragFloat4("Quaternion", &quat_.x, 0.1f, -1, 1);
		ImGui::Checkbox("IsMove", &isMove_);
		ImGui::TreePop();
	}
}

void Move::CreateJsonFIle() {
	// 値を保存する項目を作成
	json_.Init("MoveData.json");
	json_.AddValue<float>("WalkSpeedMultiply", &walkSpeedMultiply)
		.AddValue<float>("RunSpeedMultiply", &runSpeedMultiply)
		.AddValue<float>("DashSpeedMultiply", &dashSpeedMultiply)
		.AddValue<float>("MoveSpeedRate", &moveSpeedRate)
		.AddValue<float>("RunThreshold", &runThreshold)
		.CheckJsonFile();
}

void Move::Command() {
	isActive_ = true;
	enableInput_ = true;
	player_->GetSystemManager()->SetInputState(InputState::kMove);
}

void Move::AnimCommand() {
	//CheckMoveState();
}

void Move::CheckMoveState() {
	// 待機状態に移行
	if (!GetIsMove()) {
		// 例外
		// 攻撃後の硬直中はIdleモーションを再生しない
		if (player_->GetSystemManager()->GetAttackSystem()->GetIsThisRoot()) {
			if (player_->GetSystemManager()->GetAttackSystem()->GetIsAttackRecovery()) {
				// 連続で同じ状態なら変更しないようにする
				if (GetTriggerChangeMoveState(MoveState::kAttackRecovery)) {
					moveState_ = MoveState::kAttackRecovery;
					player_->GetSystemManager()->GetEvasionSystem()->SetIsDash(false);
					ChangeState(new AttackRecovery(this, player_));
				}
				return;
			}		
		}

		// 待機状態
		if (GetTriggerChangeMoveState(MoveState::kIdle) && stickStrength_ == 0) {
			moveState_ = MoveState::kIdle;
			// ダッシュ状態解除
			player_->GetSystemManager()->GetEvasionSystem()->SetIsDash(false);
			ChangeState(new Idle(this, player_));
		}
	}
	// 移動状態に移行
	else {
		// もしも直前に攻撃をしていたら硬直フラグをfalseにしてAttackRecovery状態に移行しないようにする
		player_->GetSystemManager()->GetAttackSystem()->SetIsAttackRecovery(false);

		if (stickStrength_ > runThreshold) {
			// 走り状態に移行
			if (player_->GetSystemManager()->GetEvasionSystem()->GetIsDash()) {
			
				// 走りモーション再生中なら状態遷移しない
				if (GetTriggerChangeMoveState(MoveState::kDash)) {
					moveState_ = MoveState::kDash;
					ChangeState(new Dash(this, player_, dashSpeedMultiply));
				}
			}
			else {
				
				if (GetTriggerChangeMoveState(MoveState::kRun)) {
					moveState_ = MoveState::kRun;
					ChangeState(new Run(this, player_, runSpeedMultiply));
					// ダッシュ状態解除
					player_->GetSystemManager()->GetEvasionSystem()->SetIsDash(false);
				}
			}
		}
		// 歩行状態に移行
		else {
			
			if (GetTriggerChangeMoveState(MoveState::kWalk)) {
				moveState_ = MoveState::kWalk;
				// ダッシュ状態解除
				player_->GetSystemManager()->GetEvasionSystem()->SetIsDash(false);
				ChangeState(new Walk(this, player_, walkSpeedMultiply));
			}
		}
	}

	preMoveState_ = moveState_;
}

void Move::InputUpdate() {
	if (!enableInput_) {
		Reset();
		return;
	}

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
	// 自機とカメラY軸を除いた方向ベクトルを算出
	Vector3 p2c = (player_->GetWorldTF()->GetWorldPosition() - player_->GetFollowCamera()->GetDefaultPos()).Normalize();
	p2c.y = 0;
	// 回転行列を求める
	Matrix4x4 rotMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(LWP::Math::Quaternion::ConvertDirection(p2c));
	// 方向ベクトルを求める
	moveVel_ = LWP::Utility::Interpolation::Exponential(moveVel_, dir * moveMultiply_ * rotMatrix, moveSpeedRate);

	// 移動状態
	state_->Update();

	isMove_ = false;
	// 移動ベクトルから体の向きを算出(入力があるときのみ処理する)
	if (LWP::Math::Vector3::Dot(Abs(dir), LWP::Math::Vector3{ 1,1,1 }) != 0) {
		// 移動速度からラジアンを求める
		radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, moveVel_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
		quat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y);
		isMove_ = true;
	}
}

void Move::ChangeState(IMoveSystemState* pState) {
	delete state_;
	state_ = pState;
}