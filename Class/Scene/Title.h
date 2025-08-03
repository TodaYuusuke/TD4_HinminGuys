#pragma once
#include "scene/IScene.h"
#include "SceneTransitioner.h"
#include "../UI/GameUI.h"
#include "../Audio/BGMPlayer.h"
#include "../Audio/SEPlayer.h"

enum class SelectUI {
	kStart,
	kEnd,
	kMax
};

class Title final
	: public IScene {
public:
	Title() = default;
	~Title() = default;

	// ** 純粋仮想関数の実体宣言 ** //

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	/// <summary>
	/// デバッグ用のタブを表示(Debug時のみ)
	/// </summary>
	void DebugGUI();

private: // ** これより先に必要な処理や変数を記述 ** //

	//ライト
	LWP::Object::DirectionLight light_;

	// 平面(一時的に作っているので要修正)
	LWP::Resource::RigidModel plane_;
	// 天球(一時的に作っているので要修正)
	LWP::Resource::RigidModel skydome_;

	// プレイヤーモデル
	LWP::Resource::SkinningModel playerModel_;
	// プレイヤーアニメーション
	LWP::Resource::Animation playerAnimation_;

	// 刀モデル
	LWP::Resource::SkinningModel swordModel_;
	// 鞘モデル
	LWP::Resource::SkinningModel sheathModel_;

	//ゲームUI
	GameUI UIStart_;
	GameUI UIExit_;
	GameUI UITitleLogo_;

	//シーン切り替え
	SceneTransitioner sceneTransitioner_;

	SelectUI selectUI_;
	//オーディオ再生クラス
	BGMPlayer bgmPlayer_;
	SEPlayer sePlayer_;

	// BGMのファイルパス
	std::string bgmPath_ = "TitleBGM.mp3";
	// 各種効果音までのファイルパス
	std::string decideSound_ = "system/select.mp3";
	std::string endSound_ = "system/end.mp3";
	std::string moveSound_ = "system/move.mp3";
	// 効果音ボリューム
	float seVolume_ = 1.0f;

};