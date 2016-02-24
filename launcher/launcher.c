#include "windows.h"

void CALLBACK startW(HWND hwnd, HINSTANCE hinst, LPWSTR lpszCmdLine, int nCmdShow)
{
	HANDLE token;
	HANDLE newToken;
	PROCESS_INFORMATION procInfo;
	STARTUPINFO startInfo;

	RtlZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));
	RtlZeroMemory(&startInfo, sizeof(STARTUPINFO));
	startInfo.cb = sizeof(STARTUPINFO);

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ASSIGN_PRIMARY | TOKEN_DUPLICATE | TOKEN_QUERY, &token))
	{
		if (CreateRestrictedToken(token, SANDBOX_INERT, 0, NULL, 0, NULL, 0, NULL, &newToken))
		{
			if (CreateProcessAsUser(newToken, NULL, lpszCmdLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startInfo, &procInfo))
			{
				CloseHandle(procInfo.hThread);
				CloseHandle(procInfo.hProcess);
			}
			CloseHandle(newToken);
		}
		CloseHandle(token);
	}
}