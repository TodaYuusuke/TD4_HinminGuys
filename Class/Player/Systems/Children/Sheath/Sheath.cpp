#include "Sheath.h"
#include "State/Throw.h"
#include "State/Break.h"
#include "../../../Player.h"
#include "../../../../GameMask.h"
#include "../../../Math/MathFunctions.h"

Sheath::Sheath(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;
	// ヒットストップの管理クラス
	hitStopController_ = HitStopController::GetInstance();

	// 鞘
	sheathModel_.LoadShortPath("player/Sheath.gltf");
	sheathModel_.worldTF.scale = { 2.0f, 2.0f, 2.0f };
	sheathModel_.isActive = false;

	// 鞘のある状態
	isSheathing_ = true;
	// 機能を稼働させない
	isActive_ = false;

	// 鞘が相手に当たった時の処理
	player_->GetSystemManager()->SetSheathOnHitFunc(
		[this](LWP::Object::Collision* hitTarget) {
			// 鞘が破壊されているなら処理しない
			if (isBreak_) { return; }
			if (!hitTargetNames_.empty()) { return; }

			hitTargetNames_.push_back(hitTarget->name);

			// 鞘のゲージを減少
			player_->TakeSheathDamage(player_->GetParameter()->GetCurrentSheathDamageStrength());
		});
	// 鞘攻撃が当たった時の処理
	player_->GetSystemManager()->SetSheathAttackOnHitFunc(
		[this](LWP::Object::Collision* hitTarget) {
			// 鞘が破壊されているなら処理しない
			if (isBreak_) { return; }
			if (!hitTargetNames_.empty()) { return; }// 一度誰かと衝突したことがあるなら早期リターン
			hitTargetNames_.push_back(hitTarget->name);

			// 鞘のゲージを減少
			player_->TakeSheathDamage(player_->GetParameter()->GetCurrentSheathDamageStrength());
		});
	// 鞘の当たり判定を演出用の鞘に追従させる
	player_->GetSystemManager()->GetSheathCollision().worldTF.Parent(&sheathModel_.worldTF);

	// 浮遊パーティクル
	floatParticle_ = std::make_unique<FloatParticle>(player_);
	floatParticle_->model.LoadCube();
	// オーラ
	auraParticles_ = std::make_unique<AuraParticles>();
	auraParticles_->Initialize();
	auraParticles_->SetTexName("Effect/Particle.png");
}

void Sheath::Initialize() {
	// コマンドの登録
	inputHandler_ = InputHandler::GetInstance();

	chain_ = std::make_unique<Chain>();

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
	// オーラ
	auraParticles_->Update();

	// 鎖の始点終点を指定
	chain_->SetStartPos(player_->GetSwordModel()->GetJointWorldPosition("Grip")); 
	chain_->SetEndPos(sheathModel_.GetJointWorldPosition("Sheath"));
	chain_->Update();

	if (!isActive_) { return; }

	// 状態
	state_->Update();

	// カプセルの当たり判定を更新
	player_->GetSystemManager()->GetSheathAttackCapsule().end = jsonData_.dashAttackLength;

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

		// 鎖
		if (ImGui::TreeNode("Chain")) {
			chain_->DebugGui();
			chain_->SetStartPos(player_->GetSwordModelWorldTF()->GetWorldPosition());
			chain_->SetEndPos(player_->GetSheathModelWorldTF()->GetWorldPosition());
			ImGui::TreePop();
		}

		ImGui::DragFloat3("Velocity", &velocity_.x);
		ImGui::DragFloat3("Radian", &radian_.x);
		int size = (int)hitTargetNames_.size();
		ImGui::DragInt("HitTargetNames", &size);
		bool is = player_->GetSystemManager()->GetSheathCollision().isActive;
		ImGui::Checkbox("IsSheathCollision", &is);

		ImGui::Checkbox("IsEvasion", &isActive_);

		ImGui::TreePop();
	}
}

