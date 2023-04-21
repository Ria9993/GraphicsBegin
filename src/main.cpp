#include <PCH.h>
//#include <Wnd/Wnd.h>
#include <Wnd/WndFrame.h>
#include <Core/Log.h>
#include <Scene/TestScene.h>

int main(int argc, char** argv)
{
	WndFrame wnd(nullptr, 500, 500);

	TestScene scene{};

	return wnd.ExecFrame(&scene);
}