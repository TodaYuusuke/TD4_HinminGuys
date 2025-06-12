#pragma once
#include <Adapter.h>
#include <list>

/// <summary>
/// ダメージ一回分のエフェクト
/// </summary>
struct DamageEffect {
	std::list<LWP::Primitive::Sprite> sprites; //画像
	LWP::Math::Vector3 position; //座標
	int digit; //桁数
	float damage; //受けたダメージ
	static float maxScale; //最大スケール
	static float widthSpacing; //横幅間隔
	static float maxDisplayTime; //最大表示時間(秒)
	static float vanishThreshold; //消えていく時間のしきい値
	float remainingDisplayTime; //残りの表示時間(秒)
	static float divide; //分割数
	bool isVanish = false; //消えていくかどうか
};

/// <summary>
/// ダメージエフェクト表示を呼び出すためのクラス
/// </summary>
class DamageEffectEmitter
{
public:
	DamageEffectEmitter() = default;
	~DamageEffectEmitter() = default;
	//初期化
	void Initialize();
	//更新
	void Update();
	//デバッグ処理
	void DebugGUI();
	//エフェクト追加
	void AddEffect(const float& damage, const LWP::Math::Vector3& position);
	//カメラセット
	void SetCamera(LWP::Object::Camera* camera) { camera_ = camera; }

private:

	//カメラポインタ
	LWP::Object::Camera* camera_;

	//数字エフェクト
	std::list<DamageEffect> effects_;

};
