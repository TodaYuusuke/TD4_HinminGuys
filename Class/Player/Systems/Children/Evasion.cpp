#include "Evasion.h"
#include "../../Player.h"

using namespace LWP;
using namespace LWP::Math;

Evasion::Evasion(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;
}

void Evasion::Initialize() {
	isActive_ = false;
	isPreActive_ = false;

	// アクションイベントを生成
	CreateEventOrder();

	json_.Init("EvasionData.json");
	json_.BeginGroup("EventOrder")
		.BeginGroup("GraceTime")
		.AddValue<float>("SwingTime", &kEvasionSwingTime)
		.AddValue<float>("InvinsibleTime", &kInvinsibleTime)
		.AddValue<float>("RecoveryTime", &kEvasionRecoveryTime)
		.EndGroup()
		.EndGroup()
		.AddValue<float>("EvasionMultiply", &moveMultiply)
		.CheckJsonFile();
}

void Evasion::Update() {
	if (!isActive_) { return; }

	// frameごとに起きるアクションイベント
	eventOrder_.Update();

	// 移動処理
	Move();

	// アクションイベントの確認
	CheckEvasionState();

	// 全てのアクションイベントが終了しているなら機能停止
	if (eventOrder_.GetIsEnd()) {
		Reset();
	}

	isPreActive_ = isActive_;
}

void Evasion::Reset() {
	eventOrder_.Reset();
	isActive_ = false;
	isPreActive_ = false;
	// 回避時の速度
	velocity_ = { 0.0f, 0.0f, 0.0f };
	// 回避時の角度(ラジアン)
	radian_ = { 0.0f, 0.0f, 0.0f };
	// アニメーションを初期化
	player_->ResetAnimation();
}

void Evasion::DebugGUI() {
	if (ImGui::TreeNode("Evasion")) {
		// 回避のアクションイベントを保存
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

		ImGui::Checkbox("IsEvasion", &isActive_);

		ImGui::TreePop();
	}
}

void Evasion::Command() {
	if (eventOrder_.GetIsEnd()) {
		isActive_ = true;
		player_->ResetAnimation();
		player_->StartAnimation("Dash", 1.0f, 0.0f);
		player_->SetIsLoopAnimation(true);
	}
	eventOrder_.Start();
}

void Evasion::CreateEventOrder() {
	eventOrder_.Initialize();
	// 回避発生までの時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kEvasionSwingTime * 60.0f, "EvasionSwingTime" });
	// 回避の無敵猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kInvinsibleTime * 60.0f, "InvinsibleTime" });
	// 回避の硬直時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kEvasionRecoveryTime * 60.0f, "EvasionRecoveryTime" });
}

void Evasion::CheckEvasionState() {
	// 予備動作
	if (eventOrder_.GetCurrentTimeEvent().name == "EvasionSwingTime") {
		//collider_.isActive = false;
		//isJustParry_ = false;
		//isGoodParry_ = false;
	}
	// 無敵時間
	else if (eventOrder_.GetCurrentTimeEvent().name == "InvinsibleTime") {
		//collider_.isActive = false;
		//isJustParry_ = false;
		//isGoodParry_ = false;
	}
	// 硬直時間
	else if (eventOrder_.GetCurrentTimeEvent().name == "EvasionRecoveryTime") {
		//collider_.isActive = false;
		//isJustParry_ = false;
		//isGoodParry_ = false;
	}
}

void Evasion::Move() {
	// カメラが向いている方向に進む
	// 回転行列を求める
	Matrix4x4 rotMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(player_->GetSystemManager()->GetMoveSystem()->GetMoveQuat());
	// 方向ベクトルを求める
	velocity_ = Vector3{ 0.0f,0.0f,1.0f } * rotMatrix * moveMultiply;
	velocity_.y = 0;

	// 移動ベクトルから体の向きを算出(入力があるときのみ処理する)
	// 移動速度からラジアンを求める
	radian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, velocity_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
}