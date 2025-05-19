#pragma once
#include "../IMoveSystemState.h"

class None : public IMoveSystemState {
public:
	// コンストラクタ
	None(Move* moveSystem, Player* player);
	// デストラクタ
	~None() override = default;

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

