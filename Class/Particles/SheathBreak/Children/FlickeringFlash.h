#pragma once
#include <Adapter.h>
#include "../../Common/Children/IParticle.h"

/// <summary>
/// 点滅する閃光
/// </summary>
class FollowCamera;
class FlickeringFlash : public IParticle {
public:
	struct FlickeringFlashJsonData {
		LWP::Math::Vector3 maxRotDegree;	// 回転する角度(度数)
		LWP::Math::Vector3 maxScale = { 1.0f,1.0f,1.0f };
		LWP::Math::Vector3 minScale = { 1.0f,1.0f,1.0f };
		LWP::Utility::Color color;
		float maxScaleTime;			// スケールが最大になるまでの時間
		float minScaleTime;			// スケールが最小になるまでの時間
		float maxElapseTime;		// パーティクルが存在できる時間
	};

public:
	// コンストラクタ
	FlickeringFlash(FollowCamera* followCamera, const std::string& texName);
	// デストラクタ
	~FlickeringFlash() override = default;

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
	FlickeringFlashJsonData GetFlickeringFlashJsonData() { return jsonData_; }
#pragma endregion

#pragma region Setter
	void SetFlickeringFlashJsonData(FlickeringFlashJsonData data) { jsonData_ = data; }
	/// <summary>
	/// 生成時の角度の設定
	/// </summary>
	/// <param name="euler"></param>
	void SetEuler(const LWP::Math::Vector3& euler) { particleData_.euler = euler; }
	/// <summary>
	/// 生成時の大きさの設定
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const LWP::Math::Vector3& scale) { particleData_.scale = scale; }
#pragma endregion

private:
	// ビルボード
	LWP::Primitive::NormalBillboard2D plane_;

	// 外部で調整するデータ
	FlickeringFlashJsonData jsonData_;
};