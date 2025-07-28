#pragma once
#include <Adapter.h>
#include "../../Common/Children/IParticle.h"

class FollowCamera;
class CrackPlane : public IParticle {
public:
	struct CrackPlaneJsonData {
		LWP::Math::Vector3 scale = { 1,1,1 };
		float maxElapseTime;	// パーティクルが存在できる時間
	};

public:
	// コンストラクタ
	CrackPlane(const std::string& texName);
	// デストラクタ
	~CrackPlane() override = default;

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
	CrackPlaneJsonData GetCrackPlaneJsonData() { return jsonData_; }
#pragma endregion

#pragma region Setter
	void SetCrackPlaneJsonData(CrackPlaneJsonData data) { jsonData_ = data; }
#pragma endregion

private:
	// ビルボード
	LWP::Primitive::NormalSurface plane_;

	// 外部で調整するデータ
	CrackPlaneJsonData jsonData_;
};