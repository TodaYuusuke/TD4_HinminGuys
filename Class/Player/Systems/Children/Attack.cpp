#include "Attack.h"
#include "../../Player.h"
#include "State/Attack/DefaultAttack.h"
#include "State/Attack/LockOnAttack.h"
#include "State/Attack/NoneAttack.h"
#include "../../../GameMask.h"
#include "../../../Enemy/EnemyManager.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Object;
using namespace LWP::Object::Collider;
using namespace GameMask;

Attack::Attack(LWP::Object::Camera* camera, Player* player, EnemyManager* enemyManager)
{
	pCamera_ = camera;
	player_ = player;
	enemyManager_ = enemyManager;

	// 状態作成
	state_ = new NoneAttack(this);
	state_->Initialize();

	// コンボツリーの初期化
	comboTree_.Init("Combo.json", player_->GetModel(), player_->GetAnimation());
	// コライダーのマスク設定
	comboTree_.SetColliderMaskFrag(GetAttack(), GetEnemy());
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
	// 攻撃入力可能状態
	if (!IsBitSame(inputHandler_->GetBanInput(), BanAttack, GetSetBitPosition(BanAttack))) {
		comboTree_.SetIsRecept(true);
	}
	else {
		comboTree_.SetIsRecept(false);
	}

	// コンボツリー自体は毎フレーム更新する
	comboTree_.Update();

	// コンボが無操作状態のコンボでない場合
	if (comboTree_.GetIsStiffness()) {
		// 攻撃しているものとみなし、攻撃状態に移行
		if (!isActive_) {
			player_->GetSystemManager()->SetInputState(InputState::kAttack);
			isActive_ = true;
			// 一番近い敵に向かって攻撃できるようにする
			// 有効距離は3M
			if (!enemyManager_->GetEnemyListPtr()->empty()) {
				attackAssistTarget_ = nullptr;
				if ((enemyManager_->GetEnemyListPtr()->front()->GetWorldTF()->GetWorldPosition() - player_->GetWorldTF()->GetWorldPosition()).Length() <= std::powf(attackAsisstRange, 2.0f)) {
					// 自機の方向ベクトル
					Vector3 playerDir = { 0.0f,0.0f,1.0f };
					// 回転行列を求める
					Matrix4x4 rotMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(player_->GetQuat());
					// 方向ベクトルを求める
					playerDir = playerDir * rotMatrix;
					playerDir.y = 0;
					// 自機の背後にいるなら攻撃位置アシストをしない
					if (!IsObjectInOppositeDirection(enemyManager_->GetEnemyListPtr()->front()->GetWorldTF()->GetWorldPosition(), player_->GetWorldTF()->GetWorldPosition(), playerDir)) {
						attackAssistTarget_ = enemyManager_->GetEnemyListPtr()->front();
					}
				}
			}
		}
	}
	if (!comboTree_.GetIsStiffness()) { // 無操作状態のコンボが選択されている場合
		// 機能停止させる
		if (isActive_) {
			Reset();
			isAttackRecovery_ = true;
		}
	}
	// 受付時間が終了していればコンボ中断
	if (comboTree_.GetIsReceptEndTrigger() && !comboTree_.GetIsStiffness()) {
		isAttackRecovery_ = false;
	}

	// 攻撃アシストが有効になっている場合
	if (comboTree_.GetIsEnableAttackAssist() && !comboTree_.GetIsThisRoot()) {
		// ロックオン中なら対象に近づいて攻撃
		if (lockOnSystem_->GetCurrentLockOnTarget()) {
			// 5M以内なら攻撃位置アシスト
			if ((enemyManager_->GetEnemyListPtr()->front()->GetWorldTF()->GetWorldPosition() - player_->GetWorldTF()->GetWorldPosition()).Length() <= std::powf(lockOnAsisstRange, 2.0f)) {
				LockOnAssist(lockOnSystem_->GetCurrentLockOnTarget());
			}
			// アシストなし
			else {
				AttackAssist();
			}
		}
		// ロックオンなし時の攻撃位置アシスト
		else {
			if (attackAssistTarget_) {
				if (attackAssistTarget_->GetIsDead()) {
					attackAssistTarget_ = nullptr;
				}
				// 攻撃位置アシスト
				LockOnAssist(attackAssistTarget_);
			}
			// アシストなし
			else {
				AttackAssist();
			}
		}
	}
	else {
		// アシストの移動ベクトルリセット
		attackAssistVel_ = { 0.0f,0.0f,0.0f };
	}
}

void Attack::Reset() {
	isActive_ = false;
	isAttackRecovery_ = false;
	attackAssistVel_ = { 0.0f,0.0f,0.0f };
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

		ImGui::DragFloat3("Velocity", &attackAssistVel_.x, 0.1f, -10000, 10000);
		ImGui::DragFloat3("Rotation", &attackAssistRadian_.x, 0.1f, -6.28f, 6.28f);
		ImGui::DragFloat4("Quaternion", &attackAssistQuat_.x, 0.1f, -1, 1);

		ImGui::TreePop();
	}
}

void Attack::CreateJsonFIle() {
}

void Attack::Command() {
	//collider_.isActive = true;
	//isEnableInput_ = true;
}

void Attack::ChangeState(IAttackSystemState* pState) {
	delete state_;
	state_ = pState;
}

void Attack::CreateEventOrder() {
	eventOrder_.Initialize();
}

void Attack::CheckAttackState() {

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

void Attack::LockOnAssist(IEnemy* lockOnTarget) {
	if (!lockOnTarget) { return; }

	// 自機とロックオン中の敵との距離
	Vector3 attackTargetDist = (lockOnTarget->GetWorldTF()->GetWorldPosition() - player_->GetWorldTF()->GetWorldPosition());
	Vector3 aa = lockOnTarget->GetWorldTF()->GetWorldPosition() + (-0.01f * attackTargetDist.Normalize()) - player_->GetWorldTF()->GetWorldPosition();

	attackAssistVel_ = LWP::Utility::Interpolation::Lerp(Vector3{ 0,0,0 }, aa, 0.25f);

	// 移動速度からラジアンを求める
	attackAssistRadian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, attackAssistVel_.Normalize(), LWP::Math::Vector3{ 0,1,0 });
	attackAssistQuat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, attackAssistVel_.y);
}

void Attack::AttackAssist() {
	// 攻撃の移動量の取得
	attackAssistVel_ = comboTree_.GetAttackAssistMoveAmount();

	// 自機の方向ベクトル
	Vector3 playerDir = { 0.0f,0.0f,1.0f };
	// 回転行列を求める
	Matrix4x4 rotMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(player_->GetSystemManager()->GetMoveSystem()->GetMoveQuat());
	// 方向ベクトルを求める
	playerDir = playerDir * rotMatrix;
	playerDir.y = 0;

	attackAssistVel_ = attackAssistVel_ * rotMatrix;

	// 移動速度からラジアンを求める
	attackAssistRadian_.y = LWP::Utility::GetRadian(LWP::Math::Vector3{ 0,0,1 }, playerDir.Normalize(), LWP::Math::Vector3{ 0,1,0 });
	attackAssistQuat_ = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, attackAssistRadian_.y);
}