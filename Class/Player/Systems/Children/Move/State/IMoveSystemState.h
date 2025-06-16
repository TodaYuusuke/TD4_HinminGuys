#pragma once
#include "../Adapter/Adapter.h"

class Player;
class Move;
class IMoveSystemState {
public:
	virtual ~IMoveSystemState() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// アニメーションのコマンド
	/// </summary>
	virtual void AnimCommand() = 0;

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// 状態の名前を取得
	/// </summary>
	/// <returns></returns>
	std::string GetStateName() { return stateName_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 状態の名前を設定
	/// </summary>
	/// <param name="stateName"></param>
	void SetStateName(const std::string& stateName) { stateName_ = stateName; }
#pragma endregion

protected:
	Player* player_;
	Move* moveSystem_;

	// 状態の名前
	std::string stateName_;

	bool isActive_ = false;
};