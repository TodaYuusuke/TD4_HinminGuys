#pragma once
#include <Adapter.h>
#include "../Player/Player.h"
#include "../Enemy/EnemyManager.h"
#include "SceneTransitioner.h"
#include "../Timer/FrameTimer.h"

/// <summary>
/// ゲームシーンの進捗等を管理するマネージャクラス
/// </summary>
class GameSceneManager
{
private: // サブクラス

	

public: // コンストラクタ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player">プレイヤー実体</param>
	/// <param name="enemyManager">敵の管理マネージャ</param>
	/// <param name="sceneTransitioner">シーン遷移マネージャ</param>
	GameSceneManager(Player* player, EnemyManager* enemyManager, SceneTransitioner* sceneTransitioner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameSceneManager() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// デバッグ用GUI関数
	/// </summary>
	void DebugGUI();

private: // その他関数群

	/// <summary>
	/// 現在のゲーム状態を確認する
	/// </summary>
	void GameStateCheck();

	/// <summary>
	/// 終了演出関数
	/// </summary>
	void EndStaging();

	/// <summary>
	/// 終了確認関数
	/// </summary>
	void EndCheck();

public: // アクセッサ等

	/// <summary>
	/// ゲームの終了フラグのゲッター
	/// </summary>
	/// <returns>ゲームが終了しているか</returns>
	bool GetIsEndGame() { return isEndGame_; }

private: // メンバ変数

	// プレイヤー本体
	Player* player_ = nullptr;
	// 敵管理マネージャ本体
	EnemyManager* enemyManager_ = nullptr;
	// シーン遷移マネージャ本体
	SceneTransitioner* sceneTransitioner_ = nullptr;

	// ゲーム終了フラグ
	bool isEndGame_ = false;
	// 勝利フラグ
	bool isWin_ = false;

	// 画面背景を暗くするスプライト
	LWP::Primitive::NormalSprite fadeSprite_{};
	// リザルト文字スプライト
	LWP::Primitive::NormalSprite resultSprite_{};
	// ボタンスプライト
	LWP::Primitive::NormalSprite buttonSprite_{};

	// 演出用タイマー
	LWP::Utility::FrameTimer timer_{};

};

