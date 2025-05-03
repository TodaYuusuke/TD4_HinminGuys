#include "Parry.h"
#include "../../Player.h"
#include "../Engine/object/core/collision/Collision.h"

Parry::Parry(LWP::Object::Camera* camera, Player* player)
	: aabb_(collider_.SetBroadShape(LWP::Object::Collider::AABB()))
{
	pCamera_ = camera;
	player_ = player;

	// パリィ判定生成
	CreateCollision();
}

void Parry::Initialize() {
	isActive_ = false;

	// フレーム単位で発生するアクションイベントを管理するクラス
	eventOrder_.Initialize();
	// パリィ発生までの時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kSwingTime, "SwingTime" });
	// ジャストパリィの猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kJustParryTime, "JustParry" });
	// 通常パリィの猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kGoodParryTime , "GoodParry" });
	// パリィの硬直時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kRecoveryTime , "RecoveryTime" });
}

void Parry::Update() {
	// パリィ機能を使えないなら早期リターン
	if (!isActive_) { return; }

	// frameごとに起きるイベント
	eventOrder_.Update();

	// パリィの状態確認
	CheckParryState();

	// 全てのイベントが終了しているなら機能停止
	if (eventOrder_.GetIsEnd()) {
		Reset();
	}
}

void Parry::Reset() {
	isActive_ = false;
	isJustParry_ = false;
	isGoodParry_ = false;
	collider_.isActive = false;	
}

void Parry::DebugGUI() {
	if (ImGui::TreeNode("Parry")) {
		eventOrder_.DebugGUI();
		ImGui::Checkbox("IsJustParry", &isJustParry_);
		ImGui::Checkbox("IsGoodParry", &isGoodParry_);

		if (ImGui::TreeNode("Collider")) {
			collider_.DebugGUI();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void Parry::Command() {
	if (eventOrder_.GetIsEnd()) {
		isActive_ = true;
		collider_.isActive = true;
	}
	eventOrder_.Start();
}

void Parry::CreateCollision() {
	// 攻撃判定生成
	aabb_.min.y = 0.0f;
	aabb_.max.y = 1.0f;
	collider_.SetFollowTarget(player_->GetWorldTF());
	collider_.isActive = false;
	collider_.mask.SetBelongFrag(ColMask0);
	collider_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		// 衝突した相手が同じマスクなら処理しない
		if (hitTarget->mask.GetBelongFrag() == collider_.mask.GetBelongFrag()) { return; }
		// すでにジャスパor甘パリィなら処理しない
		if (isGoodParry_ || isJustParry_) { return; }

		// ジャストパリィ
		if (eventOrder_.GetCurrentTimeEvent().name == "JustParry") {
			collider_.isActive = true;
			isJustParry_ = true;
			isGoodParry_ = false;
		}
		// 甘めパリィ
		else if (eventOrder_.GetCurrentTimeEvent().name == "GoodParry") {
			collider_.isActive = true;
			isGoodParry_ = true;
			isJustParry_ = false;
		}
		};
}

void Parry::CheckParryState() {
	// 予備動作
	if (eventOrder_.GetCurrentTimeEvent().name == "SwingTime") {
		collider_.isActive = false;
		isJustParry_ = false;
		isGoodParry_ = false;
	}
	// 硬直
	else if (eventOrder_.GetCurrentTimeEvent().name == "RecoveryTime") {
		collider_.isActive = false;
		isJustParry_ = false;
		isGoodParry_ = false;
	}
}