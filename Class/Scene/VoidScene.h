#pragma once
#include "scene/IScene.h"
#include "SceneTransitioner.h"

/// <summary>
/// 平行光源のないシーンを経由するための何もないシーン
/// </summary>
class VoidScene final
	: public IScene {
public:
	VoidScene() = default;
	~VoidScene() = default;

	// ** 純粋仮想関数の実体宣言 ** //

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: // ** これより先に必要な処理や変数を記述 ** //

	//シーン切り替え
	SceneTransitioner sceneTransitioner_;

};