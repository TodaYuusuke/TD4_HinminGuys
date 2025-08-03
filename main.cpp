#include <Adapter.h>
#include "Class/Scene/GameScene.h"
#include "Class/Scene/Title.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {	
#ifdef _DEBUG
	LWP::System::Run<Title>(L"刀タナシス");
#else
	LWP::System::Run<Title>(L"刀タナシス");
#endif // _DEBUG
	return 0;
}