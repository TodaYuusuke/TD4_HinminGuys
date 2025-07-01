#pragma once
#include "Adapter.h"
#include "../../../../Particles/ParticleJsonDataStructs.h"

class Player;
class FollowCamera;
class ParryEffect {
public:
	enum class ParticleType {
		kLine,
		kCircle
	};

	struct ParticleData {
		LWP::Primitive::Billboard2D billboard;
		LWP::Resource::RigidModel plane;		// 平面
		LWP::Math::Vector3 vel;					// 速度
		float multiply;
		float lifeTime;							// 生存時間
		float currentTime = 0;					// 経過フレーム
		ParticleType type;
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
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	ParticleData MakeLineParticle(LWP::Math::Vector3 pos);
	/// <summary>
	/// 円パーティクル単体を生成
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	ParticleData MakeCircleParticle(LWP::Math::Vector3 pos);
	/// <summary>
	/// 指定されている数のパーティクルを生成
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	std::list<ParticleData> Emission(LWP::Math::Vector3 pos);

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

public:
	/// <summary>
	/// パーティクル生成開始
	/// </summary>
	void Create(LWP::Math::Vector3 pos);

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
	LimitF lineParticleKelvin = { 1000.0f, 3000.0f };
	LimitF circleParticleKelvin = { 1000.0f, 3000.0f };
	float circleParticleEasingEndTime = 1.0f;

private:
	std::list<ParticleData> particles_;
};