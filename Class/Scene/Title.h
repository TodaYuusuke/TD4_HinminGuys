#pragma once
#include "scene/IScene.h"
#include "../ComboEditor/ComboTree.h"

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


private: // ** これより先に必要な処理や変数を記述 ** //

	LWP::Resource::SkinningModel model_;
	LWP::Resource::Animation anim_;

	// コンボツリークラス
	ComboTree tree_{};

};