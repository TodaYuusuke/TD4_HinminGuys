#pragma once
#include <Adapter.h>

class UIMotions {
public:
	struct UIMotionData {
		LWP::Math::Vector3* targetValue_vec3;// アニメーション対象の値を入れる(Vector3)
		LWP::Math::Vector2* targetValue_vec2;// アニメーション対象の値を入れる(Vector2)
		float* targetValue_float;			 // アニメーション対象の値を入れる(float)
		int* targetValue_int;				 // アニメーション対象の値を入れる(int)
		LWP::Resource::Motion moation;		 // アニメーションクラス
		std::string name;					 // 名前
	};

public:
	// コンストラクタ
	UIMotions() = default;
	// デストラクタ
	~UIMotions() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

private:
	// 作成したアニメーションリスト
	std::map<std::string, LWP::Resource::Motion> animations_;
};