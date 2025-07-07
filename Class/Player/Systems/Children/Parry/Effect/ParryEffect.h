#pragma once
#include "Adapter.h"
#include "../../../../Particles/ParticleJsonDataStructs.h"

class Player;
class FollowCamera;
class ParryEffect {
public:
	enum class ParticleType {
		kLine,
		kCircle,
		kLargeFlash,
		kShortFlash,
		kRing
	};

	struct ParticleData {
		LWP::Primitive::Billboard2D billboard;
		LWP::Resource::RigidModel plane;		// 平面
		LWP::Math::Vector3 vel;					// 速度
		LWP::Math::Vector3 euler;				// オイラー角
		float multiply;
		float lifeTime;							// 生存時間
		float currentTime = 0;					// 経過フレーム
		ParticleType type;
		std::function<void(ParticleData&)> updateFunc;
	};
	// Particleを発生させる
	struct Emitter {
		LWP::Math::Vector3 pos;
		int32_t count;
		int32_t spawnCount;
		int32_t spawnLeft = 1;	// 発生の残り回数 
		float frequency;
		float frequencyTime;
	};

public:
	// コンストラクタ
	ParryEffect(Player* player, FollowCamera* followCamera);
	// デストラクタ
	~ParryEffect() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

private:
	/// <summary>
	/// 線パーティクル単体を生成
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	ParticleData MakeLineParticle(LWP::Math::Vector3 pos);
	/// <summary>
	/// 円パーティクル単体を生成
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	ParticleData MakeCircleParticle(LWP::Math::Vector3 pos);
	/// <summary>
	/// 大きい光パーティクル単体を生成
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	ParticleData MakeLargeFlashParticle(LWP::Math::Vector3 pos);
	/// <summary>
	/// 小さい光パーティクル単体を生成
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	ParticleData MakeShortFlashParticle(LWP::Math::Vector3 pos);
	/// <summary>
	/// リングを生成
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	ParticleData MakeRingParticle(LWP::Math::Vector3 pos);
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

public:
	/// <summary>
	/// ジャストパリィパーティクル生成開始
	/// </summary>
	void CreateJustParticles(LWP::Math::Vector3 pos);
	/// <summary>
	/// 弱パリィパーティクル生成開始
	/// </summary>
	void CreateGoodParticles(LWP::Math::Vector3 pos);

private:
	/// <summary>
	/// 線パーティクルの更新処理
	/// </summary>
	/// <param name="data"></param>
	void LineParticleUpdate(ParticleData& data);
	/// <summary>
	/// 円パーティクルの更新処理
	/// </summary>
	/// <param name="data"></param>
	void CircleParticleUpdate(ParticleData& data);
	/// <summary>
	/// 大きい閃光の更新処理
	/// </summary>
	/// <param name="data"></param>
	void LargeFlashUpdate(ParticleData& data);
	/// <summary>
	/// 小さい閃光の更新処理
	/// </summary>
	/// <param name="data"></param>
	void ShortFlashUpdate(ParticleData& data);
	/// <summary>
	/// リングの更新処理
	/// </summary>
	/// <param name="data"></param>
	void RingParticleUpdate(ParticleData& data);

#pragma region 数学関数
	/// <summary>
	/// 方向ベクトルからクォータニオンを算出
	/// </summary>
	/// <param name="dirVec"></param>
	/// <returns></returns>
	LWP::Math::Quaternion LookRotation(const LWP::Math::Vector3& dirVec);

	LWP::Math::Quaternion HorizontalBillboard(const LWP::Math::Vector3& targetPos);

	LWP::Math::Quaternion StretchedBillboard(const LWP::Math::Vector3& targetPos, const LWP::Math::Vector3& vel, const LWP::Math::Vector3& cameraPos);

	LWP::Math::Quaternion QuaternionRotateMatrix(const LWP::Math::Matrix4x4& m);

	float ExponentialInterpolateF(const float& current, const float& target, float damping);

	float GetRotationAngleFromMatrix(const LWP::Math::Matrix4x4& m);

	LWP::Math::Quaternion QuaternionFromMatrix(const LWP::Math::Matrix4x4& m);
#pragma endregion

public:
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

private:// 外部から受け取る変数
	Player* player_;
	FollowCamera* followCamera_;

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

private:
	std::list<ParticleData> particles_;

	LWP::Math::Vector3 emitterPos_;
};