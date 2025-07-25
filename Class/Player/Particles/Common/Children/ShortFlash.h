#pragma once
#include <Adapter.h>
#include "IParticle.h"

class FollowCamera;
class ShortFlash : public IParticle {
public:
	struct ShortFlashJsonData {
		float maxElapseTime;	// パーティクルが存在できる時間
		// 大きい閃光の最大サイズ
		LWP::Math::Vector3 maxScale = { 1.0f,1.0f,1.0f };
	};

public:
	// コンストラクタ
	ShortFlash(Player* player, FollowCamera* followCamera);
	// デストラクタ
	~ShortFlash() override = default;

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
	ShortFlashJsonData GetShortFlashJsonData() { return jsonData_; }
#pragma endregion

#pragma region Setter
	void SetShortFlashJsonData(ShortFlashJsonData data) { jsonData_ = data; }
#pragma endregion

private:
	// ビルボード
	LWP::Primitive::NormalBillboard2D plane_;

	// 外部で調整するデータ
	ShortFlashJsonData jsonData_;
};