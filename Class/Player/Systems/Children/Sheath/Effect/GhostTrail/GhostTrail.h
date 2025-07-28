#pragma once
#include <Adapter.h>
#include "GhostTrailNode.h"

/// <summary>
/// 残像の列
/// </summary>
class GhostTrail {
public:
	// コンストラクタ
	GhostTrail(const std::string& fileName, std::string materialName, LWP::Object::TransformQuat* target);
	// デストラクタ
	~GhostTrail();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

private:
	/// <summary>
	/// 残像単体を生成
	/// </summary>
	/// <returns></returns>
	GhostTrailNode* CreateGhostTrailNode();

public:// アクセサ
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 親子付けの設定
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(LWP::Object::TransformQuat* parent) { 
		for (GhostTrailNode* ghost : ghostNodes_) {
			ghost->SetParent(parent);
		}
	}
	void SetModelFileName(const std::string& fileName) { modelFileName_ = fileName; }
	void SetMaterialName(const std::string& name) { materialName_ = name; }
	void SetIsActive(const bool& isActive) { isActive_ = isActive; }

private:
	// 残像の生成間隔
	float ghostIntervalFrame = 3;

	float maxElapseFrame = 9.0f;

private:
	// 残像
	std::vector<GhostTrailNode*> ghostNodes_;

	// 残像を出す対象のworldTransform
	LWP::Object::TransformQuat* target_;

	// 使用するモデル名
	std::string modelFileName_;
	std::string materialName_;

	// 経過時間
	float currentFrame_;

	bool isActive_ = true;
};