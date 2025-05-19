#pragma once
#include "../IMoveSystemState.h"

class Run : public IMoveSystemState {
public:
	// コンストラクタ
	Run(Move* moveSystem, Player* player, const float& moveMultiply);
	// デストラクタ
	~Run() override = default;

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

