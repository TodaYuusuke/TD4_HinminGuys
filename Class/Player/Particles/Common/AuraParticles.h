#pragma once
#include "IParticles.h"
#include "Children/AuraParticle.h"

class AuraParticles : public IParticles {
public:
	// コンストラクタ
	AuraParticles();
	// デストラクタ
	~AuraParticles() override = default;

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
	/// 生成をするかを設定
	/// </summary>
	/// <param name="isActive"></param>
	void Start(const bool& isActive, const LWP::Math::Vector3& pos);
	/// <summary>
	/// オーラの生成終了
	/// </summary>
	void Finish();

private:
	/// <summary>
	/// パーティクルを生成
	/// </summary>
	/// <param name="value">生成する個数</param>
	void Add(int value);

public:
	/// <summary>
	/// 使用するテクスチャの名前
	/// </summary>
	/// <param name="name"></param>
	void SetTexName(const std::string& name) { texName_ = name; }

private:
	// 一度に生成する個数
	int count_;

private:
	// 使用するなら(多分あまり使わない)
	LWP::Utility::JsonIO json_;

	// パーティクル
	std::list<AuraParticle*> particles_;

	// 調整項目
	AuraParticle::AuraParticleJsonData jsonData_;

	// 生成座標
	LWP::Object::TransformQuat emitterPos_;

	// 使用するテクスチャの名前
	std::string texName_;

	// 生成するか
	bool isActive_ = false;
};