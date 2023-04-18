#pragma once

class Wnd
{
public:
	Wnd(Wnd* parent, int x, int y, int width, int height);
	virtual ~Wnd();

	//create
	bool Create();

	//virtual
	virtual void OnCreateEvent();
	virtual void OnResizeEvent(int w, int h);
	virtual LRESULT LocalWndProc(UINT msg, WPARAM wp, LPARAM lp);

	//static
	static LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	
	//execution
	int Exec();

	operator HWND();
	union {
		struct { int x, y, width, height; } geo;
		int raw_geo[4];
	};
private:
	Wnd* mParent;
	HWND mHwnd;
};