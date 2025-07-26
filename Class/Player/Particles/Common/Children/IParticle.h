#pragma once
#include <Adapter.h>
#include "../../ParticleJsonDataStructs.h"

class Player;
class FollowCamera;
class IParticle {
public:
	// コンストラクタ
	IParticle() = default;
	// デストラクタ
	virtual ~IParticle() = default;

	/// <summary>
	/// 生成
	/// </summary>
	virtual void Create(const LWP::Math::Vector3& pos) = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

protected:
	/// <summary>
	/// パーティクルの更新処理
	/// </summary>
	virtual void UpdateParticle() = 0;

public:// アクセサ
#pragma region Getter
	//ParticleJsonData GetJsonData() { return jsonData_; }
	ParticleData GetParticleData() { return particleData_; }
#pragma endregion

#pragma region Setter
	//void SetJsonData(ParticleJsonData data) { jsonData_ = data; }
	/// <summary>
	/// 使用するテクスチャの名前
	/// </summary>
	/// <param name="name"></param>
	void SetTexName(const std::string& name) { texName_ = name; }
#pragma endregion

protected:// 外部から受け取る変数
	Player* player_;
	FollowCamera* followCamera_;
	//ParticleJsonData jsonData_;

protected:
	ParticleData particleData_;

	// 使用するテクスチャの名前
	std::string texName_;
};