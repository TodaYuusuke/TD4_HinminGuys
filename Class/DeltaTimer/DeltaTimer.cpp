#include "DeltaTimer.h"

void LWP::Utility::DeltaTimer::Start()
{
	// 各種項目のリセット
	nowFlame_ = 0.0f;	// フレーム
	isFinish_ = false;	// 終了トリガー
	isActive_ = true;	// 有効トリガー
}

void LWP::Utility::DeltaTimer::Start(float time)
{
	// 項目リセット
	Start();
	// 終了秒数取得
	finishFlame_ = time;
}

bool LWP::Utility::DeltaTimer::Update()
{
	// 終了している場合
	if (isFinish_) {
		// 実行中ではない
		return false;
	}

	// フレームを加算
	nowFlame_ += static_cast<float>(LWP::Info::GetDeltaTime()) * speed_;

	// 経過秒数が終了秒数を超えていたら
	if (nowFlame_ >= finishFlame_) {
		// タイマー終了
		isFinish_ = true;
	}
	return true;
}

void LWP::Utility::DeltaTimer::Stop()
{
	// 全メンバ変数をリセット
	nowFlame_		= 0.0f;
	isFinish_		= true;
	isActive_		= false;
	finishFlame_	= 0.0f;
}

void LWP::Utility::DeltaTimer::DebugGUI(const std::string& name)
{
	// ツリーノード開始
	if (ImGui::TreeNode(name.c_str())) {
		// 情報表示
		ImGui::Text("NowFlame : %4.2f", nowFlame_);			// 現在の経過秒数
		ImGui::Text("FinishFlame : %4.2f", finishFlame_);	// 終了秒数
		ImGui::Text("Progress : %4.2f", GetProgress());		// タイマー進捗
		ImGui::Text("Speed : %4.2f", speed_);				// 再生速度
		ImGui::Text("isFinish : %d", isFinish_);			// 終了トリガー
		ImGui::Text("isActive : %d", isActive_);			// 有効トリガー

		// 開始秒数の設定
		ImGui::DragFloat("StartTime", &imGuiStartTime_, 0.05f, 0.05f, 25.0f);
		// 再生速度の設定
		ImGui::DragFloat("Speed", &speed_, 0.05f, 0.05f, 10.0f);

		// 終了していなければ
		if (!isFinish_) {
			// タイマーの停止
			if (ImGui::Button("Stop")) {
				Stop();
			}
			// リスタート
			if (ImGui::Button("Restart")) {
				Start(imGuiStartTime_);
			}
		}
		else {
			// タイマー開始
			if (ImGui::Button("Start")) {
				Start(imGuiStartTime_);
			}
		}

		// ツリーノード終了
		ImGui::TreePop();
	}
}

float LWP::Utility::DeltaTimer::GetProgress() const
{
	// 現在のタイマーの進行度を返す
	return std::clamp(nowFlame_ / finishFlame_, 0.f, 1.f);
}
