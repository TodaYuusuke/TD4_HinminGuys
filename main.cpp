#include <Adapter.h>
#include "Class/Scene/Title.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::StartUp(L"貧民Guys");
	LWP::System::Run(new Title);
	return 0;
}