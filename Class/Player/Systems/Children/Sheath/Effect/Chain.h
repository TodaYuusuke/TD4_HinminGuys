#pragma once
#include <Adapter.h>
#include "ChainNode.h"

class Chain {
public:
	// コンストラクタ
	Chain();
	// デストラクタ
	~Chain() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// jsonで保存する値を設定
	/// </summary>
	void CreateJsonData();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

public:// アクセサ
	void Reset();

#pragma region Getter
	/// <summary>
	/// 質点の情報を取得
	/// </summary>
	std::vector<PointMass> GetPointMassList() { return pointMassList_; }
	bool GetIsActive() { return isActive_; }
#pragma endregion

#pragma region Setter
	void SetStartPos(const LWP::Math::Vector3& startPos) { startPos_ = startPos; }
	void SetEndPos(const LWP::Math::Vector3& endPos) { endPos_ = endPos; }
	void SetIsActive(const bool& isActive) { isActive_ = isActive; }
#pragma endregion

private:// jsonで保存する値
	// 質点の個数
	int segmentCount = 20;
	// 質点の間隔
	float segmentLength = 1.0f;
	// 加速度
	LWP::Math::Vector3 defaultAcceleration = { 0.0f, -0.0008f, 0.0f };

private:
	LWP::Utility::JsonIO json_;
	// 鎖モデル
	LWP::Resource::RigidModel model_;
	// 質点リスト
	std::vector<PointMass> pointMassList_;
	// 全てのばねの計算
	std::vector<ChainNode*> chainNodeList_;

	// jsonの値反映用のばね
	std::unique_ptr<ChainNode> sampleSpring_;

	// 始点
	LWP::Math::Vector3 startPos_;
	// 終点
	LWP::Math::Vector3 endPos_;

	bool isActive_;
	bool isPreActive_;
};