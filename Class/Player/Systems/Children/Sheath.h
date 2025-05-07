#pragma once
#include "../ISystem.h"

class Sheath : public ISystem{
public:
	// コンストラクタ
	Sheath(LWP::Object::Camera * camera, Player * player);
	// デストラクタ
	~Sheath() override = default;

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

private:// プライベートな変数

};