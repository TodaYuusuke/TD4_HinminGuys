#include "Attack.h"
#include "../../Player.h"
#include "State/Attack/DefaultAttack.h"
#include "State/Attack/LockOnAttack.h"
#include "State/Attack/NoneAttack.h"
#include "../../../GameMask.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Object;
using namespace LWP::Object::Collider;
using namespace GameMask;

// 通常攻撃発動までにかかる時間[秒]
float Attack::kNormalSwingTime;
// 通常攻撃の猶予時間[秒]
float Attack::kNormalAttackTime;
// 通常攻撃の硬直[秒]
float Attack::kNormalRecoveryTime;

Attack::Attack(LWP::Object::Camera* camera, Player* player)
	: aabb_(collider_.SetBroadShape(LWP::Object::Collider::AABB()))
{
	pCamera_ = camera;
	player_ = player;

	// 攻撃の当たり判定作成
	CreateCollision();

	nextState_ = InputNone;
	currentState_ = InputAttack;

	// 状態作成
	state_ = new NoneAttack(this);
	state_->Initialize();

	// コンボツリーの初期化
	comboTree_.Init("Combo.json", player_->GetModel(), player_->GetAnimation());
}

Attack::~Attack() {
	delete state_;
}

void Attack::Initialize() {
	// コマンドの登録
	inputHandler_ = InputHandler::GetInstance();
	isActive_ = false;
	isPreActive_ = false;

	// jsonで保存している値
	CreateJsonFIle();

	// フレーム単位で発生するアクションイベントを管理するクラス
	CreateEventOrder();
}

void Attack::Update() {

	// コンボツリー自体は毎フレーム更新する
	comboTree_.Update();

	// コンボが無操作状態のコンボでない場合
	if (!comboTree_.GetIsThisRoot()) {
		// 攻撃しているものとみなし、攻撃状態に移行
		if (!isActive_) {
			player_->GetSystemManager()->SetInputState(InputState::kAttack);
			isActive_ = true;
			//player_->ResetAnimation();
		}
	}
	else { // 無操作状態のコンボが選択されている場合
		// 機能停止させる
		if (isActive_) {
			Reset();
		}
	}

	// 攻撃アシストが有効になっている場合
	if (comboTree_.GetIsEnableAttackAssist()) {
		// ロックオン中なら対象に近づいて攻撃
		if (lockOnSystem_->GetCurrentLockOnTarget()) {
			// 自機とロックオン中の敵との距離
			Vector3 attackTargetDist = (lockOnTarget_->GetWorldTF()->GetWorldPosition() - player_->GetWorldTF()->GetWorldPosition()) * 0.4f;
			attackAssistVel_ = LWP::Utility::Interpolation::Slerp(comboTree_.GetAttackAssistMoveAmount(), attackTargetDist, 0.25f);

			// 移動速度からラジアンを求める
			attackAssistRadian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, attackAssistVel_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
			attackAssistQuat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, attackAssistVel_.y);
		}
		else {
			// 攻撃の移動量の取得
			attackAssistVel_ = comboTree_.GetAttackAssistMoveAmount();

			// 自機の方向ベクトル
			Vector3 playerDir = { 0.0f,0.0f,1.0f };
			// 回転行列を求める
			Matrix4x4 rotMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(player_->GetSystemManager()->GetMoveSystem()->GetMoveQuat());
			// 方向ベクトルを求める
			playerDir = playerDir * rotMatrix;
			playerDir.y = 0;

			attackAssistVel_ = attackAssistVel_ * rotMatrix;

			// 移動速度からラジアンを求める
			attackAssistRadian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, playerDir.Normalize(), LWP::Math::Vector3{ 0,1,0 });
			attackAssistQuat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, attackAssistRadian_.y);
		}
	}
}

void Attack::Reset() {
	isActive_ = false;
	isNormalAttack_ = false;
	collider_.isActive = false;
	aabb_.isShowWireFrame = false;
	attackAssistVel_ = { 0.0f,0.0f,0.0f };
	attackAssistRadian_ = { 0.0f,0.0f,0.0f };
	attackAssistQuat_ = { 0.0f,0.0f,0.0f,1.0f };
	// アニメーションを初期化
	player_->ResetAnimation();
}