void Sheath::CreateJsonFIle() {
	json_.Init("SheathData.json");
	json_.BeginGroup("Throw")
		// 鞘投げの設定
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &jsonData_.throwSwingTime)
		.AddValue<float>("ThrowFinishTime", &jsonData_.throwTime)
		.AddValue<float>("RecoveryTime", &jsonData_.throwRecoveryTime)
		.EndGroup()
		// 鞘投げの移動距離
		.AddValue<LWP::Math::Vector3>("Movement", &jsonData_.throwMovement)
		.EndGroup()

		// 鞘回収の設定
		.BeginGroup("Collect")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &jsonData_.collectSwingTime)
		.AddValue<float>("CollectFinishTime", &jsonData_.collectTime)
		.AddValue<float>("RecoveryTime", &jsonData_.collectRecoveryTime)
		.EndGroup()
		// 鞘ゲージ減少量[%]
		.AddValue<float>("SheathDecrementPercent", &jsonData_.collectAttackSheathDecrementPercent)
		.EndGroup()

		// ダッシュ攻撃の設定
		.BeginGroup("DashAttack")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &jsonData_.dashAttackSwingTime)
		.AddValue<float>("DashAttackFinishTime", &jsonData_.dashAttackFinishTime)
		.AddValue<float>("RecoveryTime", &jsonData_.dashAttackRecoveryTime)
		.EndGroup()
		// 鞘投げの移動距離
		.AddValue<LWP::Math::Vector3>("Movement", &jsonData_.dashAttackMovement)
		.EndGroup()

		// 無敵の設定
		.BeginGroup("Invinsible")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &jsonData_.invinsibleSwingTime)
		.AddValue<float>("InvinsibleTime", &jsonData_.invinsibleFinishTime)
		.AddValue<float>("RecoveryTime", &jsonData_.invinsibleRecoveryTime)
		.EndGroup()
		.EndGroup()

		// 攻撃の判定
		.BeginGroup("Collider")
		.AddValue<float>("Radius", &player_->GetSystemManager()->GetSheathAttackCapsule().radius)
		.AddValue<LWP::Math::Vector3>("Length", &jsonData_.dashAttackLength)
		.EndGroup()

		// 攻撃力
		.BeginGroup("AttackValue")
		// 鞘自体の攻撃力
		.AddValue<float>("Sheath", &jsonData_.sheathAttackValue)
		// ダッシュ攻撃の攻撃力
		.AddValue<float>("Dash", &jsonData_.dashAttackValue)
		// 回収時の攻撃の威力
		.AddValue<float>("Collect", &jsonData_.collectAttackValue)
		.EndGroup();
		
		// パーティクル
		json_.BeginGroup("FloatParticle");
		floatParticle_->SetJsonData(json_);
		json_.EndGroup();
		// パーティクル
		json_.BeginGroup("AuraParticle");
		auraParticles_->SetJsonData(json_);
		json_.EndGroup();

		// 移動可能範囲
		json_.AddValue<float>("MoveRange", &jsonData_.enableMoveRange)
		// クールタイム
		.AddValue<float>("CoolTime", &jsonData_.coolTime)

		.CheckJsonFile();
}

void Sheath::Command() {
	// 鞘破壊状態に移行
	if (player_->GetUIManager()->GetSheathGauge().GetIsIncrease() && !isBreak_ && !isActive_) {
		isBreak_ = true;
		ChangeState(new Break(this, player_, &eventOrders_));
	}
	// 状態によって変更
	state_->Command();
	// 状態によって変更
	state_->AnimCommand();
}

void Sheath::AnimCommand() {
	// 状態によって変更
	state_->AnimCommand();
}

