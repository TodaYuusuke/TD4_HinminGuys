#pragma once
#include <Adapter.h>

// 質点
struct PointMass {
	LWP::Resource::RigidModel model;
	LWP::Math::Vector3 velocity;
	LWP::Math::Vector3 euler;
	LWP::Math::Vector3 acceleration;
	LWP::Math::Vector3 force;       // 積算された外力
	float mass = 1.0f;				// 質量
	bool isFixed = false;			// 固定された点（例えば最上部）
};
// ばね
struct Spring {
	int p1, p2;          // 接続されている2点のインデックス
	float restLength;    // ばねの自然長
	float stiffness;     // ばね定数
	float damping;       // 減衰
};

class ChainNode {
public:
	// コンストラクタ
	ChainNode(const int& springConectPoint1, const int& springConectPoint2);
	ChainNode(const Spring& spring);
	// デストラクタ
	~ChainNode() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	void InverseUpdate();

	/// <summary>
	/// jsonで保存する値を設定
	/// </summary>
	void CreateJsonData();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

private:// 数学関数
	/// <summary>
	/// ばねの計算
	///	p1 の速度を求める
	/// </summary>
	LWP::Math::Vector3 CalcPointMassVel(PointMass p0, PointMass p1);
	LWP::Math::Vector3 CalcPointMassVel(PointMass p0, PointMass p1, LWP::Math::Vector3 p1Pos);

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// ばねの情報を取得
	/// </summary>
	Spring GetSpring() { return spring_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 質点リストを設定
	/// </summary>
	/// <param name="pointMass"></param>
	void SetPointMass(std::vector<PointMass>* pointMassList) { pointMassList_ = pointMassList; }
	/// <summary>
	/// ばねの情報を設定
	/// </summary>
	/// <param name="spring"></param>
	void SetSpring(const Spring& spring) { spring_ = spring; }
	/// <summary>
	/// 
	/// </summary>
	/// <param name="json"></param>
	void SetJsonData(LWP::Utility::JsonIO& json);
#pragma endregion

private:
	// 鎖モデル
	LWP::Resource::RigidModel model_;
	// 全ての質点リスト
	std::vector<PointMass>* pointMassList_;
	// ばね
	Spring spring_;
};