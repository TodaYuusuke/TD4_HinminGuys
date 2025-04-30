#pragma once
#include "../Adapter/Adapter.h"

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

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// 機能が使える状態かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsActive() { return isActive_; }
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
#pragma endregion

protected:
	// 自機のアドレス
	Player* player_;

	// シーンで使用しているカメラのポインタ
	LWP::Object::Camera* pCamera_;

	// 機能が使える状態か
	bool isActive_ = true;
};