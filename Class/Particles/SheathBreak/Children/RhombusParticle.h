#pragma once
#include <Adapter.h>
#include "../../Common/Children/IParticle.h"

/// <summary>
/// ひし形のパーティクル
/// </summary>
class RhombusParticle : public IParticle {
public:
	struct RhombusParticleJsonData {
		LimitVec3 velLimit;
		LimitF scaleLimit;
		LWP::Utility::Color color;
		LWP::Math::Vector3 maxScale;
		LWP::Math::Vector3 firstVel;
		float creaateRange;
		float acceleration;
		float multiply;
		float dampingRate;
		float maxElapseTime;
	};

public:
	// コンストラクタ
	RhombusParticle(FollowCamera* followCamera, const std::string& texName);
	// デストラクタ
	~RhombusParticle() override = default;

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
	RhombusParticleJsonData GetRhombusParticleJsonData() { return jsonData_; }
#pragma endregion

#pragma region Setter
	void SetRhombusParticleJsonData(RhombusParticleJsonData data) { jsonData_ = data; }
	/// <summary>
	/// 生成角度の設定
	/// </summary>
	/// <param name="shotRotate"></param>
	void SetShotRotate(const LWP::Math::Vector3& shotRotate) { shotRotate_ = shotRotate; }
#pragma endregion

private:
	// ビルボード
	LWP::Primitive::NormalBillboard2D plane_;

	// 外部で調整するデータ
	RhombusParticleJsonData jsonData_;

	LWP::Math::Vector3 randomScale_;

	// 生成角度
	LWP::Math::Vector3 shotRotate_;
};