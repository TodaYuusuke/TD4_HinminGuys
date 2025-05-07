#pragma once
#include "scene/IScene.h"
#include "../Player/Player.h"
#include "../Camera/FollowCamera.h"
#include "../Enemy/EnemyManager.h"
#include "../Player/Command/ICommand.h"
#include "../Player/Command/InputHandler.h"
#include <set>

class GameScene final
	: public IScene {
public:
	GameScene();
	~GameScene() override;

	// ** 純粋仮想関数の実体宣言 ** //

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: // ** これより先に必要な処理や変数を記述 ** //
	// 自機
	Player player_;
	InputHandler inputHandler_;
	//std::set<ICommand*> command_;

	// 追従カメラ
	FollowCamera followCamera_;

	//敵マネージャー
	EnemyManager enemyManager_;

	// 平面(一時的に作っているので要修正)
	LWP::Resource::RigidModel plane;

	//ライト
	LWP::Object::DirectionLight light_;

};