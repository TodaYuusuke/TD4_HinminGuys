#pragma once
#include <Adapter.h>

// 敵の音再生時に必要な情報
struct EnemyAudioPlayData {
	std::string fileName;		// ファイルパスから名前(SE/～/ファイル名)
	std::string name;			// 識別名(std::mapで使用するため)
	float volume;				// 音量
};

/// <summary>
/// 敵陣営で鳴る音の名前を定義する
/// </summary>
namespace EnemyAudio {
	namespace SE {
		// 移動時の音
		inline std::array<EnemyAudioPlayData, 5> move = { {
			{ "enemy/footSteps/enemyFootstep0.mp3", "footStep0", 0.7f },
			{ "enemy/footSteps/enemyFootstep1.mp3", "footStep1", 0.7f },
			{ "enemy/footSteps/enemyFootstep2.mp3", "footStep2", 0.7f },
			{ "enemy/footSteps/enemyFootstep3.mp3", "footStep3", 0.7f },
			{ "enemy/footSteps/enemyFootstep4.mp3", "footStep4", 0.7f }
		} };
	};
};