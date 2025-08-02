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
		inline std::array<PlayerAudioPlayData, 5> move = { {
			{ "player/footSteps/footsteps0.mp3", "footSteps0", 1.0f },
			{ "player/footSteps/footsteps1.mp3", "footSteps1", 1.0f },
			{ "player/footSteps/footsteps2.mp3", "footSteps2", 1.0f },
			{ "player/footSteps/footsteps3.mp3", "footSteps3", 1.0f },
			{ "player/footSteps/footsteps4.mp3", "footSteps4", 1.0f }
		} };
		// 剣を振る
		inline std::array<PlayerAudioPlayData, 3> attackSwing = { {
			{ "player/missingShot/missingShot0.mp3","missingShot0", 1.0f },
			{ "player/missingShot/missingShot1.mp3","missingShot1", 1.0f },
			{ "player/missingShot/missingShot2.mp3","missingShot2", 1.0f }
		} };
		namespace Sheath {
			// 鞘を戻す
			inline PlayerAudioPlayData returnSheath = {
				"player/sheath/returnSheath.mp3","returnSheath", 1.0f
			};
			// 鞘投げ
			inline PlayerAudioPlayData throwSheath = {
				"player/sheath/throwSheath.mp3","throwSheath", 1.0f
			};
			// 鎖
			inline PlayerAudioPlayData swingChain = {
				"player/sheath/swingingChain.mp3","swingingChain", 0.6f
			};
		};
		namespace Parry {
			// ジャストパリィ
			inline PlayerAudioPlayData justParry = {
				"player/parry/justParry.mp3","justParry", 1.0f
			};
			// 弱パリィ
			inline PlayerAudioPlayData goodParry = {
				"player/parry/parry.mp3","goodParry", 1.0f
			};
		};
		namespace Damage {
			// 被ダメージ
			inline PlayerAudioPlayData damage = {
				"player/damage.mp3","damage", 1.0f
			};
		};
		namespace Evasion {
			// 回避
			inline PlayerAudioPlayData dashWind = {
				"player/dashWInd.mp3","dashWind", 1.0f
			};
		};
	};
};