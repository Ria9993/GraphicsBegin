#pragma once

#include <Core/Singleton.h>

#define HR(hr) Log::__log_hr(hr)

class Log
{
public:
	static void __log_hr(HRESULT hr);
private:
	Log();
	static char tempStr[256];
};