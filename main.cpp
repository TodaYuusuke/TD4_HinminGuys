#include <Adapter.h>
#include "Class/Scene/GameOver.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {	
	LWP::System::Run<GameOver>(L"貧民Guys");
	return 0;
}