void Attack::DebugGUI() {
	if (ImGui::TreeNode("Attack")) {
		// パリィのアクションイベントを保存
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

		// 当たり判定
		if (ImGui::TreeNode("Collider")) {
			collider_.DebugGUI();
			ImGui::TreePop();
		}

		ImGui::DragFloat3("Velocity", &attackAssistVel_.x, 0.1f, -10000, 10000);
		ImGui::DragFloat3("Rotation", &attackAssistRadian_.x, 0.1f, -6.28f, 6.28f);
		ImGui::DragFloat4("Quaternion", &attackAssistQuat_.x, 0.1f, -1, 1);
		ImGui::Checkbox("IsNormalAttack", &isNormalAttack_);

		ImGui::TreePop();
	}
}

void Attack::CreateJsonFIle() {
	json_.Init("AttackData.json");
	json_.BeginGroup("EventOrder")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &kNormalSwingTime)
		.AddValue<float>("AttackTime", &kNormalAttackTime)
		.AddValue<float>("AttackRecoveryTime", &kNormalRecoveryTime)
		.EndGroup()
		.EndGroup()
		.CheckJsonFile();
}

void Attack::NormalCommand() {
	isActive_ = true;

	//if (eventOrder_.GetIsEnd()) {
	//	// 攻撃状態に移行
	//	player_->GetSystemManager()->SetInputState(InputState::kAttack);
	//	isActive_ = true;
	//	collider_.isActive = true;
	//	aabb_.isShowWireFrame = true;
	//	player_->ResetAnimation();
	//	player_->StartAnimation("LightAttack1", 0.6f, 0.0f);
	//}
	//eventOrder_.Start();
}

void Attack::ChangeState(IAttackSystemState* pState) {
	delete state_;
	state_ = pState;
}

void Attack::CreateCollision() {
	// 攻撃判定生成
	aabb_.isShowWireFrame = false;
	collider_.SetFollowTarget(player_->GetWorldTF());
	collider_.worldTF.translation = { 0,1,2 };
	collider_.isActive = false;
	collider_.mask.SetBelongFrag(GetAttack());
	collider_.mask.SetHitFrag(GetEnemy());
	collider_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;

		// 攻撃判定が出ているとき
		if (eventOrder_.GetCurrentTimeEvent().name == "NormalAttackTime") {
			collider_.isActive = true;
			isNormalAttack_ = true;
		}
		};
}

void Attack::CreateEventOrder() {
	eventOrder_.Initialize();
	// 通常攻撃発生までの時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kNormalSwingTime * 60.0f, "NormalAttackSwingTime" });
	// 通常攻撃の猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kNormalAttackTime * 60.0f, "NormalAttackTime" });
	// 通常攻撃の硬直時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kNormalRecoveryTime * 60.0f, "NormalAttackRecoveryTime" });
}

void Attack::CheckAttackState() {
	// 振りかぶりの時
	if (eventOrder_.GetCurrentTimeEvent().name == "NormalAttackSwingTime") {
		collider_.isActive = false;
		isNormalAttack_ = false;
		// 攻撃が当たる位置に自機を移動させる
		AttackAssistMovement();
	}
	else if (eventOrder_.GetCurrentTimeEvent().name == "NormalAttackTime") {
		collider_.isActive = true;
	}
	// 硬直
	else if (eventOrder_.GetCurrentTimeEvent().name == "NormalAttackRecoveryTime") {
		collider_.isActive = false;
		isNormalAttack_ = false;
	}
	// 振りかぶり以外の時なら状態をリセット
	else {
		ChangeState(new NoneAttack(this));
	}
}

void Attack::AttackAssistMovement() {
	if (!GetTrigger()) { return; }

	// ロックオン中なら対象に近づいてから攻撃
	if (lockOnSystem_->GetCurrentLockOnTarget()) {
		ChangeState(new LockOnAttack(this, player_, lockOnSystem_->GetCurrentLockOnTarget()));
	}
	else {
		ChangeState(new DefaultAttack(this, player_));
	}
}