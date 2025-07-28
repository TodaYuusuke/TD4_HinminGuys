#pragma once
#include <Adapter.h>
#include "../../Common/Children/IParticle.h"

class FollowCamera;
class AttackHitParticle : public IParticle {
public:
	struct AttackHitParticleJsonData {
		LimitVec3 velLimit;
		LimitF scaleLimit;
		LWP::Utility::Color color = { 255,255 ,255 ,255 };
		LWP::Math::Vector3 firstVel;
		float acceleration;
		float multiply = 1.0f;
		float maxElapseTime;
	};

public:
	// コンストラクタ
	AttackHitParticle(const std::string& texName);
	// デストラクタ
	~AttackHitParticle() override = default;

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
	AttackHitParticleJsonData GetAttackHitParticleJsonData() { return jsonData_; }
#pragma endregion

#pragma region Setter
	void SetAttackHitParticleJsonData(AttackHitParticleJsonData data) { jsonData_ = data; }
#pragma endregion

private:
	// ビルボード
	LWP::Primitive::NormalBillboard2D plane_;

	// 外部で調整するデータ
	AttackHitParticleJsonData jsonData_;

	// ランダムスケール
	LWP::Math::Vector3 randomScale_;
};