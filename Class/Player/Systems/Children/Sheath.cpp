#include "Sheath.h"
#include "State/Sheath/Throw.h"
#include "State/Sheath/Break.h"
#include "../../Player.h"
#include "../../../GameMask.h"

Sheath::Sheath(LWP::Object::Camera* camera, Player* player)
	: capsule_(collider_.SetBroadShape(LWP::Object::Collider::Capsule()))
{
	pCamera_ = camera;
	player_ = player;

	// 鞘
	sheathModel_.LoadShortPath("player/Sheath.gltf");
	sheathModel_.worldTF.scale = { 2.0f, 2.0f, 2.0f };
	sheathModel_.isActive = false;

	// ダッシュ攻撃の判定を作成
	CreateCollision();
}

void Sheath::Initialize() {
	// コマンドの登録
	inputHandler_ = InputHandler::GetInstance();

	// jsonで保存している値
	CreateJsonFIle();

	// アクションイベント作成
	CreateThrowEventOrder();
	CreateCollectEventOrder();
	CreateBreakEventOrder();
	CreateInvinsibleEventOrder();

	// 状態の生成
	state_ = new Throw(this, player_, &eventOrders_);
	state_->Initialize();
}

void Sheath::Update() {
	// クールタイムの時間更新
	CoolTimeUpdate();
	if (!isActive_) { return; }

	// 状態
	state_->Update();

	// カプセルの当たり判定を更新
	capsule_.end = dashAttackLength;

	// 無敵時間
	eventOrders_[(int)SheathState::kInvinsible].Update();

	// 無敵終了条件
	if (eventOrders_[(int)SheathState::kInvinsible].GetIsEnd()) {
		eventOrders_[(int)SheathState::kInvinsible].Reset();
	}

	isPreActive_ = isActive_;
}

void Sheath::Reset() {
	isActive_ = false;
	isPreActive_ = false;
	eventOrders_[(int)SheathState::kThrow].Reset();
	eventOrders_[(int)SheathState::kCollect].Reset();
	eventOrders_[(int)SheathState::kBreak].Reset();
	eventOrders_[(int)SheathState::kInvinsible].Reset();
	state_->Reset();
	// 移動速度
	velocity_ = { 0.0f,0.0f,0.0f };
	// 向いている角度
	quat_ = { 0.0f,0.0f,0.0f,1.0f };
	radian_ = { 0.0f,0.0f,0.0f };

	currentCoolTime_ = 0.0f;
}

void Sheath::DebugGUI() {
	if (ImGui::TreeNode("Sheath")) {
		if (ImGui::TreeNode("Json")) {
			json_.DebugGUI();
			// 鞘投げのアクションイベントを実行してないときのみ変更可能
			if (eventOrders_[(int)SheathState::kThrow].GetIsEnd()) {
				// アクションイベントを再登録
				eventOrders_[(int)SheathState::kThrow].Initialize();
				CreateThrowEventOrder();
			}
			// 鞘回収のアクションイベントを実行してないときのみ変更可能
			if (eventOrders_[(int)SheathState::kCollect].GetIsEnd()) {
				// アクションイベントを再登録
				eventOrders_[(int)SheathState::kCollect].Initialize();
				CreateCollectEventOrder();
			}
			// ダッシュ攻撃のアクションイベントを実行してないときのみ変更可能
			if (eventOrders_[(int)SheathState::kBreak].GetIsEnd()) {
				// アクションイベントを再登録
				eventOrders_[(int)SheathState::kBreak].Initialize();
				CreateBreakEventOrder();
			}
			// 無敵のアクションイベントを実行してないときのみ変更可能
			if (eventOrders_[(int)SheathState::kInvinsible].GetIsEnd()) {
				// アクションイベントを再登録
				eventOrders_[(int)SheathState::kInvinsible].Initialize();
				CreateInvinsibleEventOrder();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Throw")) {
			eventOrders_[(int)SheathState::kThrow].DebugGUI();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Collect")) {
			eventOrders_[(int)SheathState::kCollect].DebugGUI();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("DashAttack")) {
			eventOrders_[(int)SheathState::kBreak].DebugGUI();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Invinsible")) {
			eventOrders_[(int)SheathState::kInvinsible].DebugGUI();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Collider")) {
			collider_.DebugGUI();
			ImGui::TreePop();
		}

		ImGui::DragFloat3("Velocity", &velocity_.x);
		ImGui::DragFloat3("Radian", &radian_.x);
		ImGui::DragFloat("CoolTime", &currentCoolTime_);

		ImGui::Checkbox("IsEvasion", &isActive_);

		ImGui::TreePop();
	}
}

void Sheath::CreateJsonFIle() {
	json_.Init("SheathData.json");
	json_.BeginGroup("Throw")
		// 鞘投げの設定
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &throwSwingTime)
		.AddValue<float>("ThrowFinishTime", &throwTime)
		.AddValue<float>("RecoveryTime", &throwRecoveryTime)
		.EndGroup()
		// 鞘投げの移動距離
		.AddValue<Vector3>("Movement", &throwMovement)
		.EndGroup()

		// 鞘回収の設定
		.BeginGroup("Collect")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &collectSwingTime)
		.AddValue<float>("CollectFinishTime", &collectTime)
		.AddValue<float>("RecoveryTime", &collectRecoveryTime)
		.EndGroup()
		.EndGroup()

		// ダッシュ攻撃の設定
		.BeginGroup("DashAttack")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &dashAttackSwingTime)
		.AddValue<float>("DashAttackFinishTime", &dashAttackFinishTime)
		.AddValue<float>("RecoveryTime", &dashAttackRecoveryTime)
		.EndGroup()
		// 鞘投げの移動距離
		.AddValue<Vector3>("Movement", &dashAttackMovement)
		.EndGroup()

		// 無敵の設定
		.BeginGroup("Invinsible")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &invinsibleSwingTime)
		.AddValue<float>("InvinsibleTime", &invinsibleFinishTime)
		.AddValue<float>("RecoveryTime", &invinsibleRecoveryTime)
		.EndGroup()
		.EndGroup()

		// 攻撃の判定
		.BeginGroup("Collider")
		.AddValue<float>("Radius", &capsule_.radius)
		.AddValue<Vector3>("Length", &dashAttackLength)
		.EndGroup()

		// 移動可能範囲
		.AddValue<float>("MoveRange", &enableMoveRange)
		// クールタイム
		.AddValue<float>("CoolTime", &coolTime)

		.CheckJsonFile();
}

