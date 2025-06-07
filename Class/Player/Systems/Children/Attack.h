#pragma once
#include "../ISystem.h"
#include "LockOn.h"
#include "State/IAttackSystemState.h"
#include "../../../ComboEditor/ComboTree.h"
#include "../../../GameMask.h"

struct AttackJsonData {
	// ロックオン時の攻撃位置アシストを行う範囲
	float lockOnAsisstRange = 5.0f;
	// 非ロックオン時の攻撃位置アシストを行う範囲
	float attackAsisstRange = 3.0f;
};

class IEnemy;
class EnemyManager;
/// <summary>
/// 自機の攻撃機能をまとめたクラス
/// </summary>
class Attack : public ISystem {
public:
	// コンストラクタ
	Attack(LWP::Object::Camera* camera, Player* player, EnemyManager* enemyManager);
	// デストラクタ
	~Attack() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 全ての数値をリセット
	/// </summary>
	void Reset() override;

	/// <summary>
	/// ImGuiによるパラメータを表示
	/// </summary>
	void DebugGUI() override;

	/// <summary>
	/// jsonファイルの作成
	/// </summary>
	void CreateJsonFIle() override;

	/// <summary>
	/// ImGuiによるコンボのGUI表示
	/// </summary>
	//void DebugComboGUI() { comboTree_->DebugGUI(); }

	/// <summary>
	/// 攻撃のコマンド
	/// </summary>
	void Command();

	/// <summary>
	/// コンボの作成
	/// </summary>
	void CreateCombo();

private:
	/// <summary>
	/// 状態の遷移
	/// </summary>
	/// <param name="pState">次の状態</param>
	void ChangeState(IAttackSystemState* pState);

	/// <summary>
	/// アクションイベントの生成
	/// </summary>
	void CreateEventOrder();

	/// <summary>
	/// 攻撃のイベント状態の確認とそれに伴った処理
	/// </summary>
	void CheckAttackState();

	/// <summary>
	/// 攻撃時の位置アシスト
	/// </summary>
	void AttackAssistMovement();

	/// <summary>
	/// ロックオン時の位置アシスト
	/// </summary>
	void LockOnAssist(IEnemy* lockOnTarget);

	/// <summary>
	/// 攻撃時の位置アシスト(非ロックオン時)
	/// </summary>
	void AttackAssist();

private:
	bool IsObjectInOppositeDirection(const Vector3& objectPosition, const Vector3& cameraPosition, const Vector3& cameraDirection) {
		Vector3 cameraToEnemyDirection = (cameraPosition - objectPosition).Normalize();
		float dot = Vector3::Dot((cameraDirection).Normalize(), cameraToEnemyDirection);

		// カメラの正面方向にいる
		if (dot < 0.0f) {
			return false;
		}

		// カメラの背後にいる
		return true;
	}

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// jsonに保存する値を取得
	/// </summary>
	AttackJsonData GetJsonData() { return jsonData_; }

	//ComboTree* GetComboTreeData() { return comboTree_; }
	/// <summary>
	/// 編集モード中かどうかのゲッター
	/// </summary>
	/// <returns></returns>
	//bool GetIsEditingMode() { return comboTree_->GetIsEditingMode(); }
	/// <summary>
	/// 現在のコンボが大元のコンボであるかどうかを取得
	/// </summary>
	/// <returns></returns>
	//bool GetIsThisRoot() { return comboTree_->GetIsThisRoot(); }
	/// <summary>
	/// 攻撃中に入力処理を受け付けていないかを取得
	/// </summary>
	/// <returns></returns>
	//bool GetIsStiffness() { return comboTree_->GetIsStiffness(); }
	/// <summary>
	/// コンボの受付状態を習得
	/// </summary>
	/// <returns>コンボの受付状態を取得</returns>
	//bool GetIsRecept() { return comboTree_->GetIsRecept(); }
	/// <summary>
	/// 攻撃が全て終わった後か
	/// </summary>
	/// <returns></returns>
	bool GetIsAttackRecovery() { return isAttackRecovery_; }
	/// <summary>
	/// ダメージ取得
	/// </summary>
	/// <returns></returns>
	//float GetDamage() { return comboTree_->GetDamage(); }
	/// <summary>
	/// ノックバック量取得
	/// </summary>
	/// <returns></returns>
	//float GetKnockBackStrength() { return comboTree_->GetNockBackStrength(); }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// ロックオン機能のアドレスを設定
	/// </summary>
	/// <param name="lockOnSystem"></param>
	void SetLockOnSystem(LockOn* lockOnSystem) { lockOnSystem_ = lockOnSystem; }
	/// <summary>
	/// jsonに保存する値を設定
	/// </summary>
	/// <param name="jsonData"></param>
	void SetJsonData(const AttackJsonData& jsonData) { jsonData_ = jsonData; }
	//void SetComboTreeData(ComboTree* comboTree) { comboTree_ = comboTree; }
	/// <summary>
	/// 攻撃が全て終わった後かを設定
	/// </summary>
	/// <returns></returns>
	void SetIsAttackRecovery(const bool& isAttackRecovery) { isAttackRecovery_ = isAttackRecovery; }
#pragma endregion

private:// jsonで保存する値
	AttackJsonData jsonData_;

private:// 外部からポインタをもらう変数
	// 敵の管理クラス
	EnemyManager* enemyManager_;
	// ロックオン機能
	LockOn* lockOnSystem_;

private:
	// コンボ攻撃用クラス
	//ComboTree* comboTree_;

	// 当たり判定の内容
	//LWP::Object::Collision::OnHitFunction onCollision_;

	// 状態遷移
	IAttackSystemState* state_;

	IEnemy* lockOnTarget_;
	// 非ロックオン時のアシスト攻撃対象
	IEnemy* attackAssistTarget_;

	// 攻撃が全て終わった後か
	bool isAttackRecovery_;
};