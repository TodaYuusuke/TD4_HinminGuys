#pragma once
#include "../ISystem.h"
#include "LockOn.h"
#include "State/IAttackSystemState.h"

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
	/// 通常攻撃のコマンド
	/// </summary>
	void NormalCommand();

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
	/// 攻撃時の位置アシスト用のベクトル
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
#pragma endregion

private:// jsonで保存する値
	// 通常攻撃発動までにかかる時間[秒]
	float kNormalSwingTime = 0.25f;
	// 通常攻撃の猶予時間[秒]
	float kNormalAttackTime = 0.6f;
	// 通常攻撃の硬直[秒]
	float kNormalRecoveryTime = 0.2f;

private:// 外部からポインタをもらう変数
	// ロックオン機能
	LockOn* lockOnSystem_;

private:
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

	// 通常攻撃の判定が出ているか
	bool isNormalAttack_;
};