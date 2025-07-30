#pragma once
#include "Adapter.h"
#include "../../../../../Particles/IEffect.h"
#include "../../../../../Particles/ParticleJsonDataStructs.h"
#include "../../../../../Particles/Common/LargeFlashes.h"
#include "../../../../../Particles/Common/ShortFlashes.h"
#include "../../../../../Particles/Common/Rings.h"
#include "../../../../../Particles/Common/Sparks.h"

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

public:// 演出開始
	/// <summary>
	/// ジャストパリィパーティクル生成開始
	/// </summary>
	void CreateJustParticles(const LWP::Math::Vector3& pos);
	/// <summary>
	/// 弱パリィパーティクル生成開始
	/// </summary>
	void CreateGoodParticles(const LWP::Math::Vector3& pos);
	/// <summary>
	/// パリィ時の足元に発生するパーティクル生成開始
	/// </summary>
	/// <param name="pos"></param>
	void CreateFootSparks(const LWP::Math::Vector3& pos);

public:// Getter, Setter
#pragma region Getter

#pragma endregion

#pragma region Setter
	/// <summary>
	/// JSONデータの調整項目を設定
	/// </summary>
	/// <param name="jsonData"></param>
	void SetJsonData(LWP::Utility::JsonIO& json);
#pragma endregion

public:
	// 足元に出る火花の時間
	float maxFootSparksTime = 0.5f;
	float dirayFootSparksTime = 0.15f;
	int maxFootSparkCount = 1;

private:
	std::unique_ptr<LargeFlashes> largeFlashes_;
	std::unique_ptr<ShortFlashes> shortFlashes_;
	std::unique_ptr<Rings> rings_;
	std::unique_ptr<Sparks> sparks_;

	// 足元に出すパーティクル
	std::unique_ptr<Sparks> footSparks_;
	float currentFrame_;
	bool isJust_ = false;
};