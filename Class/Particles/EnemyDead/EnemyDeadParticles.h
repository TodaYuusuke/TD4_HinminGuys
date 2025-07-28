#pragma once
#include "../Common/IParticles.h"
#include "Children/EnemyDeadParticle.h"

class EnemyDeadParticles : public IParticles {
public:
	// コンストラクタ
	EnemyDeadParticles(const std::string& texName);
	// デストラクタ
	~EnemyDeadParticles() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// Jsonで保存した調整項目()
	/// </summary>
	void JsonDebugGui();

	/// <summary>
	/// Jsonで保存する値を設定
	/// </summary>
	/// <param name="json"></param>
	void SetJsonData(LWP::Utility::JsonIO& json) override;
	void SetJsonData(const EnemyDeadParticle::EnemyDeadParticleJsonData& jsonData) { jsonData_ = jsonData; }
	void SetJsonData();

	/// <summary>
	/// パーティクルを生成
	/// </summary>
	/// <param name="value">生成する個数</param>
	void Add(int value);
	/// <summary>
	/// 場所を指定してパーティクルを生成
	/// </summary>
	/// <param name="value">生成する個数</param>
	void Add(int value, LWP::Math::Vector3 pos);

	/// <summary>
	/// jsonで保存する内容の取得
	/// </summary>
	/// <returns></returns>
	EnemyDeadParticle::EnemyDeadParticleJsonData GetJsonData() { return jsonData_; }

private:
	// 使用するなら(多分あまり使わない)
	LWP::Utility::JsonIO json_;

	// パーティクル
	std::list<EnemyDeadParticle*> particles_;

	// 調整項目
	EnemyDeadParticle::EnemyDeadParticleJsonData jsonData_;
};