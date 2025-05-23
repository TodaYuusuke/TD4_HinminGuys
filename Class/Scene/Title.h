#pragma once
#include "scene/IScene.h"
#include "SceneTransitioner.h"

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

	//刀モデル
	LWP::Resource::RigidModel sword_;
	// 平面(一時的に作っているので要修正)
	LWP::Resource::RigidModel plane_;
	// 天球(一時的に作っているので要修正)
	LWP::Resource::RigidModel skydome_;

	//シーン切り替え
	SceneTransitioner sceneTransitioner_;

};