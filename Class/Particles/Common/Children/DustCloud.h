#pragma once
#include <Adapter.h>
#include "IParticle.h"

class FollowCamera;
class DustCloud : public IParticle {
public:
	struct DustCloudJsonData {
		LimitVec3 velLimit;
		LimitF scaleLimit;
		LimitInt whiteLimit;
		LimitInt alphaLimit;
		LWP::Math::Vector3 firstVel;
		float acceleration;
		float multiply;
		float dampingRate;
		float maxElapseTime;								// パーティクルが存在できる時間
	};

public:
	// コンストラクタ
	DustCloud(const std::string& texName);
	// デストラクタ
	~DustCloud() override = default;

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
	DustCloudJsonData GetDustCloudJsonData() { return jsonData_; }
#pragma endregion

#pragma region Setter
	void SetDustCloudJsonData(DustCloudJsonData data) { jsonData_ = data; }
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
	DustCloudJsonData jsonData_;

	// 生成角度
	LWP::Math::Vector3 shotRotate_;
};