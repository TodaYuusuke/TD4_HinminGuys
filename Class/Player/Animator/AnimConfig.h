#pragma once
#include "../Adapter/Adapter.h"

/// <summary>
/// 使用するアニメーションの名前を定義する(実装中)
/// </summary>
namespace AnimConfig {
	namespace Name {
		inline std::string NormalAttack = "LightAttack1";			// 通常攻撃
		inline std::string Parry = "Gaurd";							// パリィ
		inline std::string Evasion = "Dash";						// 回避
		inline std::string Sheath = "";								// 鞘投げor鞘回収
	};
};