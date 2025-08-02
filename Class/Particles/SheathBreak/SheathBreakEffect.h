#pragma once
#include "../Common/IParticles.h"
#include "Children/FlickeringFlash.h"
#include "Children/RhombusParticle.h"
#include "Children/NoticeCircle.h"
#include "../Common//Rings.h"
#include <numbers>

class SheathBreakEffect : public IParticles {
public:
	struct SheathBreakEffectJsonData {
		FlickeringFlash::FlickeringFlashJsonData flickeringFlash;
		RhombusParticle::RhombusParticleJsonData rhombusParticle;
		NoticeCircle::NoticeCircleJsonData noticeCircle;
		int flickeringFlashCount;
		int rhombusParticleCount;
	};

public:
	// コンストラクタ
	SheathBreakEffect(FollowCamera* followCamera);
	// デストラクタ
	~SheathBreakEffect() override;

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
	void SetJsonData(const SheathBreakEffectJsonData& jsonData) { jsonData_ = jsonData; }
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
	SheathBreakEffectJsonData GetJsonData() { return jsonData_; }

private:
	// 使用するなら(多分あまり使わない)
	LWP::Utility::JsonIO json_;

	// パーティクル
	std::list<IParticle*> particles_;

	// リング
	NoticeCircle* noticeCircle_;

	// 調整項目
	SheathBreakEffectJsonData jsonData_;

	// 生成角度
	LWP::Math::Vector3 shotRotate_ = { 0.0f,(float)std::numbers::pi / 2.0f,0.0f };

	bool isCreate_;
};