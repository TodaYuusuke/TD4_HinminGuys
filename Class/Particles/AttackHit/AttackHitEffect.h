#pragma once
#include "../Common/IParticles.h"
#include "Children/AttackHitParticle.h"
#include "../Common/Children/Spark.h"
#include "../Common/Sparks.h"

class AttackHitEffect : public IParticles {
public:
	struct AttackHitEffectJsonData {
		AttackHitParticle::AttackHitParticleJsonData attackHitParticle;
		LimitF CreatePosOffset;
		int sparkCount;
		int attackHitParticleCount;
	};

public:
	// コンストラクタ
	AttackHitEffect();
	// デストラクタ
	~AttackHitEffect() override;

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
	void SetJsonData(const AttackHitEffectJsonData& jsonData) { jsonData_ = jsonData; }
	void SetJsonData();

	/// <summary>
	/// パーティクルを生成
	/// </summary>
	/// <param name="value">生成する個数</param>
	void Add(const LWP::Math::Vector3& pos);

	/// <summary>
	/// jsonで保存する内容の取得
	/// </summary>
	/// <returns></returns>
	AttackHitEffectJsonData GetJsonData() { return jsonData_; }

private:
	// パーティクル
	std::list<AttackHitParticle*> particles_;
	std::unique_ptr<Sparks> sparks_;

	// 調整項目
	AttackHitEffectJsonData jsonData_;
};