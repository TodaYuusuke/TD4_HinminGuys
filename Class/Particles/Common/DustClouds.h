#pragma once
#include "IParticles.h"
#include "Children/DustCloud.h"

class DustClouds : public IParticles {
public:
	// コンストラクタ
	DustClouds(const std::string& texName);
	// デストラクタ
	~DustClouds() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// Jsonで保存した調整項目()
	/// </summary>
	void JsonDebugGui();

	/// <summary>
	/// Jsonで保存する値を設定
	/// </summary>
	/// <param name="json"></param>
	void SetJsonData(LWP::Utility::JsonIO& json) override;
	void SetJsonData();

	/// <summary>
	/// パーティクルを生成
	/// </summary>
	/// <param name="value">生成する個数</param>
	void Add(int value, const LWP::Math::Vector3& pos);

private:
	// 使用するなら(多分あまり使わない)
	LWP::Utility::JsonIO json_;

	// パーティクル
	std::list<DustCloud*> particles_;

	// 調整項目
	DustCloud::DustCloudJsonData jsonData_;

	// 生成角度
	LWP::Math::Vector3 shotRotate_;
};