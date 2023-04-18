#include <PCH.h>
#include <Core/Log.h>

char Log::tempStr[256] = {};

void Log::__log_hr(HRESULT hr)
{
	if (FAILED(hr))
	{
		char* buffer;
		FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER
			| FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&buffer, 0, NULL);
		LocalFree(buffer);
		char bufferExt[256] = {};
		sprintf_s(bufferExt, "%s\n%s\n", tempStr, buffer);
		MessageBox(NULL, bufferExt, "HRESULT Error", MB_OK | MB_ICONERROR);
	}
}
