#pragma once
#include <Adapter.h>
#include "IParticle.h"

class FollowCamera;
class Spark : public IParticle {
public:
	struct SparkJsonData {
		LimitVec3 velLimit;
		LimitVec3 scaleLimit;
		LimitF kelvinLimit = { 1000.0f, 3000.0f };
		LWP::Utility::Color color = { 255,255 ,255 ,255 };
		LWP::Math::Vector3 firstVel;
		LWP::Math::Vector3 acceleration;
		float createRange = 0.0f;
		float multiply;
		float maxElapseTime;								// パーティクルが存在できる時間
		LWP::Math::Vector3 maxScale = { 1.0f,1.0f,1.0f };	// 大きい閃光の最大サイズ
	};

public:
	// コンストラクタ
	Spark(const std::string& texName);
	// デストラクタ
	~Spark() override = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 生成
	/// </summary>
	void Create(const LWP::Math::Vector3& pos) override;

private:
	/// <summary>
	/// パーティクルの更新処理
	/// </summary>
	void UpdateParticle() override;

public:// アクセサ
#pragma region Getter
	SparkJsonData GetSparkJsonData() { return jsonData_; }
#pragma endregion

#pragma region Setter
	void SetSparkJsonData(SparkJsonData data) { jsonData_ = data; }
#pragma endregion

private:
	// ビルボード
	LWP::Primitive::NormalStretchedBillboard plane_;

	// 外部で調整するデータ
	SparkJsonData jsonData_;
};