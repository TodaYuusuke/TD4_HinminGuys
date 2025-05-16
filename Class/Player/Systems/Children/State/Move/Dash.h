#pragma once
#include "../IMoveSystemState.h"

class Dash : public IMoveSystemState {
public:
	// コンストラクタ
	Dash(Move* moveSystem, Player* player, const float& moveMultiply);
	// デストラクタ
	~Dash() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// アニメーションのコマンド
	/// </summary>
	void AnimCommand() override;

private:

};