#pragma once
#include "../Adapter/Adapter.h"

using namespace LWP;
using namespace LWP::Math;

class ICharacter {
public:
	// コンストラクタ
	ICharacter() = default;
	// デストラクタ
	virtual ~ICharacter() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

protected:
	// モデル
	LWP::Resource::RigidModel model_;
};