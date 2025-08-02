#include <Adapter.h>
#include "Class/Scene/GameScene.h"
#include "Class/Scene/Title.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {	
#ifdef _DEBUG
	LWP::System::Run<Title>(L"貧民Guys");
#else
	LWP::System::Run<Title>(L"貧民Guys");
#endif // _DEBUG
	return 0;
}