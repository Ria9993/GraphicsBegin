#include <PCH.h>
//#include <Wnd/Wnd.h>
#include <Wnd/WndFrame.h>
#include <Core/Log.h>

int main(int argc, char** argv)
{
	WndFrame wnd(nullptr, 500, 500);
	//Wnd child(&wnd, 300, 400);

	//Log::__log_hr(E_NOINTERFACE);

	return wnd.ExecFrame(NULL);
}