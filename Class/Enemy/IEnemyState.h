#pragma once
#include "../Adapter/Adapter.h"

class IEnemy;

/// <summary>
/// 敵の行動パターン基底クラス
/// </summary>
class IEnemyState
{
public:
	virtual ~IEnemyState() = default;
	//初期化
	virtual void Initialize(IEnemy* enemy) = 0;
	//更新
	virtual void Update() = 0;

protected:

	//敵の情報
	IEnemy* enemy_;

};
