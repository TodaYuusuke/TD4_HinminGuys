#pragma once
#include <Adapter.h>
#include "IParticle.h"

class FollowCamera;
class AuraParticle : public IParticle {
public:
	struct AuraParticleJsonData {
		LimitVec3 velLimit;
		LimitF scaleLimit;
		LWP::Utility::Color color;
		float acceleration;
		float multiply;
		float maxElapseTime;								// パーティクルが存在できる時間
	};

public:
	// コンストラクタ
	AuraParticle(Player* player, FollowCamera* followCamera, const std::string& texName);
	// デストラクタ
	~AuraParticle() override = default;

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
	AuraParticleJsonData GetAuraParticleJsonData() { return jsonData_; }
#pragma endregion

#pragma region Setter
	void SetAuraParticleJsonData(AuraParticleJsonData data) { jsonData_ = data; }
	void SetParent(LWP::Object::TransformQuat* emitter) { emitterPos_ = emitter; }
#pragma endregion

private:
	// ビルボード
	LWP::Primitive::NormalBillboard2D plane_;

	// 外部で調整するデータ
	AuraParticleJsonData jsonData_;

	// 生成座標
	LWP::Object::TransformQuat* emitterPos_;
	LWP::Math::Vector3 localPos_;
};