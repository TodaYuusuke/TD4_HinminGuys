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

Attack::Attack(LWP::Object::Camera* camera, Player* player)
	: aabb_(collider_.SetBroadShape(LWP::Object::Collider::AABB()))
{
	pCamera_ = camera;
	player_ = player;

	// 攻撃の当たり判定作成
	CreateCollision();

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
	isActive_ = false;
	isPreActive_ = false;

	json_.Init("AttackData.json");
	json_.BeginGroup("EventOrder")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &kNormalSwingTime)
		.AddValue<float>("AttackTime", &kNormalAttackTime)
		.AddValue<float>("AttackRecoveryTime", &kNormalRecoveryTime)
		.EndGroup()
		.EndGroup()
		.CheckJsonFile();

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

	//// 機能を使えないなら早期リターン
	//if (!isActive_) {
	//	return;
	//}

	//// frameごとに起きるアクションイベント
	//eventOrder_.Update();

	//// 攻撃のアクションイベント状態の確認
	//CheckAttackState();

	//// 状態
	//state_->Update();

	//// 全てのアクションイベントが終了しているなら機能停止
	//if (eventOrder_.GetIsEnd()) {
	//	Reset();
	//}

	isPreActive_ = isActive_;
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

void Attack::NormalCommand() {
	
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