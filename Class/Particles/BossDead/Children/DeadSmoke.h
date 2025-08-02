#pragma once
#include <Adapter.h>
#include "../../Common/Children/IParticle.h"

class FollowCamera;
class DeadSmoke : public IParticle {
public:
	struct DeadSmokeJsonData {
		LimitVec3 velLimit;
		LimitF scaleLimit;
		LimitColor colorLimit;
		LWP::Math::Vector3 firstVel;
		float acceleration;
		float multiply = 1.0f;
		float maxElapseTime;
	};

public:
	// コンストラクタ
	DeadSmoke(const std::string& texName);
	// デストラクタ
	~DeadSmoke() override = default;

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
	DeadSmokeJsonData GetDeadSmokeJsonData() { return jsonData_; }
#pragma endregion

#pragma region Setter
	void SetDeadSmokeJsonData(DeadSmokeJsonData data) { jsonData_ = data; }
#pragma endregion

private:
	// ビルボード
	LWP::Primitive::NormalBillboard2D plane_;

	// 外部で調整するデータ
	DeadSmokeJsonData jsonData_;

	// ランダムスケール
	LWP::Math::Vector3 randomScale_;
};