#pragma once
#include <Adapter.h>
#include "../../Common/Children/IParticle.h"

class FollowCamera;
class EnemyDeadParticle : public IParticle {
public:
	struct EnemyDeadParticleJsonData {
		LimitVec3 velLimit;
		LimitF scaleLimit;
		LWP::Utility::Color color;
		float acceleration;
		float multiply;
		float maxElapseTime;								// パーティクルが存在できる時間
	};

public:
	// コンストラクタ
	EnemyDeadParticle(const std::string& texName);
	// デストラクタ
	~EnemyDeadParticle() override = default;

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
	EnemyDeadParticleJsonData GetEnemyDeadParticleJsonData() { return jsonData_; }
#pragma endregion

#pragma region Setter
	void SetEnemyDeadParticleJsonData(EnemyDeadParticleJsonData data) { jsonData_ = data; }
#pragma endregion

private:
	// ビルボード
	LWP::Primitive::NormalBillboard2D plane_;

	// 外部で調整するデータ
	EnemyDeadParticleJsonData jsonData_;

	// ランダムスケール
	LWP::Math::Vector3 randomScale_;

	// 生成角度
	LWP::Math::Vector3 shotRotate_;
};