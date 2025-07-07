#pragma once
#include "Adapter.h"
#include "../../../../Particles/ParticleJsonDataStructs.h"

class Player;
class EvasionEffect : public LWP::Object::Particle {
public:
	// コンストラクタ
	EvasionEffect(Player* player);
	// デストラクタ
	~EvasionEffect() = default;

	/// <summary>
	/// パーティクルを１つ生成する度に呼ばれる関数
	/// </summary>
	/// <param name="newData">生成された実体の参照（mに基準となるmodelは既に代入されている）</param>
	/// <returns></returns>
	void Generate(LWP::Object::Particle::Data& data) override;
	/// <summary>
	/// パーティクルの更新処理
	/// </summary>
	/// <param name="data">更新する実態の参照</param>
	/// <returns></returns>
	bool UpdateParticle(LWP::Object::Particle::Data& data) override;

	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

public:
#pragma region Getter
	/// <summary>
	/// JSONに保存している値を取得
	/// </summary>
	/// <returns></returns>
	ParticleJsonData GetParticleJsonData() { return jsonData_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// JSONデータの調整項目を設定
	/// </summary>
	/// <param name="jsonData"></param>
	void SetJsonData(LWP::Utility::JsonIO& json);
#pragma endregion

private:// 外部からアドレスを受け取る変数
	Player* player_;

private:
	ParticleJsonData jsonData_;
};