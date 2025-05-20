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
	// 機能を使えないなら早期リターン
	if (!isActive_) {
		isMoveInput_ = true;
		return;
	}

	// frameごとに起きるアクションイベント
	eventOrder_.Update();

	// 攻撃のアクションイベント状態の確認
	CheckAttackState();

	// 状態
	state_->Update();

	// 全てのアクションイベントが終了しているなら機能停止
	if (eventOrder_.GetIsEnd()) {
		Reset();
	}

	isPreActive_ = isActive_;
}

void Attack::Reset() {
	isActive_ = false;
	isMoveInput_ = true;
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
	if (eventOrder_.GetIsEnd()) {
		// 攻撃状態に移行
		player_->GetSystemManager()->SetInputState(InputState::kAttack);
		isActive_ = true;
		isMoveInput_ = false;
		collider_.isActive = true;
		aabb_.isShowWireFrame = true;
		player_->ResetAnimation();
		player_->StartAnimation("LightAttack1", 0.6f, 0.0f);
	}
	eventOrder_.Start();
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