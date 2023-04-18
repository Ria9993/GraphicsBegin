#include <PCH.h>
#include <Wnd\Wnd.h>

Wnd::Wnd(Wnd* parent, int x, int y, int width, int height)
	: mParent(parent),
	geo{x, y, width, height},
	mHwnd(NULL)
{
}

Wnd::~Wnd()
{
	DestroyWindow(mHwnd);
}

bool Wnd::Create()
{
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = "Graphics++";

	//check pre register class exist
	if (!GetClassInfoEx(wc.hInstance, wc.lpszClassName, &wc))
	{
		wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
		wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
		wc.lpfnWndProc = Wnd::WndProc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		//regist class
		if (!RegisterClassEx(&wc))
			MessageBox(nullptr, "failed toa create window class", "error", MB_OK);
	}

	HWND parentHwnd = mParent ? mParent->mHwnd : nullptr;
	DWORD style = WS_VISIBLE;
	if (parentHwnd != nullptr)
		style |= WS_CHILD;
	else
		style |= WS_OVERLAPPEDWINDOW;

	mHwnd = CreateWindowEx(
		NULL,
		wc.lpszClassName,
		wc.lpszClassName,
		style,
		geo.x, geo.y, geo.width, geo.height,
		parentHwnd, (HMENU)0, wc.hInstance, 
		this //< GWLP_USERDATA
	);
	//CREATESTRUCT -> LPARAM

	if (mHwnd == 0)
	{
		MessageBox(nullptr, "failed to create window", "error", MB_OK);
		return false;
	}

	return true;
}

void Wnd::OnCreateEvent()
{
}

void Wnd::OnResizeEvent(int w, int h)
{
}

LRESULT Wnd::LocalWndProc(UINT msg, WPARAM wp, LPARAM lp)
{
	return DefWindowProc(*this, msg, wp, lp);
}

LRESULT WINAPI Wnd::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	//it must be nullptr if we don't registered
	Wnd* currentWnd = (Wnd*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (msg == WM_NCCREATE) //< Non Client Create
	{
		CREATESTRUCT* cs = (LPCREATESTRUCT)lp;
		currentWnd = (Wnd*)cs->lpCreateParams;
		currentWnd->mHwnd = hwnd; //< ????????????????
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)currentWnd);
	}

	// after MW_NCCREATE
	if (currentWnd)
	{
		return currentWnd->LocalWndProc(msg, wp, lp);
	}

	return DefWindowProc(hwnd, msg, wp, lp);
}

int Wnd::Exec()
{
	MSG msg{};

	while (msg.message != WM_QUIT)
	{
		//PeekMessage; //< nonblocking
		if (GetMessage(&msg, NULL, 0, 0)) //< blocking
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.lParam;
}

Wnd::operator HWND()
{
	return mHwnd;
}
