#pragma once
#include "Adapter.h"
#include "ParticleJsonDataStructs.h"

class Player;
class FollowCamera;
class IEffect {
public:
	// コンストラクタ
	IEffect() = default;
	// デストラクタ
	virtual ~IEffect() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

protected:// 外部から受け取る変数
	Player* player_;
	FollowCamera* followCamera_;

protected:
	std::list<ParticleData> particles_;

	LWP::Math::Vector3 emitterPos_;
};

