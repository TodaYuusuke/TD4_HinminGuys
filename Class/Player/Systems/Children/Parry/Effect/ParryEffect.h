#pragma once
#include "Adapter.h"

class ParryEffect : public LWP::Object::Particle {
public:
	// コンストラクタ
	ParryEffect() = default;
	// デストラクタ
	~ParryEffect() = default;

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

private:

};

