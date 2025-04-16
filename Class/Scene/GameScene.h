#pragma once
#include "scene/IScene.h"
#include "../Player/Player.h"
#include "../Camera/FollowCamera.h"

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
	// 自機
	std::unique_ptr<Player> player_;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	// 平面(一時的に作っているので要修正)
	LWP::Resource::RigidModel plane;
};