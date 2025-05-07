#pragma once
#include "../ISystem.h"
#include "../EventOrder.h"

class Evasion : public ISystem {
public:
	// コンストラクタ
	Evasion(LWP::Object::Camera* camera, Player* player);
	// デストラクタ
	~Evasion() override = default;

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
	/// 回避のコマンド
	/// </summary>
	void Command();

private:// プライベートな変数
	// 回避時の速度
	Vector3 velocity_;
	// 回避時の角度(ラジアン)
	Vector3 radian_;
};