#pragma once
#include <Wnd/Wnd.h>
#include <Graphics/DX11Context1.h>

class WndFrame : public Wnd
{
public:
	WndFrame(Wnd* parent, int w, int h);
	
	//virtual
	void OnCreateEvent() override;
	void OnResizeEvent(int w, int h) override;

	int ExecFrame(Scene* scene);

	std::shared_ptr<DX11Context1> mGraphics;

	LRESULT LocalWndProc(UINT msg, WPARAM wp, LPARAM lp) override;
};