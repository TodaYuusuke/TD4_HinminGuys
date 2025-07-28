#pragma once
#include <Adapter.h>

/// <summary>
/// UIの残像を出す
/// </summary>
class EchoMotion {
public:
	// コンストラクタ
	EchoMotion(std::string fileName);
	EchoMotion(LWP::Resource::Texture texture);
	// デストラクタ
	~EchoMotion() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	bool GetIsActive() { return isActive_; }

	void SetSpriteData(LWP::Primitive::NormalSprite sprite) { afterimage_ = sprite; }
	void SetPos(LWP::Math::Vector3 pos) { afterimage_.worldTF.translation = pos; }

private:
	LWP::Primitive::NormalSprite afterimage_;
	LWP::Math::Vector3 startScale_ = { 1,1,1 };
	LWP::Math::Vector3 endScale_ = { 2,2,2 };
	float currentFrame_;
	float endFrame_ = 60.0f;
	int startAlpha_ = 255;
	// 可能状態か
	bool isActive_ = true;
};