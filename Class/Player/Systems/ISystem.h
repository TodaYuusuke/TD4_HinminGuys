#pragma once
#include "../Adapter/Adapter.h"
#include "EventOrder.h"
#include "../Command/InputHandler.h"

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
#pragma region Getter
	int GetCurrentState() { return currentState_; }
	/// <summary>
	/// 次に遷移できるシステムを取得
	/// </summary>
	/// <returns></returns>
	int GetNextState() { return nextState_; }
	/// <summary>
	/// 機能が使える状態かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsActive() { return isActive_; }
	/// <summary>
	/// 移動入力を受け付ける状態かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsMoveInput() { return isMoveInput_; }

	/// <summary>
	/// 機能を使用できるようになった瞬間を取得
	/// </summary>
	/// <returns></returns>
	bool GetTrigger(){
		if (!isPreActive_ && isActive_) { return true; }
		return false;
	}
#pragma endregion

#pragma region Setter
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
	/// 次に遷移できるシステムを設定
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextState(const int& nextState) { nextState_ = nextState; }
	/// <summary>
	/// 機能が使える状態かを設定
	/// </summary>
	void SetIsActive(bool isActive) { isActive_ = isActive; }
	/// <summary>
	/// 移動入力を受け付ける状態かを設定
	/// </summary>
	void SetIsMoveInput(bool isMoveInput) { isMoveInput_ = isMoveInput; }
#pragma endregion

protected:
	// 自機のアドレス
	Player* player_;

	//　キー入力
	InputHandler* inputHandler_;

	// シーンで使用しているカメラのポインタ
	LWP::Object::Camera* pCamera_;

	// フレーム単位で発生するアクションイベントを管理するクラス
	EventOrder eventOrder_;


	int currentState_;
	// 次に遷移できるシステム(ビットで管理)
	int nextState_;

	// 機能が使える状態か
	bool isActive_ = false;
	bool isPreActive_ = false;
	// 移動入力を受け付ける状態か
	bool isMoveInput_ = true;

	// jsonによるパラメータの保存、読み込み
	LWP::Utility::JsonIO json_;
};