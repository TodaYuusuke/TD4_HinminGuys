#pragma once
#include <Adapter.h>
#include "Children/IParticle.h"

class IParticles {
public:
	// コンストラクタ
	IParticles() = default;
	// デストラクタ
	virtual ~IParticles() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// Jsonで保存する値を設定
	/// </summary>
	/// <param name="json"></param>
	virtual void SetJsonData(LWP::Utility::JsonIO& json) = 0;

public:
	/// <summary>
	/// 使用するテクスチャの名前
	/// </summary>
	/// <param name="name"></param>
	void SetTexName(const std::string& name) { texName_ = name; }

protected:// 外部から受け取る変数
	Player* player_;
	FollowCamera* followCamera_;

protected:
	// json
	LWP::Utility::JsonIO json_;

	// 使用するテクスチャの名前
	std::string texName_;

	// 生成座標
	LWP::Math::Vector3 emitterPos_;
};

