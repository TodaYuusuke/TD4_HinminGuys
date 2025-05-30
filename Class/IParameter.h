#pragma once
#include "../Adapter/Adapter.h"

class IParameter {
public:
	// パラメータの情報
	struct ParameterData {
		float strength;				// 強さ
		float multiply = 1.0f;		// 倍率
	};

public:
	// コンストラクタ
	IParameter() = default;
	// デストラクタ
	virtual ~IParameter() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

public:
#pragma region Getter
	/// <summary>
	/// 攻撃のパラメータを取得
	/// </summary>
	/// <returns></returns>
	ParameterData GetAttackParameter() { return attack_; }
	/// <summary>
	/// 速度のパラメータを取得
	/// </summary>
	/// <returns></returns>
	ParameterData GetSpeedParameter() { return speed_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 攻撃のパラメータを設定
	/// </summary>
	/// <returns></returns>
	void SetAttackParameter(const ParameterData& attack) { attack_ = attack; }
	/// <summary>
	/// 速度のパラメータを設定
	/// </summary>
	/// <returns></returns>
	void SetSpeedParameter(const ParameterData& speed) { speed_ = speed; }
#pragma endregion

protected:
	// 攻撃に関する情報
	ParameterData attack_;
	// 速度に関する情報
	ParameterData speed_;

	// 名前
	std::string name_;
};