#pragma once
#include "../ISystem.h"
#include "LockOn.h"
#include "State/IAttackSystemState.h"
#include "../../../ComboEditor/ComboTree.h"

class IEnemy;
/// <summary>
/// 自機の攻撃機能をまとめたクラス
/// </summary>
class Attack : public ISystem {
public:
	// コンストラクタ
	Attack(LWP::Object::Camera* camera, Player* player);
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
	void DebugComboGUI() { comboTree_.DebugGUI(); }

	/// <summary>
	/// 攻撃のコマンド
	/// </summary>
	void Command();

	/// <summary>
	/// コンボのリセット
	/// </summary>
	void ComboReset() { comboTree_.ResetCombo(); }

private:
	/// <summary>
	/// 状態の遷移
	/// </summary>
	/// <param name="pState">次の状態</param>
	void ChangeState(IAttackSystemState* pState);

	/// <summary>
	/// 当たり判定を作成
	/// </summary>
	void CreateCollision();

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

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// 攻撃時の位置アシスト用のベクトルを取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetAttackAssistVel() { return attackAssistVel_; }
	/// <summary>
	/// 攻撃時の位置アシスト用のベクトルからクォータニオンを取得
	/// </summary>
	/// <returns></returns>
	Quaternion GetAttackAssistQuat() { return attackAssistQuat_; }
	/// <summary>
	/// 攻撃時の位置アシスト用のベクトルからラジアンを取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetAttackAssistRadian() { return attackAssistRadian_; }
	/// <summary>
	/// 編集モード中かどうかのゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsEditingMode() { return comboTree_.GetIsEditingMode(); }
	/// <summary>
	/// 現在のコンボが大元のコンボであるかどうかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsThisRoot() { return comboTree_.GetIsThisRoot(); }
	/// <summary>
	/// 攻撃中に入力処理を受け付けていないかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsStiffness() { return comboTree_.GetIsStiffness(); }
	/// <summary>
	/// コンボの受付状態を習得
	/// </summary>
	/// <returns>コンボの受付状態を取得</returns>
	bool GetIsRecept() { return comboTree_.GetIsRecept(); }
	/// <summary>
	/// 攻撃が全て終わった後か
	/// </summary>
	/// <returns></returns>
	bool GetIsAttackRecovery() { return isAttackRecovery_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// ロックオン機能のアドレスを設定
	/// </summary>
	/// <param name="lockOnSystem"></param>
	void SetLockOnSystem(LockOn* lockOnSystem) { lockOnSystem_ = lockOnSystem; }
	/// <summary>
	/// 攻撃時の位置アシスト用のベクトルを設定
	/// </summary>
	/// <returns></returns>
	void SetAttackAssistVel(Vector3 attackAssistVel) {  attackAssistVel_ = attackAssistVel; }
	/// <summary>
	/// 攻撃時の位置アシスト用のベクトルからクォータニオンを設定
	/// </summary>
	/// <returns></returns>
	void SetAttackAssistQuat(Quaternion attackAssistQuat) {  attackAssistQuat_ = attackAssistQuat; }
	/// <summary>
	/// 攻撃時の位置アシスト用のベクトルからラジアンを設定
	/// </summary>
	/// <returns></returns>
	void SetAttackAssistRadian(Vector3 attackAssistRadian) {  attackAssistRadian_ = attackAssistRadian; }
	/// <summary>
	/// 攻撃が全て終わった後かを設定
	/// </summary>
	/// <returns></returns>
	void SetIsAttackRecovery(const bool& isAttackRecovery) { isAttackRecovery_ = isAttackRecovery; }
#pragma endregion

private:// jsonで保存する値
	// 通常攻撃発動までにかかる時間[秒]
	static float kNormalSwingTime;
	// 通常攻撃の猶予時間[秒]
	static float kNormalAttackTime;
	// 通常攻撃の硬直[秒]
	static float kNormalRecoveryTime;

private:// 外部からポインタをもらう変数
	// ロックオン機能
	LockOn* lockOnSystem_;

private:

	// コンボ攻撃用クラス
	ComboTree comboTree_;

	// 状態遷移
	IAttackSystemState* state_;

	// 攻撃判定
	LWP::Object::Collision collider_;
	LWP::Object::Collider::AABB& aabb_;

	// 攻撃時の位置アシスト用のベクトル
	Vector3 attackAssistVel_;
	Vector3 attackAssistRadian_;
	Quaternion attackAssistQuat_;
	IEnemy* lockOnTarget_;

	// 攻撃が全て終わった後か
	bool isAttackRecovery_;
};