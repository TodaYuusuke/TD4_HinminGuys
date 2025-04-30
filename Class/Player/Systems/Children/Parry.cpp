#include "Parry.h"
#include "../../Player.h"

Parry::Parry(LWP::Object::Camera* camera, Player* player) {
	pCamera_ = camera;
	player_ = player;
}

void Parry::Initialize() {
	isActive_ = false;

	// フレーム単位で発生するアクションイベントを管理するクラス
	eventOrder_.Initialize();
	// パリィ発生までの時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kSwingTime, "SwingTime" });
	// ジャストパリィの猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kJustParryTime, "JustParry" });
	// 通常パリィの猶予時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kGoodParryTime , "GoodParry" });
	// パリィの硬直時間
	eventOrder_.CreateTimeEvent(TimeEvent{ kRecoveryTime , "RecoveryTime" });
}

void Parry::Update() {
	// 入力処理
	InputUpdate();

	// パリィ機能を使えないなら早期リターン
	if (!isActive_) { return; }

	// frameごとに起きるイベント
	eventOrder_.Update();

	// 全てのイベントが終了しているなら機能停止
	if (eventOrder_.GetIsEnd()) {
		Reset();
	}
}

void Parry::Reset() {
	isActive_ = false;
}

void Parry::DebugGUI() {
	eventOrder_.DebugGUI();
}

void Parry::InputUpdate() {
	if (LWP::Input::Keyboard::GetPress(DIK_SPACE) || LWP::Input::Pad::GetPress(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		eventOrder_.Start();
		isActive_ = true;
	}
}