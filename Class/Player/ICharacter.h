#pragma once
#include "../Adapter/Adapter.h"

class ICharacter {
public:
	// コンストラクタ
	ICharacter() = default;
	// デストラクタ
	virtual ~ICharacter() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

protected:
	// シーンで使用しているカメラのポインタ
	LWP::Object::Camera* pCamera_;

	// モデル
	LWP::Resource::SkinningModel model_;

	//アニメーション
	LWP::Resource::Animation animation_;
};