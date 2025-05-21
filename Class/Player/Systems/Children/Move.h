#pragma once
#include "../ISystem.h"
#include "State/IMoveSystemState.h"

/// <summary>
/// 自機の移動機能をまとめたクラス
/// </summary>
class Move : public ISystem {
public:
enum class MoveState {
	kNone,			// 何もなし
	kIdle,			// 待機
	kWalk,			// 歩き
	kRun,			// 小走り
	kDash,			// 走り
	kAttackRecovery,// 攻撃後に何も移動キーを入力していない場合
	kCount
};

public:
	// コンストラクタ
	Move(LWP::Object::Camera* camera, Player* player);
	// デストラクタ
	~Move() override;

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
	/// ImGuiによるパラメータ表示
	/// </summary>
	void DebugGUI() override;

	/// <summary>
	/// jsonファイルの作成
	/// </summary>
	void CreateJsonFIle() override;

	/// <summary>
	/// 移動コマンド
	/// </summary>
	void Command();

	/// <summary>
	/// アニメーションのコマンド
	/// </summary>
	void AnimCommand();

	/// <summary>
	/// 移動の状態を更新
	/// </summary>
	void CheckMoveState();

private:
	/// <summary>
	/// 入力処理
	/// </summary>
	void InputUpdate();

	/// <summary>
	/// 状態の遷移
	/// </summary>
	/// <param name="pState">次の状態</param>
	void ChangeState(IMoveSystemState* pState);

	/// <summary>
	/// クォータニオンのy軸のみ取り出す
	/// </summary>
	/// <param name="q"></param>
	/// <returns></returns>
	float GetYawFromQuaternion(const LWP::Math::Quaternion& q) {
		// Yaw（Y軸まわりの回転）を取り出す
		float siny_cosp = 2.0f * (q.w * q.y + q.z * q.x);
		float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
		return std::atan2(siny_cosp, cosy_cosp); // ラジアン
	}

	// 絶対値に変換
	LWP::Math::Vector3 Abs(LWP::Math::Vector3 value) {
		LWP::Math::Vector3 result{
			std::fabsf(value.x),
			std::fabsf(value.y),
			std::fabsf(value.z)
		};
		return result;
	}

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// 移動対象のモデルのアドレスを取得
	/// </summary>
	/// <returns></returns>
	LWP::Resource::RigidModel GetModel() { return *model_; }

	/// <summary>
	/// 移動速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetMoveVel() { return moveVel_; }
	/// <summary>
	/// 向いている方向を取得(クォータニオン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetMoveQuat() { return quat_; }
	/// <summary>
	/// 向いている方向を取得(ラジアン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetMoveRadian() { return radian_; }
	/// <summary>
	/// 移動状態を取得
	/// </summary>
	/// <returns></returns>
	IMoveSystemState* GetMoveState() { return state_; }
	/// <summary>
	/// スティックの倒し具合を取得
	/// </summary>
	/// <returns></returns>
	float GetStickStrength() { return stickStrength_; }
	/// <summary>
	/// 移動しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsMove() { return isMove_; }
	/// <summary>
	/// 入力処理を受けつけるかを取得
	/// </summary>
	/// <returns></returns>
	bool GetEnableInput() { return enableInput_; }
	/// <summary>
	/// 指定した移動状態に変更された瞬間かを取得
	/// </summary>
	/// <param name="moveState"></param>
	/// <returns></returns>
	bool GetTriggerChangeMoveState(MoveState moveState){
		if (preMoveState_ != moveState /*&& preMoveState_ != moveState*/) {
			return true;
		}
		return false;
	}
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 移動対象のモデルのアドレスを設定
	/// </summary>
	/// <param name="model">モデルのアドレス</param>
	void SetModel(LWP::Resource::RigidModel* model) { model_ = model; }

	/// <summary>
	/// 移動速度を設定
	/// </summary>
	/// <param name="moveVel">移動速度</param>
	void SetMoveVel(const LWP::Math::Vector3& moveVel) { moveVel_ = moveVel; }
	/// <summary>
	/// 向いている方向を設定
	/// </summary>
	/// <param name="radian">向かせる方向(ラジアン)</param>
	void SetRotate(const LWP::Math::Vector3& radian) { radian_ = radian; }
	/// <summary>
	/// 向いている方向を設定
	/// </summary>
	/// <param name="quat">向かせる方向(クォータニオン)</param>
	void SetRotate(const LWP::Math::Quaternion& quat) { quat_ = quat; }
	/// <summary>
	/// 移動速度の倍率を設定
	/// </summary>
	/// <param name="moveMultiply"></param>
	void SetMoveMultiply(const float& moveMultiply) { moveMultiply_ = moveMultiply; }
	/// <summary>
	/// 入力処理を受けつけるかを設定
	/// </summary>
	/// <returns></returns>
	void SetEnableInput(const bool& enableInput) { enableInput_ = enableInput; }
#pragma endregion

private:// jsonで保存する値
	// 歩き時の速度倍率
	float walkSpeedMultiply = 1.0f;
	// 小走りの速度倍率
	float runSpeedMultiply = 1.0f;
	// 走り時の速度倍率
	float dashSpeedMultiply = 1.0f;
	// 移動の補間レート
	float moveSpeedRate = 0.2f;

	// 小走り状態に移行するのに必要なスティックの倒し具合(0.0f~1.0f)
	float runThreshold = 0.5f;


private:// プライベートな変数
	// 移動対象のモデルのアドレス
	LWP::Resource::RigidModel* model_;

	IMoveSystemState* state_;

	// 移動速度
	LWP::Math::Vector3 moveVel_;

	// 向いている角度
	LWP::Math::Quaternion quat_ = { 0.0f,0.0f,0.0f,1.0f };
	LWP::Math::Vector3 radian_;

	// 移動時のイージング
	LWP::Math::Vector3 moveOffset_;

	MoveState moveState_;
	MoveState preMoveState_;

	// スティックの倒し具合
	float stickStrength_;

	// 移動速度の倍率
	float moveMultiply_;

	// 移動しているか
	bool isMove_;

	bool enableInput_;
};