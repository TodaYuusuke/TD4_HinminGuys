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

	// 攻撃判定生成
	aabb_.min.y = 0.0f;
	aabb_.max.y = 1.0f;
	collider_.SetFollowTarget(player_->GetWorldTF());
	collider_.isActive = true;
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

	// frameごとに起きるイベント
	eventOrder_.Update();

	// 全てのイベントが終了しているなら機能停止
	if (eventOrder_.GetIsEnd()) {
		Reset();
	}
}

void Attack::Reset() {
	isActive_ = false;
}

void Attack::DebugGUI() {
	if (ImGui::TreeNode("Attack")) {
		collider_.DebugGUI();
		ImGui::TreePop();
	}
}

void Attack::NormalCommand() {
	eventOrder_.Start();
	isActive_ = true;
}