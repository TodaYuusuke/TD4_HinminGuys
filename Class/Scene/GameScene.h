#pragma once
#include "scene/IScene.h"

class GameScene final
	: public IScene {
public:
	GameScene() = default;
	~GameScene() = default;

	// ** 純粋仮想関数の実体宣言 ** //

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: // ** これより先に必要な処理や変数を記述 ** //
};