#include "Parry.h"
#include "../../Player.h"
#include "../Engine/object/core/collision/Collision.h"
#include "../../../GameMask.h"

using namespace LWP::Utility;
using namespace GameMask;

Parry::Parry(LWP::Object::Camera* camera, Player* player)
	: aabb_(collider_.SetBroadShape(LWP::Object::Collider::AABB()))
{
	pCamera_ = camera;
	player_ = player;

	// パリィ判定生成
	CreateCollision();

	json_.Init("ParryData.json");
	json_.BeginGroup("EventOrder")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &kSwingTime)
		.AddValue<float>("JustParry", &kJustParryTime)
		.AddValue<float>("GoodParry", &kGoodParryTime)
		.AddValue<float>("RecoveryTime", &kRecoveryTime)
		.EndGroup()
		.EndGroup()
		.CheckJsonFile();
}

void Parry::Initialize() {
	// コマンドの登録
	inputHandler_ = InputHandler::GetInstance();
	inputHandler_->GetA();
	isActive_ = false;
	isPreActive_ = false;

	// フレーム単位で発生するアクションイベントを管理するクラス
	CreateEventOrder();
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

	isPreActive_ = isActive_;
}

void Parry::Reset() {
	isActive_ = false;
	isJustParry_ = false;
	isGoodParry_ = false;
	collider_.isActive = false;
	aabb_.isShowWireFrame = false;
	// アニメーションを初期化
	player_->ResetAnimation();
}

void Parry::DebugGUI() {
	if (ImGui::TreeNode("Parry")) {
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

		if (ImGui::TreeNode("Collider")) {
			collider_.DebugGUI();
			ImGui::TreePop();
		}

		ImGui::Checkbox("IsJustParry", &isJustParry_);
		ImGui::Checkbox("IsGoodParry", &isGoodParry_);

		ImGui::TreePop();
	}
}

void Parry::Command() {
	if (eventOrder_.GetIsEnd()) {
		// パリィ状態に移行
		player_->GetSystemManager()->SetInputState(InputState::kParry);
		isActive_ = true;
		collider_.isActive = true;
		aabb_.isShowWireFrame = true;
		//isMoveInput_ = false;
	}
	eventOrder_.Start();
}

void Parry::AnimCommand() {
	// ガードアニメーション開始
	player_->ResetAnimation();
	player_->StartAnimation("Gaurd", 0.1f, 0.0f);
	//player_->StopAnimation();
}

void Parry::CreateCollision() {
	// 攻撃判定生成
	aabb_.min = { -1.0f, -1.0f, -1.0f };
	aabb_.max = { 1.0f, 1.0f, 1.0f };
	aabb_.isShowWireFrame = false;
	collider_.SetFollow(player_->GetWorldTF());
	collider_.isActive = false;
	collider_.worldTF.translation = { 0.0f, 1.0f, 0.0f };
	collider_.mask.SetBelongFrag(GetPlayer());
	collider_.mask.SetHitFrag(GetEnemy() | GetAttack());
	collider_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		// すでにジャスパor甘パリィなら処理しない
		if (isGoodParry_ || isJustParry_) { return; }

		// ジャストパリィ
		if (eventOrder_.GetCurrentTimeEvent().name == "JustParry") {
			isJustParry_ = true;
			isGoodParry_ = false;
			// ガードアニメーション開始
			player_->ResetAnimation();
			player_->StartAnimation("WeakParry", 1.0f, 0.0f);
		}
		// 甘めパリィ
		else if (eventOrder_.GetCurrentTimeEvent().name == "GoodParry") {
			isGoodParry_ = true;
			isJustParry_ = false;
			// ガードアニメーション開始
			player_->ResetAnimation();
			player_->StartAnimation("WeakParry", 1.0f, 0.0f);
		}
		};
}

void Parry::CreateEventOrder() {
	eventOrder_.Initialize();
	// パリィ発生までの時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kSwingTime * 60.0f, "SwingTime" });
	// ジャストパリィの猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kJustParryTime * 60.0f, "JustParry" });
	// 通常パリィの猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kGoodParryTime * 60.0f, "GoodParry" });
	// パリィの硬直時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kRecoveryTime * 60.0f, "RecoveryTime" });
}

void Parry::CheckParryState() {
	// 予備動作
	if (eventOrder_.GetCurrentTimeEvent().name == "SwingTime") {
		collider_.isActive = false;
		isJustParry_ = false;
		isGoodParry_ = false;
	}
	// ジャストパリィ
	else if (eventOrder_.GetCurrentTimeEvent().name == "JustParry") {
		collider_.isActive = true;
	}
	// 甘めパリィ
	else if (eventOrder_.GetCurrentTimeEvent().name == "GoodParry") {
		collider_.isActive = true;
	}
	// 硬直
	else if (eventOrder_.GetCurrentTimeEvent().name == "RecoveryTime") {
		collider_.isActive = false;
		isJustParry_ = false;
		isGoodParry_ = false;
	}
}