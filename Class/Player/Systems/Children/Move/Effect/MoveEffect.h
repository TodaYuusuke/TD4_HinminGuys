#pragma once
#include "Adapter.h"
#include "../../../../Particles/IEffect.h"
#include "../../../../Particles/ParticleJsonDataStructs.h"

class MoveEffect : public IEffect {
public:
	enum class ParticleType {
		kCircle
	};

public:
	// コンストラクタ
	MoveEffect(Player* player, FollowCamera* followCamera);
	// デストラクタ
	~MoveEffect() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();
	/// <summary>
	/// JSONデータの調整項目を設定
	/// </summary>
	/// <param name="jsonData"></param>
	void SetJsonData(LWP::Utility::JsonIO& json);

public:// 各パーティクルの生成
	/// <summary>
	/// 土煙のパーティクル単体を生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateDustCloud(ParticleData&, LWP::Math::Vector3 pos);

public:// 演出開始
	/// <summary>
	/// 土煙パーティクル生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateDustClouds(LWP::Math::Vector3 pos);

	/// <summary>
	/// 指定されている数のパーティクルを生成
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	std::list<ParticleData> Emission(LWP::Math::Vector3 pos, float& createRotateY);

private:// パーティクルの更新処理
	/// <summary>
	/// 土煙の更新処理
	/// </summary>
	/// <param name="data"></param>
	void DustCloudUpdate(ParticleData& data);

private:// jsonで保存する値
	// 土煙
	ParticleJsonData dustCloudData_;

	// 色
	LWP::Utility::Color maxColor;
	LWP::Utility::Color minColor;

	int maxWhite = 255;
	int minWhite = 0;
	int maxAlpha = 255;
	int minAlpha = 0;

	// パーティクルの消え方
	std::function<float(float)> disappearEasing;

private:

	// 土煙を発生させるY軸の角度
	float createRotateY_;
};