void Sheath::CreateThrowEventOrder() {
	eventOrders_[(int)SheathState::kThrow].Initialize();
	// 回避の無敵発生までの時間
	eventOrders_[(int)SheathState::kThrow].CreateTimeEvent(TimeEvent{ jsonData_.throwSwingTime * 60.0f, "SwingTime" });
	// 回避の無敵猶予時間
	eventOrders_[(int)SheathState::kThrow].CreateTimeEvent(TimeEvent{ jsonData_.throwTime * 60.0f, "ThrowFinishTime" });
	// 回避の無敵硬直時間
	eventOrders_[(int)SheathState::kThrow].CreateTimeEvent(TimeEvent{ jsonData_.throwRecoveryTime * 60.0f, "RecoveryTime" });
}

void Sheath::CreateCollectEventOrder() {
	float graceTime = (sheathModel_.worldTF.GetWorldPosition() - player_->GetWorldTF()->GetWorldPosition()).Length() / jsonData_.enableMoveRange * jsonData_.collectTime * 60.0f;

	eventOrders_[(int)SheathState::kCollect].Initialize();
	// 回避の加速発生までの時間
	eventOrders_[(int)SheathState::kCollect].CreateTimeEvent(TimeEvent{ jsonData_.collectSwingTime * 60.0f, "SwingTime" });
	// 回避の加速時間
	eventOrders_[(int)SheathState::kCollect].CreateTimeEvent(TimeEvent{ graceTime, "CollectFinishTime" });
	// 回避の加速硬直時間
	eventOrders_[(int)SheathState::kCollect].CreateTimeEvent(TimeEvent{ jsonData_.collectRecoveryTime * 60.0f, "RecoveryTime" });
}

void Sheath::CreateBreakEventOrder() {
	eventOrders_[(int)SheathState::kBreak].Initialize();
	// ダッシュ攻撃発生までの時間
	eventOrders_[(int)SheathState::kBreak].CreateTimeEvent(TimeEvent{ jsonData_.dashAttackSwingTime * 60.0f, "SwingTime" });
	// ダッシュ攻撃時間
	eventOrders_[(int)SheathState::kBreak].CreateTimeEvent(TimeEvent{ jsonData_.dashAttackFinishTime * 60.0f, "DashAttackFinishTime" });
	// ダッシュ攻撃硬直時間
	eventOrders_[(int)SheathState::kBreak].CreateTimeEvent(TimeEvent{ jsonData_.dashAttackRecoveryTime * 60.0f, "RecoveryTime" });
}

void Sheath::CreateInvinsibleEventOrder() {
	eventOrders_[(int)SheathState::kInvinsible].Initialize();
	// 無敵発生までの時間
	eventOrders_[(int)SheathState::kInvinsible].CreateTimeEvent(TimeEvent{ jsonData_.invinsibleSwingTime * 60.0f, "SwingTime" });
	// 無敵時間
	eventOrders_[(int)SheathState::kInvinsible].CreateTimeEvent(TimeEvent{ jsonData_.invinsibleFinishTime * 60.0f, "InvinsibleTime" });
	// 無敵硬直
	eventOrders_[(int)SheathState::kInvinsible].CreateTimeEvent(TimeEvent{ jsonData_.invinsibleRecoveryTime * 60.0f, "RecoveryTime" });
}

void Sheath::ChangeState(ISheathSystemState* pState) {
	delete state_;
	state_ = pState;
}

LWP::Math::Vector3 Sheath::ClampToCircle(LWP::Math::Vector3& position) {
	// 移動可能範囲を円でとる
	LWP::Math::Vector2 offset = LWP::Math::Vector2{ position.x,position.z } - LWP::Math::Vector2{ sheathModel_.worldTF.GetWorldPosition().x,sheathModel_.worldTF.GetWorldPosition().z };
	float distance = offset.Length();

	if (distance > jsonData_.enableMoveRange) {
		// 移動可能範囲の設定
		offset = offset.Normalize() * jsonData_.enableMoveRange;
		// 鞘の座標
		LWP::Math::Vector3 sheathPos = sheathModel_.worldTF.GetWorldPosition();
		sheathPos.y = player_->GetWorldTF()->GetWorldPosition().y;// 自機基準
		position = sheathPos + LWP::Math::Vector3{offset.x, 0.0f, offset.y};
	}

	return position;
}