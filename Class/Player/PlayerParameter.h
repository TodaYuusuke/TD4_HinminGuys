#pragma once
#include "../Adapter/Adapter.h"
#include "../IParameter.h"

class PlayerParameter : public IParameter {
public:
	// コンストラクタ
	PlayerParameter() = default;
	// デストラクタ
	~PlayerParameter() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// 鞘のパラメータを取得
	/// </summary>
	/// <returns></returns>
	ParameterData GetSheathParameter() { return sheath_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 鞘のパラメータを設定
	/// </summary>
	/// <returns></returns>
	void SetSheathParameter(const ParameterData& sheath) { sheath_ = sheath; }
#pragma endregion

private:
	// 鞘
	ParameterData sheath_;
};