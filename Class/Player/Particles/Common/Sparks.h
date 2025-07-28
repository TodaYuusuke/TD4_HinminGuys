#pragma once
#include "IParticles.h"
#include "Children/Spark.h"

class Sparks : public IParticles {
public:
	// コンストラクタ
	Sparks(const std::string& texName);
	// デストラクタ
	~Sparks() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// Jsonで保存する値を設定
	/// </summary>
	/// <param name="json"></param>
	void SetJsonData(LWP::Utility::JsonIO& json) override;

	/// <summary>
	/// パーティクルを生成
	/// </summary>
	/// <param name="value">生成する個数</param>
	void Add(int value);
	/// <summary>
	/// 場所を指定してパーティクルを生成
	/// </summary>
	/// <param name="value">生成する個数</param>
	void Add(int value, LWP::Math::Vector3 position);

private:
	// パーティクル
	std::list<Spark*> particles_;

	// 調整項目
	Spark::SparkJsonData jsonData_;
};