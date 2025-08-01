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
	fadeSprite_.worldTF.translation = { LWP::Info::GetWindowWidthF(),LWP::Info::GetWindowHeightF(), -1.25f };
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

	// タイマーが有効状態のときのみ更新する
	if (timer_.GetIsActive()) {
		// 終了演出
		EndStaging();
	}
}

void GameSceneManager::DebugGUI()
{
	timer_.DebugGUI("GameSceneManagerTimer");
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
		resultSprite_.worldTF.translation = { LWP::Info::GetWindowWidthF() * -2.0f, (LWP::Info::GetWindowHeightF() / 5.0f), -2.5f };
		resultSprite_.worldTF.scale = { 1.0f, 0.0f, 1.0f };

		// 演出用タイマーを指定秒数で開始
		timer_.Start(2.0f);
	}
	else {
		// テクスチャ読み込み
		resultSprite_.LoadTexture("Result/GameOver.png");
		buttonSprite_.LoadTexture("Result/Retry.png");

		// 座標設定
		resultSprite_.worldTF.translation = { (LWP::Info::GetWindowWidthF() / 2.0f), (LWP::Info::GetWindowHeightF() / 5.0f), -2.5f };
		// 透明度設定
		resultSprite_.material.color = { 1.0f, 1.0f, 1.0f, 0.0f };

		// 演出用タイマーを指定秒数で開始
		timer_.Start(3.0f);
	}

	// ボタンスプライト座標設定
	buttonSprite_.worldTF.translation = { (LWP::Info::GetWindowWidthF() / 2.0f), (LWP::Info::GetWindowHeightF() / 1.25f), -2.5f };

}

void GameSceneManager::EndStaging()
{
	// タイマー更新
	timer_.Update();

	// 背景スプライトの透明度を徐々に下げる
	fadeSprite_.material.color.A = static_cast<unsigned char>(LWP::Utility::Interp::LerpF(0.0f, 225.0f, LWP::Utility::Easing::InOutQuart(timer_.GetProgress())));

	// 勝利状態で処理を切り替える
	if (isWin_) {
		// 補間で各種パラメーターを動かす
		resultSprite_.worldTF.translation.x = LWP::Utility::Interp::LerpF(LWP::Info::GetWindowWidthF() * -2.0f, LWP::Info::GetWindowWidthF() / 2.0f, LWP::Utility::Easing::InOutExpo(timer_.GetProgress()));
		resultSprite_.worldTF.scale.y = LWP::Utility::Interp::LerpF(0.0f, 1.0f, LWP::Utility::Easing::OutExpo(timer_.GetProgress()));
	}
	else {
		// 補間で各種パラメーターを動かす
		resultSprite_.material.color.A = static_cast<unsigned char>(LWP::Utility::Interp::LerpF(0.0f, 255.0f, LWP::Utility::Easing::InOutQuart(timer_.GetProgress())));
	}

	// タイマー終了時
	if (timer_.GetIsFinish()) {
		// タイマーの有効状態切り替え
		timer_.SetIsActive(false);
	}
}