void Sheath::Command() {
	// 鞘破壊状態に移行
	if (player_->GetUIManager()->GetSheathGauge().GetIsIncrease() && !isBreak_ && !isActive_) {
		isBreak_ = true;
		ChangeState(new Break(this, player_, &eventOrders_));
	}
	isActive_ = true;
	// 状態によって変更
	state_->Command();
	// 状態によって変更
	state_->AnimCommand();
}

void Sheath::AnimCommand() {
	// 状態によって変更
	state_->AnimCommand();
}

void Sheath::CreateCollision() {
	// 攻撃判定生成
	collider_.SetFollow(player_->GetWorldTF());
	collider_.isActive = false;
	collider_.worldTF.translation = { 0.0f, 1.0f, 0.0f };
	collider_.mask.SetBelongFrag(GameMask::GetAttack());
	collider_.mask.SetHitFrag(GameMask::GetEnemy());
}

void Sheath::CreateThrowEventOrder() {
	eventOrders_[(int)SheathState::kThrow].Initialize();
	// 回避の無敵発生までの時間
	eventOrders_[(int)SheathState::kThrow].CreateTimeEvent(TimeEvent{ throwSwingTime * 60.0f, "SwingTime" });
	// 回避の無敵猶予時間
	eventOrders_[(int)SheathState::kThrow].CreateTimeEvent(TimeEvent{ throwTime * 60.0f, "ThrowFinishTime" });
	// 回避の無敵硬直時間
	eventOrders_[(int)SheathState::kThrow].CreateTimeEvent(TimeEvent{ throwRecoveryTime * 60.0f, "RecoveryTime" });
}

void Sheath::CreateCollectEventOrder() {
	eventOrders_[(int)SheathState::kCollect].Initialize();
	// 回避の加速発生までの時間
	eventOrders_[(int)SheathState::kCollect].CreateTimeEvent(TimeEvent{ collectSwingTime * 60.0f, "SwingTime" });
	// 回避の加速時間
	eventOrders_[(int)SheathState::kCollect].CreateTimeEvent(TimeEvent{ collectTime * 60.0f, "CollectFinishTime" });
	// 回避の加速硬直時間
	eventOrders_[(int)SheathState::kCollect].CreateTimeEvent(TimeEvent{ collectRecoveryTime * 60.0f, "RecoveryTime" });
}

void Sheath::CreateBreakEventOrder() {
	eventOrders_[(int)SheathState::kBreak].Initialize();
	// ダッシュ攻撃発生までの時間
	eventOrders_[(int)SheathState::kBreak].CreateTimeEvent(TimeEvent{ dashAttackSwingTime * 60.0f, "SwingTime" });
	// ダッシュ攻撃時間
	eventOrders_[(int)SheathState::kBreak].CreateTimeEvent(TimeEvent{ dashAttackFinishTime * 60.0f, "DashAttackFinishTime" });
	// ダッシュ攻撃硬直時間
	eventOrders_[(int)SheathState::kBreak].CreateTimeEvent(TimeEvent{ dashAttackRecoveryTime * 60.0f, "RecoveryTime" });
}

void Sheath::CreateInvinsibleEventOrder() {
	eventOrders_[(int)SheathState::kInvinsible].Initialize();
	// 無敵発生までの時間
	eventOrders_[(int)SheathState::kInvinsible].CreateTimeEvent(TimeEvent{ invinsibleSwingTime * 60.0f, "SwingTime" });
	// 無敵時間
	eventOrders_[(int)SheathState::kInvinsible].CreateTimeEvent(TimeEvent{ invinsibleFinishTime * 60.0f, "InvinsibleTime" });
	// 無敵硬直
	eventOrders_[(int)SheathState::kInvinsible].CreateTimeEvent(TimeEvent{ invinsibleRecoveryTime * 60.0f, "RecoveryTime" });
}

void Sheath::ChangeState(ISheathSystemState* pState) {
	delete state_;
	state_ = pState;
}

void Sheath::CoolTimeUpdate() {
	// 既定の時間を越していなかったら鞘投げを使えない
	if (CheckCoolTime()) {
		return;
	}

	currentCoolTime_--;
}

LWP::Math::Vector3 Sheath::ClampToCircle(LWP::Math::Vector3& position) {
	LWP::Math::Vector3 offset = position - sheathModel_.worldTF.GetWorldPosition();
	float distance = offset.Length();

	if (distance > enableMoveRange) {
		offset = offset.Normalize() * enableMoveRange;
		position = sheathModel_.worldTF.GetWorldPosition() + offset;
	}

	return position;
}