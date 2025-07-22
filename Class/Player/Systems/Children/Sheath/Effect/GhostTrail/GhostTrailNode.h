#pragma once
#include <Adapter.h>

class GhostTrailNode {
public:
	// コンストラクタ
	GhostTrailNode(const std::string& fileName, const std::string& materialName, const float& maxElapseFrame, LWP::Object::TransformQuat* target);
	// デストラクタ
	~GhostTrailNode() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

public:// アクセサ
	bool GetIsAlive() { return isAlive_; }

	/// <summary>
	/// 親子付けの設定
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(LWP::Object::TransformQuat* parent) { ghost_.worldTF.Parent(parent); }
	/// <summary>
	/// 対象のマテリアル名を設定
	/// </summary>
	/// <param name="name"></param>
	void SetMaterialName(const std::string& name) { materialName_ = name; }

private:
	float maxElapseFrame_ = 9.0f;

private:
	// 残像
	LWP::Resource::SkinningModel ghost_;

	// 残像を出す対象のworldTransform
	LWP::Object::TransformQuat* target_;

	// 速度
	LWP::Math::Vector3 velocity_;

	// 対象のマテリアル名
	std::string materialName_;

	// 経過時間
	float currentFrame_;

	bool isAlive_ = true;
};