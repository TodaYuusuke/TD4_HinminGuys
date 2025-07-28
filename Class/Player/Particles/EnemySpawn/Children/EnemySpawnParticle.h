#pragma once
#include <Adapter.h>
#include "../../Common/Children/IParticle.h"

class FollowCamera;
class EnemySpawnParticle : public IParticle {
public:
	struct EnemySpawnParticleJsonData {
		LimitVec3 velLimit;
		LimitF scaleLimit;
		LWP::Utility::Color color;
		float createRange = 0.0f;
		float acceleration;
		LWP::Math::Vector3 firstVel;
		float multiply;
		float maxElapseTime;		// パーティクルが存在できる時間
	};

public:
	// コンストラクタ
	EnemySpawnParticle(const std::string& texName);
	// デストラクタ
	~EnemySpawnParticle() override = default;

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
	/// <summary>
	/// jsonで保存する情報の取得
	/// </summary>
	/// <returns></returns>
	EnemySpawnParticleJsonData GetEnemySpawnParticleJsonData() { return jsonData_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// すでに設定されたjsonで保存する情報をセット
	/// </summary>
	/// <returns></returns>
	void SetEnemySpawnParticleJsonData(EnemySpawnParticleJsonData data) { jsonData_ = data; }
#pragma endregion

private:
	// ビルボード
	LWP::Primitive::NormalBillboard2D plane_;

	// スケールをランダムで所持
	LWP::Math::Vector3 randomScale_;

	// 外部で調整するデータ
	EnemySpawnParticleJsonData jsonData_;
};