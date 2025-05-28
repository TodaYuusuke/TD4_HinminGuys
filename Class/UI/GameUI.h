#pragma once
#include "Adapter.h"
#include <string>
#include <functional>

/// <summary>
/// UIの動きに使うパラメータ
/// </summary>
struct UIParameter {
	LWP::Math::Vector3 startTranslation; //開始座標
	LWP::Math::Vector3 endTranslation; //終了座標
	LWP::Math::Vector3 startScale; //開始スケール
	LWP::Math::Vector3 endScale; //終了スケール
	LWP::Math::Vector3 startColor; //開始色(Aなし)
	LWP::Math::Vector3 endColor; //終了色(Aなし)
	float moveTime = 1.0f; //UIの動く時間(秒)
	float currentTime; //現在の時間
	LWP::Utility::Easing::Type easingType = LWP::Utility::Easing::Type::Liner; //イージングの種類
	bool isLoop = true; //ループするかどうか
	bool isReverse = false; //元に戻るかどうか
};

/// <summary>
/// SpriteをUIとして活用し動きを付けるクラス
/// </summary>
class GameUI
{
public:
	GameUI();
	~GameUI();

	//イージングタグ
	static std::array<const char*, int(LWP::Utility::Easing::Type::EasingCount)> easingName;

	//初期化
	void Initialize(const std::string& fileName, const std::string& UIName);
	//更新
	void Update();
	//時間リセット
	void ResetTime() { 
		activeData.currentTime = 0.0f;
		activeData.isReverse = false;
		idleData.currentTime = 0.0f;
		idleData.isReverse = false;
	}
	//画像変更
	void SetSprite(const std::string& fileName) { sprite_.LoadTexture(fileName); }
	//デバッグ用
	void DebugGUI();
	//アクティブ、非アクティブで二種類のパラメータを用意
	UIParameter activeData;
	UIParameter idleData;
	//画像がアクティブ状態かどうか
	bool isActive = false;

private:

	//受け取ったパラメータをイージングさせる
	void EasingUpdate(UIParameter& parameter);
	//タイプに応じたイージング処理
	float Easing(LWP::Utility::Easing::Type type, float x);

private:

	//画像
	LWP::Primitive::Sprite sprite_;
	//名前
	std::string name_;
	//json
	LWP::Utility::JsonIO json_;

};

