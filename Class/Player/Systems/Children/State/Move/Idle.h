#pragma once
#include "../IMoveSystemState.h"

class Idle : public IMoveSystemState {
public:
	// コンストラクタ
	Idle(Move* moveSystem, Player* player);
	// デストラクタ
	~Idle() override = default;

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