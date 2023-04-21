#pragma once

#include <Core/Singleton.h>
#include <iostream>
#include <cstdarg>

#define HR(hr) Log::__log_hr(__FILE__, __LINE__, hr)
#define LOG_ERROR(x, ...) Log::__log_error(__FILE__, __LINE__, x, __VA_ARGS__)
#define LOG std::cout
#define ENDL std::endl

class Log
{
public:
	static void __log_hr(LPCSTR file, uint line, HRESULT hr);
	static void __log_error(LPCSTR file, int line, LPCSTR code, ...);

	static LPCSTR __get_fileline(LPCSTR filename, int line);
private:
	Log();
	static char filenameStr[256];
};