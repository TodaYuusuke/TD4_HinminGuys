#pragma once
#include <Adapter.h>

// Playerの音再生時に必要な情報
struct PlayerAudioPlayData {
	std::string fileName;		// ファイルパスから名前(SE/～/ファイル名)
	std::string name;			// 識別名(std::mapで使用するため)
	float volume;				// 音量
};

/// <summary>
/// 自機関連で鳴る音の名前を定義する
/// </summary>
namespace PlayerAudio {
	namespace SE {
		// 移動時の音
		inline PlayerAudioPlayData move[5] = {
			{ "footSteps/footsteps0.mp3", "footSteps0", 1.0f },
			{ "footSteps/footsteps1.mp3", "footSteps1", 1.0f },
			{ "footSteps/footsteps2.mp3", "footSteps2", 1.0f },
			{ "footSteps/footsteps3.mp3", "footSteps3", 1.0f },
			{ "footSteps/footsteps4.mp3", "footSteps4", 1.0f }
		};
		// 剣を振る
		inline PlayerAudioPlayData attackSwing[3] = {
			{ "missingShot/missingShot0.mp3","missingShot0", 1.0f },
			{ "missingShot/missingShot1.mp3","missingShot1", 1.0f },
			{ "missingShot/missingShot2.mp3","missingShot2", 1.0f }
		};
	};
};