#pragma once
#include "../ISystem.h"

class LockOn : public ISystem {
public:
	// コンストラクタ
	LockOn(LWP::Object::Camera* camera);
	// デストラクタ
	~LockOn() override = default;

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

private:
	/// <summary>
	/// 入力処理
	/// </summary>
	void InputUpdate();

private:// 定数
	

private:
	// 経過時間
	float currentFrame_;

};