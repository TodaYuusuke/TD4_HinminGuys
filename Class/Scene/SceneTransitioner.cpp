#include "SceneTransitioner.h"
#include "GameScene.h"
#include "Title.h"

bool SceneTransitioner::isSceneChangeStart_ = false;
bool SceneTransitioner::isSceneChangeEnd_ = false;
float SceneTransitioner::sceneChangeCount_ = 0.0f;

void SceneTransitioner::Initialize(IScene* scene)
{
	scene_ = scene;
	sprite.material.color = { 0.0f,0.0f,0.0f,0.0f };
	sprite.worldTF.scale = { LWP::Info::GetWindowWidthF() * 0.01f,LWP::Info::GetWindowHeightF() * 0.01f,1.0f};
}

void SceneTransitioner::Update()
{

	//シーン切り替え演出開始
	if (isSceneChangeStart_) {
		BeginTransitionEffect();
	}
	//シーン切り替え演出終了
	else if (isSceneChangeEnd_) {
		EndTransitionEffect();
	}

	sprite.material.color = { 0.0f,0.0f,0.0f,sceneChangeCount_ };

}

void SceneTransitioner::SceneTransitionStart()
{
	//シーン切り替えフラグ立てる
	isSceneChangeStart_ = true;
}

void SceneTransitioner::BeginTransitionEffect()
{

	//1までカウントを増やす
	if (sceneChangeCount_ < 1.0f) {

		sceneChangeCount_ += transitionSpeed_ * LWP::Info::GetDeltaTime();
		//1以上で終了
		if (sceneChangeCount_ >= 1.0f) {
			sceneChangeCount_ = 1.0f;
			isSceneChangeStart_ = false;
			isSceneChangeEnd_ = true;
			SceneTransition();
		}

	}

}

void SceneTransitioner::EndTransitionEffect()
{

	//0までカウントを減らす
	if (sceneChangeCount_ > 0.0f) {

		sceneChangeCount_ -= transitionSpeed_ * LWP::Info::GetDeltaTime();
		//0以下で終了
		if (sceneChangeCount_ <= 0.0f) {
			sceneChangeCount_ = 0.0f;
			isSceneChangeEnd_ = false;
		}

	}

}

void SceneTransitioner::SceneTransition()
{

	//シーンのポインタがある場合
	if (scene_) {

		//シーンの切り替え先を選んで切り替え
		switch (nextSceneName_)
		{
		case SceneName::kTitle:
			scene_->nextSceneFunction = []() { return new Title(); };
			break;
		case SceneName::kGameScene:
			scene_->nextSceneFunction = []() { return new GameScene(); };
			break;
		default:
			break;
		}

	}

}
