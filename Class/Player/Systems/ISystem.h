#pragma once
#include "../Adapter/Adapter.h"
#include "EventOrder.h"
#include "../Command/InputHandler.h"

// 行動の状態
enum class SystemState {
	kMove,
	kEvasion,
	kParry,
	kSheath,
	kAttack,
	kDamage,
	kAll
};

class Player;
/// <summary>
/// 自機の機能クラスの基底クラス
/// </summary>
class ISystem {
public:
	// コンストラクタ
	ISystem() = default;
	// デストラクタ
	virtual ~ISystem() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 全ての数値をリセット
	/// </summary>
	virtual void Reset() = 0;

	/// <summary>
	/// 
	/// </summary>
	virtual void DebugGUI() = 0;

	/// <summary>
	/// jsonファイルの作成
	/// </summary>
	virtual void CreateJsonFIle() = 0;

private:
	/// <summary>
	/// 状態の遷移
	/// </summary>
	/// <param name="pState">次の状態</param>
	void ChangeState(ISystem* pState);

public:// Getter, Setter
	/// <summary>
	/// 入力があったかを取得
	/// </summary>
	std::map<SystemState, bool> CheckNextSystems() {
		std::map<SystemState, bool> nextSystem;
		// 攻撃
		if (InputHandler::GetInstance()->GetPressAttack()) {
			nextSystem[SystemState::kAttack] = true;
		}
		// 移動
		if (InputHandler::GetInstance()->GetPressMove()) {
			nextSystem[SystemState::kMove] = true;
		}
		// 回避
		if (InputHandler::GetInstance()->GetPressEvasion()) {
			nextSystem[SystemState::kEvasion] = true;
		}
		// パリィ
		if (InputHandler::GetInstance()->GetPressParry()) {
			nextSystem[SystemState::kParry] = true;
		}
		// 鞘
		if (InputHandler::GetInstance()->GetPressSheath()) {
			nextSystem[SystemState::kSheath] = true;
		}

		return nextSystem;
	}

	/// <summary>
	/// 次に遷移したいシステムリストクリア
	/// </summary>
	void ClearNextSystems() { nextSystem_.clear(); }

#pragma region Getter
	/// <summary>
	/// 次に遷移したいシステムを取得
	/// </summary>
	std::map<SystemState, bool> GetNextSystems() { return nextSystem_; }
	bool GetNextSystem(SystemState systemState) {
		if (nextSystem_[systemState]) {
			return true;
		}

		return false;
	}

	/// <summary>
	/// 移動速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetVelocity() { return velocity_; }
	/// <summary>
	/// 向いている方向を取得(クォータニオン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetQuat() { return quat_; }
	/// <summary>
	/// 向いている方向を取得(ラジアン)
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetRadian() { return radian_; }

	/// <summary>
	/// 機能が使える状態かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsActive() { return isActive_; }

	/// <summary>
	/// 機能を使用できるようになった瞬間を取得
	/// </summary>
	/// <returns></returns>
	bool GetTrigger() {
		if (!isPreActive_ && isActive_) { return true; }
		return false;
	}
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 次に遷移したいシステムリストを設定
	/// </summary>
	void SetNextSystems(std::map<SystemState, bool> nextSystems) { nextSystem_ = nextSystems; }
	/// <summary>
	/// 指定したシステムが次に遷移したい
	/// </summary>
	void SetNextSystem(SystemState systemState) { nextSystem_[systemState] = true; }
	/// <summary>
	/// jsonの情報を設定
	/// </summary>
	/// <returns></returns>
	void SetJsonData(LWP::Utility::JsonIO json) { json_ = json; }
	/// <summary>
	/// シーンで使用しているカメラのポインタ
	/// </summary>
	/// <param name="camera">カメラのアドレス</param>
	void SetCamera(LWP::Object::Camera* camera) { pCamera_ = camera; }
	/// <summary>
	/// 自機のポインタを設定
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }
	/// <summary>
	/// 移動速度を設定
	/// </summary>
	/// <param name="velocity">移動速度</param>
	void SetVelocity(const LWP::Math::Vector3& velocity) { velocity_ = velocity; }
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
	/// 機能が使える状態かを設定
	/// </summary>
	void SetIsActive(bool isActive) { isActive_ = isActive; }
#pragma endregion

protected:// 外部から受け取る変数
	// 自機のアドレス
	Player* player_;

	//　キー入力
	InputHandler* inputHandler_;

	// シーンで使用しているカメラのポインタ
	LWP::Object::Camera* pCamera_;

protected:
	// フレーム単位で発生するアクションイベントを管理するクラス
	EventOrder eventOrder_;

	// jsonによるパラメータの保存、読み込み
	LWP::Utility::JsonIO json_;

	// 次に遷移したいシステム
	std::map<SystemState, bool> nextSystem_;

	// 移動速度
	LWP::Math::Vector3 velocity_;
	// 向いている角度
	LWP::Math::Quaternion quat_ = { 0.0f,0.0f,0.0f,1.0f };
	LWP::Math::Vector3 radian_;

	// 機能が使える状態か
	bool isActive_ = true;
	bool isPreActive_ = false;
};