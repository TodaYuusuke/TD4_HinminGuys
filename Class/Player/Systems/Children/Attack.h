#pragma once
#include "../ISystem.h"
#include "../EventOrder.h"
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
	void DebugGUI();

	/// <summary>
	/// 通常攻撃のコマンド
	/// </summary>
	void NormalCommand();

	/// <summary>
	/// 状態の遷移
	/// </summary>
	/// <param name="pState">次の状態</param>
	void ChangeState(IAttackSystemState* pState);

private:
	/// <summary>
	/// 当たり判定を作成
	/// </summary>
	void CreateCollision();

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

private:// 定数
	// 通常攻撃発動までにかかる時間
	const float kNormalSwingTime = 60.0f * 0.25f;
	// 通常攻撃の猶予時間[frame * 秒]
	const float kNormalAttackTime = 60.0f * 0.6f;
	// 通常攻撃の硬直[frame * 秒]
	const float kNormalRecoveryTime = 60.0f * 0.2f;

private:// 外部からポインタをもらう変数
	// ロックオン機能
	LockOn* lockOnSystem_;

private:
	// 状態遷移
	IAttackSystemState* state_;

	// フレーム単位で発生するアクションイベントを管理するクラス
	EventOrder eventOrder_;

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