#pragma once
#include "../IMoveSystemState.h"

class AttackRecovery : public IMoveSystemState {
public:
	// コンストラクタ
	AttackRecovery(Move* moveSystem, Player* player);
	// デストラクタ
	~AttackRecovery() override = default;

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