#include "GameSceneManager.h"

using namespace LWP;

GameSceneManager::GameSceneManager(Player* player, EnemyManager* enemyManager, SceneTransitioner* sceneTransitioner, BGMPlayer* bgmPlayer, SEPlayer* sePlayer)
{
	// ポインタの取得
	player_ = player;
	enemyManager_ = enemyManager;
	sceneTransitioner_ = sceneTransitioner;
	bgmPlayer_ = bgmPlayer;
	sePlayer_ = sePlayer;
}

void GameSceneManager::Init()
{
	// BGM再生
	bgmPlayer_->PlayBGM("BattleBGM.mp3", "BattleBGM", 1.0f);
	// 音量設定
	bgmPlayer_->SetVolume("BattleBGM", bgmVolume_);

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
	buttonSprite_.material.color.A = 0;

	// タイマーのアクティブ状態のリセット
	timer_.SetIsActive(false);
}

void GameSceneManager::Update()
{
	// シーン遷移管理マネージャの更新
	sceneTransitioner_->Update();

	// ゲームが終了状態で無ければ
	if (!isEndGame_) { 
		// escキーが押された場合
		if (LWP::Input::Keyboard::GetTrigger(DIK_ESCAPE)) {
			// 強制的にタイトルへ戻す
			sceneTransitioner_->SetNextScene(SceneName::kTitle);
			sceneTransitioner_->SceneTransitionStart();

			// 早期リターンでこの後の処理をスキップ
			return;
		}

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
	else if(!sceneTransitioner_->GetIsSceneChange()) { // タイマーが非有効で、シーンチェンジ中で無ければ
		// 終了確認
		EndCheck();
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

		// 勝利BGMの再生
		if(clearBGMPath != ""){ bgmPlayer_->PlayBGM(clearBGMPath, "ResultBGM", 1.0f); }
		// クリア効果音の再生
		if(clearSEPath != ""){ sePlayer_->PlaySE(clearSEPath, "ResultSE", 1.0f); }
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

		// 敗北BGMの再生
		if (gameOverBGMPath != "") { bgmPlayer_->PlayBGM(gameOverBGMPath, "ResultBGM", 1.0f); }
		// 敗北効果音の再生
		if (gameOverSEPath != "") { sePlayer_->PlaySE(gameOverSEPath, "ResultSE", 1.0f); }
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
	buttonSprite_.material.color.A = static_cast<unsigned char>(LWP::Utility::Interp::LerpF(0.0f, 255.0f, timer_.GetProgress()));

	// ゲームBGMの音量を徐々に下げる
	bgmPlayer_->SetVolume("BattleBGM", LWP::Utility::Interp::LerpF(bgmVolume_, 0.0f, Utility::Easing::InOutQuart(timer_.GetProgress())));

	// 勝利状態で処理を切り替える
	if (isWin_) {
		// 補間で各種パラメーターを動かす
		resultSprite_.worldTF.translation.x = LWP::Utility::Interp::LerpF(LWP::Info::GetWindowWidthF() * -2.0f, LWP::Info::GetWindowWidthF() / 2.0f, LWP::Utility::Easing::InOutQuint(timer_.GetProgress()));
		resultSprite_.worldTF.scale.y = LWP::Utility::Interp::LerpF(0.0f, 1.0f, LWP::Utility::Easing::InOutQuart(timer_.GetProgress()));
	}
	else {
		// 補間で各種パラメーターを動かす
		resultSprite_.material.color.A = static_cast<unsigned char>(LWP::Utility::Interp::LerpF(0.0f, 255.0f, LWP::Utility::Easing::InOutQuart(timer_.GetProgress())));
	}

	// タイマー終了時、またはAボタン押下時
	if (timer_.GetIsFinish() || LWP::Input::Controller::GetTrigger(XINPUT_GAMEPAD_A)) {
		// 強制的にパラメーター設定
		fadeSprite_.material.color.A = 225;
		resultSprite_.worldTF.translation.x = LWP::Info::GetWindowWidthF() / 2.0f;
		resultSprite_.worldTF.scale.y = 1.0f;
		resultSprite_.material.color.A = 255;
		buttonSprite_.material.color.A = 255;
		// BGMの強制停止
		bgmPlayer_->Stop("BattleBGM");
		// タイマーの有効状態切り替え
		timer_.SetIsActive(false);
	}
}

void GameSceneManager::EndCheck()
{
	// Aボタン押下時、シーン遷移を行う
	if (LWP::Input::Controller::GetTrigger(XINPUT_GAMEPAD_A)) {
		// 決定音を鳴らす
		sePlayer_->PlaySE(decideSound_, "Decide", 1.0f);
		// リザルトBGMの停止
		bgmPlayer_->Stop("ResultBGM");

		// 勝利フラグの状態によって処理を変更する
		if (isWin_) { // 勝利
			sceneTransitioner_->SetNextScene(SceneName::kTitle);
			sceneTransitioner_->SceneTransitionStart();
		}
		else { // 敗北
			sceneTransitioner_->SetNextScene(SceneName::kGameScene);
			sceneTransitioner_->SceneTransitionStart();
		}

		// デルタタイムの値を元に戻す
		LWP::Info::SetDeltaTimeMultiply(1.0f);
	}
}
