#pragma once
#include "../Adapter/Adapter.h"
class TestCollider {
public:
	// コンストラクタ
	TestCollider();
	// デストラクタ
	~TestCollider() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

public:
	// シーンで使用しているカメラのポインタ
	LWP::Object::Camera* pCamera_;

	// モデル
	LWP::Resource::SkinningModel model_;

	// パリィ判定
	LWP::Object::Collision collider_;
	LWP::Object::Collider::AABB& aabb_;
};

