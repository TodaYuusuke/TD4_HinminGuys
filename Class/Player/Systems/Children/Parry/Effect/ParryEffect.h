#pragma once
#include "Adapter.h"
#include "../../../../Particles/IEffect.h"
#include "../../../../Particles/ParticleJsonDataStructs.h"
#include "../../../../Particles/Common/LargeFlashes.h"
#include "../../../../Particles/Common/ShortFlashes.h"
#include "../../../../Particles/Common/Rings.h"
#include "../../../../Particles/Common/Sparks.h"

class ParryEffect : public IEffect {
public:
	enum class ParticleType {
		kLine,
		kCircle,
		kLargeFlash,
		kShortFlash,
		kRing
	};

public:
	// コンストラクタ
	ParryEffect(Player* player, FollowCamera* followCamera);
	// デストラクタ
	~ParryEffect() override = default;

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

private:// 各パーティクル単体を生成
	///// <summary>
	///// 線パーティクル単体を生成
	///// </summary>
	///// <param name="pos"></param>
	///// <returns></returns>
	//void MakeLineParticle(ParticleData& particle, LWP::Math::Vector3 pos);
	///// <summary>
	///// 円パーティクル単体を生成
	///// </summary>
	///// <param name="pos"></param>
	///// <returns></returns>
	//void MakeCircleParticle(ParticleData& particle, LWP::Math::Vector3 pos);
	///// <summary>
	///// 大きい光パーティクル単体を生成
	///// </summary>
	///// <param name="pos"></param>
	///// <returns></returns>
	//void MakeLargeFlashParticle(ParticleData& particle, LWP::Math::Vector3 pos);
	///// <summary>
	///// 小さい光パーティクル単体を生成
	///// </summary>
	///// <param name="pos"></param>
	///// <returns></returns>
	//void MakeShortFlashParticle(ParticleData& particle, LWP::Math::Vector3 pos);
	///// <summary>
	///// リングを生成
	///// </summary>
	///// <param name="pos"></param>
	///// <returns></returns>
	//void MakeRingParticle(ParticleData& particle, LWP::Math::Vector3 pos);
	/// <summary>
	/// 指定されている数のジャストパリィパーティクルを生成
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	std::list<ParticleData> JustEmission(LWP::Math::Vector3 pos);
	/// <summary>
	/// 指定されている数の弱パリィパーティクルを生成
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	std::list<ParticleData> GoodEmission(LWP::Math::Vector3 pos);

public:// 演出開始
	/// <summary>
	/// ジャストパリィパーティクル生成開始
	/// </summary>
	void CreateJustParticles(LWP::Math::Vector3 pos);
	/// <summary>
	/// 弱パリィパーティクル生成開始
	/// </summary>
	void CreateGoodParticles(LWP::Math::Vector3 pos);

private:// パーティクルの更新処理
	///// <summary>
	///// 線パーティクルの更新処理
	///// </summary>
	///// <param name="data"></param>
	//void LineParticleUpdate(ParticleData& data);
	///// <summary>
	///// 円パーティクルの更新処理
	///// </summary>
	///// <param name="data"></param>
	//void CircleParticleUpdate(ParticleData& data);
	///// <summary>
	///// 大きい閃光の更新処理
	///// </summary>
	///// <param name="data"></param>
	//void LargeFlashUpdate(ParticleData& data);
	///// <summary>
	///// 小さい閃光の更新処理
	///// </summary>
	///// <param name="data"></param>
	//void ShortFlashUpdate(ParticleData& data);
	///// <summary>
	///// リングの更新処理
	///// </summary>
	///// <param name="data"></param>
	//void RingParticleUpdate(ParticleData& data);

public:// Getter, Setter
#pragma region Getter
	/// <summary>
	/// JSONに保存している値を取得
	/// </summary>
	/// <returns></returns>
	//ParticleJsonData GetParticleJsonData() { return jsonData_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// JSONデータの調整項目を設定
	/// </summary>
	/// <param name="jsonData"></param>
	void SetJsonData(LWP::Utility::JsonIO& json);
#pragma endregion

private:
	ParticleJsonData lineParticleData_;
	ParticleJsonData circleParticleData_;
	ParticleJsonData largeFlashData_;
	ParticleJsonData shortFlashData_;
	ParticleJsonData ringData_;

	// ケルビン値
	LimitF lineParticleKelvin = { 1000.0f, 3000.0f };
	LimitF circleParticleKelvin = { 1000.0f, 3000.0f };

	// 大きい閃光の最大サイズ
	LWP::Math::Vector3 maxLargeFlashScale = { 1.0f,1.0f,1.0f };
	// 小さい閃光の最大サイズ
	LWP::Math::Vector3 maxShortFlashScale = { 1.0f,1.0f,1.0f };
	// リングの最大サイズ
	LWP::Math::Vector3 maxRingScale = { 1.0f,1.0f,1.0f };

	// イージング終了時間
	float circleParticleEasingEndTime = 1.0f;


	std::unique_ptr<LargeFlashes> largeFlashes_;
	std::unique_ptr<ShortFlashes> shortFlashes_;
	std::unique_ptr<Rings> rings_;
	std::unique_ptr<Sparks> sparks_;
};