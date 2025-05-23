#pragma once
#include "scene/IScene.h"
#include "../Player/Player.h"
#include "../Camera/FollowCamera.h"
#include "../Enemy/EnemyManager.h"
#include "../Player/Command/InputHandler.h"
#include "../TestCollider.h"
#include <set>
#include "SceneTransitioner.h"

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

	/// <summary>
	/// デバッグ用のタブを表示(Debug時のみ)
	/// </summary>
	void DebugGUI();


private: // ** これより先に必要な処理や変数を記述 ** //
	//　キー入力
	InputHandler* inputHandler_;

	// 自機
	Player player_;

	// 追従カメラ
	FollowCamera followCamera_;

	//敵マネージャー
	EnemyManager enemyManager_;

	// 平面(一時的に作っているので要修正)
	LWP::Resource::RigidModel plane;
	// 天球(一時的に作っているので要修正)
	LWP::Resource::RigidModel skydome;

	//ライト
	LWP::Object::DirectionLight light_;

	// 当たり判定テスト用のオブジェクト
	TestCollider testCollider_;

	//シーン切り替え
	SceneTransitioner sceneTransitioner_;

};