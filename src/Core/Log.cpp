#include <PCH.h>
#include <Core/Log.h>

char Log::filenameStr[256] = {};

void Log::__log_hr(LPCSTR file, uint line, HRESULT hr)
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
		sprintf_s(bufferExt, "%s\n%s\n", __get_fileline(file, line), buffer);
		MessageBox(NULL, bufferExt, "HRESULT Error", MB_OK | MB_ICONERROR);
	}
}

void Log::__log_error(LPCSTR file, int line, LPCSTR code, ...)
{
	va_list ap;
	va_start(ap, code);
	CHAR codeBuffer[128];
	vsnprintf(codeBuffer, 128, code, ap);
	va_end(ap);
	CHAR buffer[256];
	sprintf_s(buffer, "%s\n%s", codeBuffer, __get_fileline(file, line));

	uint res = MessageBox(NULL, buffer, "Error", MB_OKCANCEL | MB_ICONERROR);
	if (res == IDOK) ExitProcess(0);
}

LPCSTR Log::__get_fileline(LPCSTR filename, int line)
{
	memset(filenameStr, 0, sizeof(filenameStr));
	filename = (strrchr(filename, '\\') + 1);
	sprintf_s(filenameStr, "file : %s\nline : %d\n", filename, line);
	return filenameStr;
}
