#pragma once
#include "Adapter.h"
#include "../../DeltaTimer/DeltaTimer.h"

/// <summary>
/// 斬撃管理エフェクト管理
/// </summary>
class SlashEffector {
private: // サブクラス

	/// <summary>
	/// 斬撃エフェクトデータ構造体
	/// </summary>
	struct SlashEffectData {
		LWP::Primitive::SequenceSurface* plane; // 平面本体
		LWP::Utility::DeltaTimer aliveTimer;	// 生存時間タイマー
		int frame			= 0;				// フレーム
		bool isEnd			= false;			// 終了トリガー
	};

public:	// コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="tex">テクスチャ名</param>
	/// <param name="size">テクスチャ一枚ごとの大きさ</param>
	/// <param name="maxFrame">最大コマ数</param>
	SlashEffector(const std::string& tex, const LWP::Math::Vector2& size, const int maxFrame);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SlashEffector();

public: // メンバ関数

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 生成関数
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rotate">回転角</param>
	/// <param name="scale">大きさ</param>
	/// <param name="playTime">再生秒数</param>
	void Create(const LWP::Math::Vector3& pos, const LWP::Math::Quaternion& rotate, const LWP::Math::Vector3& scale , const float playTime);

	/// <summary>
	/// デバッグ用GUI関数
	/// </summary>
	void DebugGUI();

public: // アクセッサ等

	/// <summary>
	/// 親子付けする対象のワールドトランスフォームのセッター
	/// </summary>
	/// <param name="tf">親子付け対象</param>
	void SetParentTF(LWP::Object::TransformQuat* tf) { parentTF = tf; }

	/// <summary>
	/// 親子付け対象のワールドトランスフォームを削除する関数
	/// </summary>
	void DeleteParentTF() { parentTF = nullptr; }

private: // メンバ変数

	// 斬撃エフェクト
	std::list<SlashEffectData*> effects_;

	//テクスチャ名
	std::string texName_ = "";
	// テクスチャの1フレームごとのサイズ
	LWP::Math::Vector2 frameSize_ = { 0.0f, 0.0f };
	// 最大フレーム数
	int maxFrame_ = 0;

	// 親子付け対象ワールドトランスフォーム
	LWP::Object::TransformQuat* parentTF = nullptr;

	#pragma region デバッグ用変数

	// 生成座標
	LWP::Math::Vector3 imGuiPos_{};
	// 生成角
	LWP::Math::Vector3 imGuiRotate_{};
	// 生成スケール
	LWP::Math::Vector3 imGuiScale_{ 1.0f, 1.0f, 1.0f };

	// 秒数
	float imGuiAliveTime_ = 1.0f;

	// 読み込みテクスチャ名称
	std::string imGuiLoadTexName_ = "Effect/SwordSlash.png";

	#pragma endregion


};