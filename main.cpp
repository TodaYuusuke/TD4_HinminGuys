#include <Adapter.h>
#include "Class/Scene/GameScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {	
	LWP::System::Run<GameScene>(L"貧民Guys");
	return 0;
}