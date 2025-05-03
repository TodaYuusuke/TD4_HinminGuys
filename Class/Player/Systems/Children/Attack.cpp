#include "Attack.h"
#include "../../Player.h"

using namespace LWP;
using namespace LWP::Object;
using namespace LWP::Object::Collider;

Attack::Attack(LWP::Object::Camera* camera, Player* player)
	: aabb_(collider_.SetBroadShape(LWP::Object::Collider::AABB()))
{
	pCamera_ = camera;
	player_ = player;

	// 攻撃の当たり判定作成
	CreateCollision();
}

void Attack::Initialize() {
	isActive_ = false;

	// フレーム単位で発生するアクションイベントを管理するクラス
	eventOrder_.Initialize();
	// 通常攻撃発生までの時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kNormalSwingTime, "NormalAttackSwingTime" });
	// 通常攻撃の猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kNormalAttackTime, "NormalAttackTime" });
	// 通常攻撃の硬直時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kNormalRecoveryTime , "NormalAttackRecoveryTime" });
}

void Attack::Update() {
	// 機能を使えないなら早期リターン
	if (!isActive_) { return; }

	// frameごとに起きるアクションイベント
	eventOrder_.Update();

	// 攻撃のアクションイベント状態の確認
	CheckAttackState();

	// 全てのアクションイベントが終了しているなら機能停止
	if (eventOrder_.GetIsEnd()) {
		Reset();
	}
}

void Attack::Reset() {
	isActive_ = false;
	isNormalAttack_ = false;
	collider_.isActive = false;
}

void Attack::DebugGUI() {
	if (ImGui::TreeNode("Attack")) {
		eventOrder_.DebugGUI();
		ImGui::Checkbox("IsNormalAttack", &isNormalAttack_);
		if (ImGui::TreeNode("Collider")) {
			collider_.DebugGUI();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void Attack::NormalCommand() {
	if (eventOrder_.GetIsEnd()) {
		isActive_ = true;
		collider_.isActive = true;
	}
	eventOrder_.Start();
}

void Attack::CreateCollision() {
	// 攻撃判定生成
	aabb_.min.y = 0.0f;
	aabb_.max.y = 1.0f;
	collider_.SetFollowTarget(player_->GetWorldTF());
	collider_.isActive = false;
	collider_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		if (isNormalAttack_) { return; }

		// 攻撃判定が出ているとき
		if (eventOrder_.GetCurrentTimeEvent().name == "NormalAttackTime") {
			collider_.isActive = true;
			isNormalAttack_ = true;
		}
		};
}

void Attack::CheckAttackState() {
	// 振りかぶりの時
	if (eventOrder_.GetCurrentTimeEvent().name == "NormalAttackSwingTime") {
		collider_.isActive = false;
		isNormalAttack_ = false;
	}
	// 硬直
	else if (eventOrder_.GetCurrentTimeEvent().name == "NormalAttackRecoveryTime") {
		collider_.isActive = false;
		isNormalAttack_ = false;
	}
}
