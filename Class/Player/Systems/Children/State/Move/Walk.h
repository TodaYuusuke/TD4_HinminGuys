#pragma once
#include "../IMoveSystemState.h"

class Walk : public IMoveSystemState {
public:
	// コンストラクタ
	Walk(Move* moveSystem, Player* player, const float& moveMultiply);
	// デストラクタ
	~Walk() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:

};

