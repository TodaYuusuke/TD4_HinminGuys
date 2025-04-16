#include <Adapter.h>
#include "Class/Scene/GameScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::StartUp(L"貧民Guys");
	LWP::System::Run(new GameScene);
	return 0;
}