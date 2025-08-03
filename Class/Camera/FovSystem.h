#pragma once
#include <Adapter.h>

/// <summary>
/// 視野角のイージング
/// </summary>
class FovSystem {
public:
	/// <summary>
	/// 線形補間の種類
	/// </summary>
	enum class LerpType {
		LERP,
		EXPONENTIAL
	};

public:
	// コンストラクタ
	FovSystem(const float& defaultFov);
	// デストラクタ
	~FovSystem() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

private:
	/// <summary>
	/// startFovの値に戻す
	/// </summary>
	void ReturnFov();

public:// アクセサ
	/// <summary>
	/// 開始
	/// </summary>
	/// <param name="currentFov">現在のFov</param>
	/// <param name="goalFov">目標のFov</param>
	/// <param name="endFrame">終了時間</param>
	/// <param name="returnStayFrame">イージング終了後の待機時間</param>
	void Start(const float& currentFov, const float& goalFov, const float& endFrame, const float& returnStayFrame);

#pragma region Getter
	/// <summary>
	/// 目標のFovを取得
	/// </summary>
	/// <returns></returns>
	float GetGoalFov() { return goalFov_; }
	/// <summary>
	/// 現在のFovを取得
	/// </summary>
	/// <returns></returns>
	float GetCurrentFov() { return currentFov_; }
	/// <summary>
	/// 現在の経過時間を取得
	/// </summary>
	/// <returns></returns>
	float GetCurrentFrame() { return currentFrame_; }
	/// <summary>
	/// 機能を使用するかを取得
	/// </summary>
	/// <returns></returns>
	float GetIsActive() { return isActive_; }
#pragma endregion

#pragma region Setter

#pragma endregion

private:
	// イージング関数
	std::map<LerpType,std::function<float(const float&, const float&, const float&)>> easingFunc_;

	// 目標のFov
	float goalFov_;
	// 最初のFov
	float startFov_;
	// 現在のFov
	float currentFov_;
	// 初期値のFov
	float defaultFov_;

	// 経過時間
	float currentFrame_;
	// イージング終了後の待機時間
	float returnStayFrame_;
	// 終了時間
	float endFrame_;

	// 機能を使用するか
	bool isActive_;

	bool isEasing_;
};

