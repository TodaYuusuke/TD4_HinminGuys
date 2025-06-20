#pragma once
#include "IEnemy.h"
#include <Adapter.h>

/// <summary>
/// 敵のパラメータ設定に関係するものたち
/// </summary>
namespace EnemyConfig {

	class ParameterEditor
	{
	public:
		ParameterEditor() = default;
		~ParameterEditor() = default;

		void Initialize();

		void DebugGUI();
		//指定したパラメータ取得
		EnemyParameter& GetParameter(EnemyType type) { return parameters_[int(type)]; }

	private:
		//各敵ごとに共通パラメータ設定
		std::array<EnemyParameter, int(EnemyType::kMax)> parameters_{};
		//共通パラメータのJSON
		LWP::Utility::JsonIO parameterJson_;

	};

}
