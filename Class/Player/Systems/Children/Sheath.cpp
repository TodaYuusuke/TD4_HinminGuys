#include "Sheath.h"
#include "State/Sheath/Throw.h"
#include "../../Player.h"

Sheath::Sheath(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;

	sheathModel_.LoadCube();
	sheathModel_.worldTF.scale = { 0.25f, 1.0f, 0.25f };
	sheathModel_.isActive = true;

	nextState_ = InputALL;
	currentState_ = InputSheath;
}

void Sheath::Initialize() {
	// コマンドの登録
	inputHandler_ = InputHandler::GetInstance();

	// jsonで保存している値
	CreateJsonFIle();

	// アクションイベント作成
	CreateThrowEventOrder();
	CreateCollectEventOrder();

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

	isPreActive_ = isActive_;
}

void Sheath::Reset() {
	isActive_ = false;
	isPreActive_ = false;
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

		// 移動可能範囲
		.AddValue<float>("MoveRange", &enableMoveRange)
		// クールタイム
		.AddValue<float>("CoolTime", &coolTime)

		.CheckJsonFile();
}

void Sheath::Command() {
	// 状態によって変更
	state_->Command();
}

void Sheath::AnimCommand() {
	// 状態によって変更
	state_->AnimCommand();
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
