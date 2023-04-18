#include <PCH.h>
#include <Wnd/WndFrame.h>

WndFrame::WndFrame(Wnd* parent, int w, int h)
	: Wnd(parent, 0, 0, w, h)
{
	geo.x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
	geo.y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;
	Create();
}

void WndFrame::OnCreateEvent()
{
	printf("WM_CREATE\n");

	RECT rc{};
	GetClientRect(*this, &rc);
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;

	mGraphics = std::make_shared<DX11Context>(w, h, *this);
}

void WndFrame::OnResizeEvent(int w, int h)
{
	printf("WM_RESIZE %d %d\n", w, h);
	//TODO : we must to recreate swapchain
}

int WndFrame::ExecFrame(void* data)
{
	//TODO : data = Scene / Renderer
	MSG msg{};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//TODO : data->rendering
			if (mGraphics) {
				//printf("context alives\n");
				mGraphics->ClearBuffer(1.0f, 0.1f, 0.2f, 1.0f);
				//render

				mGraphics->SwapBuffer();
			}
		}
	}

	return (int)msg.lParam;
}

LRESULT WndFrame::LocalWndProc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		OnCreateEvent();
		return 0;
	case WM_SIZE: {
		int cx = LOWORD(lp);
		int cy = HIWORD(lp);
		OnResizeEvent(cx, cy);
	}break;
	case WM_PAINT:

		break;
	default:
		break;
	}
	return DefWindowProc(*this, msg, wp, lp);
}
