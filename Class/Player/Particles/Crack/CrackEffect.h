#pragma once
#include "../Common/IParticles.h"
#include "../Common/DustClouds.h"
#include "../AttackHit/Children/AttackHitParticle.h"
#include "Children/CrackPlane.h"

class CrackEffect : public IParticles {
public:
	struct CrackEffectJsonData {
		AttackHitParticle::AttackHitParticleJsonData rockParticle;
		CrackPlane::CrackPlaneJsonData crackPlane;
		int smokeCount;
		int rockParticleCount;
	};

public:
	// コンストラクタ
	CrackEffect(const std::string& texName);
	// デストラクタ
	~CrackEffect() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理A
	/// </summary>
	void Update() override;

	/// <summary>
	/// Jsonで保存した調整項目
	/// </summary>
	void JsonDebugGui();

	/// <summary>
	/// Jsonで保存する値を設定
	/// </summary>
	/// <param name="json"></param>
	void SetJsonData(LWP::Utility::JsonIO& json) override;
	void SetJsonData();

	/// <summary>
	/// パーティクルを生成
	/// </summary>
	/// <param name="value">生成する個数</param>
	void Add(const LWP::Math::Vector3& pos);

private:
	// 使用するなら(多分あまり使わない)
	LWP::Utility::JsonIO json_;

	// 土煙
	std::unique_ptr<DustClouds> dustClouds_;

	// パーティクル
	std::list<IParticle*> particles_;

	// 調整項目
	CrackEffectJsonData jsonData_;
};