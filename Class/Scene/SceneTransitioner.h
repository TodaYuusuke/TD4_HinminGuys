#pragma once
#include "scene/IScene.h"

/// <summary>
/// シーンの名前
/// </summary>
enum class SceneName {
	kTitle, //タイトル
	kGameScene, //ゲームシーン
	kMax
};

/// <summary>
/// 簡易的な遷移管理クラス
/// </summary>
class SceneTransitioner
{
public:
	SceneTransitioner() = default;
	~SceneTransitioner() = default;

	//初期化
	void Initialize(IScene* scene);
	//更新
	void Update();
	//シーン遷移
	void SceneTransitionStart();
	//シーン切り替え中かどうか
	static bool GetIsSceneChange() { return isSceneChangeStart_ or isSceneChangeEnd_; }
	//次のシーンをセット
	void SetNextScene(SceneName sceneName) { nextSceneName_ = sceneName; }

private:

	//切り替え演出開始
	void BeginTransitionEffect();
	//切り替え演出終了
	void EndTransitionEffect();
	//シーン切り替え
	void SceneTransition();

private:

	//シーンのポインタ
	IScene* scene_;
	//次のシーンの名前
	SceneName nextSceneName_;

	//シーン切り替えの始まりと終わり
	static bool isSceneChangeStart_;
	static bool isSceneChangeEnd_;
	//シーン切り替え時の時間管理変数
	static float sceneChangeCount_;
	//フェードインアウトに使う画像
	LWP::Primitive::Sprite sprite;
	//シーン切り替え速度
	float transitionSpeed_ = 1.0f;

};


