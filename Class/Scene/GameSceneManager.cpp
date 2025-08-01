#include "GameSceneManager.h"

GameSceneManager::GameSceneManager(Player* player, EnemyManager* enemyManager)
{
	// ポインタの取得
	player_ = player;
	enemyManager_ = enemyManager;
}

void GameSceneManager::Init()
{
	// フラグのリセット
	isEndGame_ = false;

	/// 各スプライトの初期化
	// フェード演出用スプライト
	fadeSprite_.material.color = { 0.0f, 0.0f, 0.0f, 0.0f };
	fadeSprite_.worldTF.scale = { LWP::Info::GetWindowWidthF(),LWP::Info::GetWindowHeightF(), 1.0f };
	fadeSprite_.isActive = false;
	// リザルト文字用スプライト
	resultSprite_.isActive = false;
	// ボタンスプライト
	buttonSprite_.isActive = false;

	// タイマーのアクティブ状態のリセット
	timer_.SetIsActive(false);
}

void GameSceneManager::Update()
{
	// ゲームが終了状態で無ければ
	if (!isEndGame_) { 
		// ゲームの進行状態チェック
		GameStateCheck();
		// 早期リターン
		return; 
	}

	// 終了演出
	EndStaging();
}

void GameSceneManager::GameStateCheck()
{
	//全ての敵が倒された場合
	if (enemyManager_->GetIsDefeatedAllEnemy()) {
		// 終了状態に
		isEndGame_ = true;
		// 勝利状態に
		isWin_ = true;
	}
	//全員倒す前にプレイヤーが死んだ場合
	else if (not enemyManager_->GetIsDefeatedAllEnemy() and not player_->GetIsAlive()) {
		// 終了状態に
		isEndGame_ = true;
		// 敗北状態に
		isWin_ = false;
	}
	else {
		// それ以外は早期リターン
		return;
	}

	// プレイヤーのコライダーをすべて無効
	player_->SetAllColliderState(false);

	// デルタタイムの値を0にする
	LWP::Info::SetDeltaTimeMultiply(0.0f);

	// 終了状態に移行した場合スプライトの表示
	fadeSprite_.isActive	= true;
	resultSprite_.isActive	= true;
	buttonSprite_.isActive	= true;

	// 勝利フラグの状態によって処理を変更
	if (isWin_) {
		// テクスチャ読み込み
		resultSprite_.LoadTexture("Result/GameClear.png");
		buttonSprite_.LoadTexture("Result/Exit.png");

		// 座標設定
		resultSprite_.worldTF.translation = { (LWP::Info::GetWindowWidthF() / 2.0f), (LWP::Info::GetWindowHeightF() / 5.0f), 1.0f };
	}
	else {
		// テクスチャ読み込み
		resultSprite_.LoadTexture("Result/GameOver.png");
		buttonSprite_.LoadTexture("Result/Retry.png");

		// 座標設定
		resultSprite_.worldTF.translation = { (LWP::Info::GetWindowWidthF() / 2.0f), (LWP::Info::GetWindowHeightF() / 5.0f), 1.0f };
	}

	// ボタンスプライト座標設定
	buttonSprite_.worldTF.translation = { (LWP::Info::GetWindowWidthF() / 2.0f), (LWP::Info::GetWindowHeightF() / 1.25f), 1.0f };

}

void GameSceneManager::EndStaging()
{
	
}
