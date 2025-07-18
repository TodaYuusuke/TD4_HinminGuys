#pragma once
#include "scene/IScene.h"
#include "SceneTransitioner.h"
#include "../UI/GameUI.h"
#include "../Audio/BGMPlayer.h"
#include "../Audio/SEPlayer.h"

class GameOver final
	: public IScene {
public:
	GameOver() = default;
	~GameOver() = default;

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

	//ゲームUI
	GameUI UIGameOver_;
	GameUI UI_Button_A_;

	//シーン切り替え
	SceneTransitioner sceneTransitioner_;
	//オーディオ再生クラス
	BGMPlayer bgmPlayer_;
	SEPlayer sePlayer_;

};