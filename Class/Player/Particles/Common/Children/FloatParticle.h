#pragma once
#include "Adapter.h"
#include "../../ParticleJsonDataStructs.h"

class Player;
/// <summary>
/// 汎用パーティクル
/// 地面から浮いて時間経過で落下を開始するパーティクル
/// </summary>
class FloatParticle : public LWP::Object::Particle {
public:
	struct FloatParticleData {
		float height;				// 浮く高さ
		float amplitude;			// 振幅
		float currentFrame;
	};
	struct FloatParticleJsonData {
		LimitF posLimit;
		LimitF scaleLimit;
		LimitF heightLimit;
		LimitF amplitudeLimit;
		float acceleration;
		float multiply = 1.0f;
		float maxFloatTime;					// 浮き終わる時間
		float maxDropingTime;				// 落下終了時間
	};

public:
	// コンストラクタ
	FloatParticle(Player* player);
	// デストラクタ
	~FloatParticle() = default;

	/// <summary>
	/// パーティクルを１つ生成する度に呼ばれる関数
	/// </summary>
	/// <param name="newData">生成された実体の参照（mに基準となるmodelは既に代入されている）</param>
	/// <returns></returns>
	void Generate(LWP::Object::Particle::Data& data) override;
	/// <summary>
	/// パーティクルの更新処理
	/// </summary>
	/// <param name="data">更新する実態の参照</param>
	/// <returns></returns>
	bool UpdateParticle(LWP::Object::Particle::Data& data) override;

	/// <summary>
	/// 浮遊開始状態の処理
	/// </summary>
	/// <param name="data"></param>
	void Floating(LWP::Object::Particle::Data& data);
	/// <summary>
	/// 滞空状態の処理
	/// </summary>
	/// <param name="data"></param>
	void Hover(LWP::Object::Particle::Data& data);
	/// <summary>
	/// 落下状態の処理
	/// </summary>
	/// <param name="data"></param>
	void Droping(LWP::Object::Particle::Data& data);
	/// <summary>
	/// 状態の更新
	/// </summary>
	void StateUpdate(LWP::Object::Particle::Data& data);

	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

public:
#pragma region Getter
	/// <summary>
	/// 落下状態かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDrop() { return isStartDrop_; }
	/// <summary>
	/// 落下状態かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsHover() { return isHover_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// JSONデータの調整項目を設定
	/// </summary>
	/// <param name="jsonData"></param>
	void SetJsonData(LWP::Utility::JsonIO& json);
	/// <summary>
	/// 落下状態かを設定
	/// </summary>
	/// <param name="isDrop"></param>
	void SetIsDrop(const bool& isDrop) { isStartDrop_ = isDrop; }
#pragma endregion

private:// 外部からアドレスを受け取る変数
	Player* player_;

private:
	std::map<int, FloatParticleData> datas_;
	FloatParticleJsonData jsonData_;

	// 落下開始
	bool isStartDrop_ = false;
	bool isHover_ = false;
};