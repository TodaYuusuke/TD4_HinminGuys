#pragma once
#include "Adapter.h"
#include "../Systems/Children/Parry/Effect/ParryEffect.h"
#include "../Systems/Children/Evasion/Effect/EvasionEffect.h"
#include "../Systems/Children/Move/Effect/MoveEffect.h"
#include "Common/LargeFlashes.h"
#include "Common/ShortFlashes.h"
#include "Common/Rings.h"
#include "Common/Sparks.h"
#include "Common/Children/FloatParticle.h"

class FollowCamera;
/// <summary>
/// 自機の行動所以で発生するパーティクルの管理クラス
/// </summary>
class Particles {
public:
	enum class MoveEffectType {
		kNone,
		kLeft,
		kRight
	};

public:
	// コンストラクタ
	Particles(Player* player, FollowCamera* followCamera);
	// デストラクタ
	~Particles() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// JSONデータを作成
	/// </summary>
	void CreateJsonData();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

public:// パーティクル生成
	/// <summary>
	/// ジャストパリィ時のパーティクル生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateJustParryParticle(const LWP::Math::Vector3& pos);
	/// <summary>
	/// 弱パリィ時のパーティクル生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateGoodParryParticle(const LWP::Math::Vector3& pos);
	/// <summary>
	/// パリィ時のパーティクル生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateParryParticle(const LWP::Math::Vector3& pos);
	/// <summary>
	/// 回避時のパーティクル生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateEvasionParticle(const LWP::Math::Vector3& pos);
	/// <summary>
	/// 移動時のパーティクル生成
	/// </summary>
	/// <param name="pos"></param>
	void CreateMoveParticle(const LWP::Math::Vector3& pos);

private:
	Player* player_;
	FollowCamera* followCamera_;

private:
	LWP::Utility::JsonIO json_;

	// パリィ
	std::unique_ptr<ParryEffect> parryEffect_;
	// 回避
	std::unique_ptr<EvasionEffect> evasionEffect_;
	// 移動
	std::unique_ptr<MoveEffect> moveEffect_;



	std::unique_ptr<LargeFlashes> largeFlashes_;
	std::unique_ptr<ShortFlashes> shortFlashes_;
	std::unique_ptr<Rings> rings_;
	std::unique_ptr<Sparks> sparks_;
	// 浮遊パーティクル
	std::unique_ptr<FloatParticle> floatParticle_;



	// パーティクル生成座標(デバッグ用)
	LWP::Math::Vector3 debugEmitterPos_;

	bool isPreJustParry_ = false;
	bool isPreGoodParry_ = false;
};

