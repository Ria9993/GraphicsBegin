#pragma once
#include <Wnd/Wnd.h>
#include <Graphics/DX11Context.h>

class WndFrame : public Wnd
{
public:
	WndFrame(Wnd* parent, int w, int h);
	
	//virtual
	void OnCreateEvent() override;
	void OnResizeEvent(int w, int h) override;

	int ExecFrame(void* data);

	std::shared_ptr<DX11Context> mGraphics;

	LRESULT LocalWndProc(UINT msg, WPARAM wp, LPARAM lp) override;
};