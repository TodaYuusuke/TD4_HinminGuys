#pragma once
#include "../Common/IParticles.h"
#include "Children/DeadSmoke.h"
#include "../Common/Children/Spark.h"
#include "../Common/Sparks.h"

class BossDeadParticles : public IParticles {
public:
	struct BossDeadParticlesJsonData {
		DeadSmoke::DeadSmokeJsonData deadSmoke;
		LimitF CreatePosOffset;
		LimitF sparkFrequency;
		int sparkCount;
		int deadSmokeCount;
	};

public:
	// コンストラクタ
	BossDeadParticles();
	// デストラクタ
	~BossDeadParticles() override;

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

private:
	void CreateSparks(const LWP::Math::Vector3& pos);

public:
	/// <summary>
	/// Jsonで保存する値を設定
	/// </summary>
	/// <param name="json"></param>
	void SetJsonData(LWP::Utility::JsonIO& json) override;
	void SetJsonData(const BossDeadParticlesJsonData& jsonData) { jsonData_ = jsonData; }
	void SetJsonData();

	/// <summary>
	/// 生成をするかを設定
	/// </summary>
	/// <param name="isActive"></param>
	void Start(const float& createTime, const LWP::Math::Vector3& pos);

	/// <summary>
	/// パーティクルを生成
	/// </summary>
	/// <param name="value">生成する個数</param>
	void Add(const LWP::Math::Vector3& pos);

	/// <summary>
	/// jsonで保存する内容の取得
	/// </summary>
	/// <returns></returns>
	BossDeadParticlesJsonData GetJsonData() { return jsonData_; }
	bool GetEmptyParticles() { 
		if (particles_.empty() && sparks_->GetEmptyParticles()) {
			return true;
		}
		return false; 
	}
	float GetCurrentFrame() { return currentFrame_; }

private:
	// 死亡時の煙
	std::list<DeadSmoke*> particles_;
	std::unique_ptr<Sparks> sparks_;

	// 調整項目
	BossDeadParticlesJsonData jsonData_;

	// 火花の生成タイミング
	float createSparkTiming_;

	float currentFrame_;
};