#pragma once
#include <Adapter.h>
#include "../../Common/Children/IParticle.h"

class FollowCamera;
class NoticeCircle : public IParticle {
public:
	struct NoticeCircleJsonData {
		LWP::Math::Vector3 maxScale = { 1.0f,1.0f,1.0f };
		LWP::Math::Vector3 minScale = { 1.0f,1.0f,1.0f };
		LWP::Utility::Color color;
		float maxScaleTime;			// スケールが最大になるまでの時間
		float minScaleTime;			// スケールが最小になるまでの時間
		float maxElapseTime;		// パーティクルが存在できる時間
	};

public:
	// コンストラクタ
	NoticeCircle();
	// デストラクタ
	~NoticeCircle() override = default;

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
	NoticeCircleJsonData GetNoticeCircleJsonData() { return jsonData_; }
#pragma endregion

#pragma region Setter
	void SetNoticeCircleJsonData(NoticeCircleJsonData data) { jsonData_ = data; }
#pragma endregion

private:
	// ビルボード
	std::array<LWP::Primitive::NormalBillboard2D, 2> planes_;

	// 外部で調整するデータ
	NoticeCircleJsonData jsonData_;
};