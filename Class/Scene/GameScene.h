#pragma once
#include "scene/IScene.h"
#include "../Player/Player.h"
#include "../Camera/FollowCamera.h"
#include "../Enemy/EnemyManager.h"
#include "../Player/Command/InputHandler.h"
#include "../TestCollider.h"
#include "../TestBillboard.h"
#include "../UI/UIManager.h"
#include "../Components/HitStopController.h"
#include <set>
#include "SceneTransitioner.h"
#include "../UI/DamageEffectEmitter.h"
#include "../Audio/BGMPlayer.h"
#include "../Audio/SEPlayer.h"
#include "../Particles/Particles.h"
#include "../World/World.h"

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
	// ヒットストップ
	HitStopController* hitStopController_;

	//　キー入力
	InputHandler* inputHandler_;

	// 自機
	Player player_;

	//ダメージエフェクトエミッター
	DamageEffectEmitter damageEffectEmitter_;

	// パーティクルの管理クラス
	Particles particles_;

	// 追従カメラ
	FollowCamera followCamera_;

	//世界共通のルールをまとめるクラス
	World world_;

	//敵マネージャー
	EnemyManager enemyManager_;

	// UIの管理クラス
	UIManager uiManager_;

	// オーディオ
	Audio audio;

	// 平面(一時的に作っているので要修正)
	LWP::Resource::RigidModel plane;
	// 天球(一時的に作っているので要修正)
	LWP::Resource::RigidModel skydome;

	//ライト
	LWP::Object::DirectionLight light_;

	//シーン切り替え
	SceneTransitioner sceneTransitioner_;
	//オーディオ再生クラス
	BGMPlayer bgmPlayer_;
	SEPlayer sePlayer_;


	//TestBillboard testBillboard_;
};