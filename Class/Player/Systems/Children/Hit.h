#pragma once
#include "../ISystem.h"
#include "../../Gauge/HP/HP.h"

class Hit : public ISystem {
public:
	Hit() = default;
	~Hit() = default;

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
	/// 
	/// </summary>
	void DebugGUI() override;

	/// <summary>
	/// jsonファイルの作成
	/// </summary>
	void CreateJsonFIle() override;

private:
	/// <summary>
	/// 被弾時の更新処理
	/// </summary>
	void HitUpdate();

public:
#pragma region Setter
	void StartDamage(const float& damageValue, const float& multiply = 1.0f) {
		hp_.SetDeltaValue(damageValue);
		hp_.SetMultiply(damageValue);
		hp_.Hit();
	}
	/// <summary>
	/// 被弾演出開始
	/// </summary>
	void StartEffect() { isHit_ = true; }
	/// <summary>
	/// HPの最大値を設定
	/// </summary>
	/// <param name="maxHP"></param>
	void SetMaxHP(const float& maxHP) {
		hp_.SetMaxValue(maxHP);
	}
	/// <summary>
	/// 演出終了時間を設定
	/// </summary>
	/// <param name="endFrame">演出終了時間</param>
	void SetEndFrame(float endFrame) {
		endFrame_ = endFrame;
		currentFrame_ = endFrame_;
	}
#pragma endregion

#pragma region Getter
	/// <summary>
	/// 被弾演出中か
	/// </summary>
	/// <returns></returns>
	const bool& GetIsHit() { return isHit_; }
#pragma endregion

private:// jsonで保存する値

private:
	// HP
	HP hp_;

	// 演出終了時間
	float endFrame_;
	// 経過時間
	float currentFrame_;

	// 被弾演出中か
	bool isHit_ = false;
	bool isPreHit_ = false;
};