#pragma once
#include "../Common/IParticles.h"
#include "Children/EnemySpawnParticle.h"

class EnemySpawnParticles : public IParticles {
public:
	// コンストラクタ
	EnemySpawnParticles(const std::string& texName);
	// デストラクタ
	~EnemySpawnParticles() override = default;

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
	void SetJsonData(const EnemySpawnParticle::EnemySpawnParticleJsonData& jsonData) { jsonData_ = jsonData; }
	void SetJsonData();
	/// <summary>
	/// 生成をするかを設定
	/// </summary>
	/// <param name="isActive"></param>
	void Start(const bool& isActive, const LWP::Math::Vector3& pos);
	/// <summary>
	/// オーラの生成終了
	/// </summary>
	void Finish();

	/// <summary>
	/// jsonで保存する内容の取得
	/// </summary>
	/// <returns></returns>
	EnemySpawnParticle::EnemySpawnParticleJsonData GetJsonData() { return jsonData_; }

private:
	/// <summary>
	/// パーティクルを生成
	/// </summary>
	/// <param name="value">生成する個数</param>
	void Add(int value);

public:
	bool GetIsStart() { return isActive_; }

private:
	// 一度に生成する個数
	int count_;

private:
	// 使用するなら(多分あまり使わない)
	LWP::Utility::JsonIO json_;

	// パーティクル
	std::list<EnemySpawnParticle*> particles_;

	// 調整項目
	EnemySpawnParticle::EnemySpawnParticleJsonData jsonData_;

	// 生成するか
	bool isActive_ = false;
};