#pragma once
#include "IParticles.h"
#include "Children/Ring.h"

class Rings : public IParticles {
public:
	// コンストラクタ
	Rings(Player* player, FollowCamera* followCamera);
	// デストラクタ
	~Rings() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// Jsonで保存する値を設定
	/// </summary>
	/// <param name="json"></param>
	void SetJsonData(LWP::Utility::JsonIO& json) override;

	/// <summary>
	/// パーティクルを生成
	/// </summary>
	/// <param name="value">生成する個数</param>
	void Add(int value);
	/// <summary>
	/// 場所を指定してパーティクルを生成
	/// </summary>
	/// <param name="value">生成する個数</param>
	void Add(int value, LWP::Math::Vector3 position);

private:
	// パーティクル
	std::list<Ring*> particles_;

	// 調整項目
	Ring::RingJsonData jsonData_;
};