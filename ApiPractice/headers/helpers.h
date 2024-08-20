#include <Windows.h>
#include <stdio.h>

// SeDebugPrivilege Implementation
// https://github.com/tbhaxor/WinAPI-RedBlue/blob/main/Process%20Listing/WTS%20Api/pch.h
BOOL AddSeDebugPrivileges() {
	// Get the current process handle
	DWORD dwPid = GetCurrentProcessId();
	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
	if (hProc == NULL) {
		wprintf(L"OpenProcess()");
		return FALSE;
	}

	// Get the token handle with query information and adjust privileges access
	HANDLE hTok = INVALID_HANDLE_VALUE;
	if (!OpenProcessToken(hProc, TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hTok)) {
		wprintf(L"OpenProcessToken()");
		return FALSE;
	} else if(hTok == NULL || hTok == INVALID_HANDLE_VALUE) {
		wprintf(L"OpenProcessToken()");
		return FALSE;
	}

	// Get the value of SeDebugPrivilege from text
	LUID pDebugPriv;
	if (!LookupPrivilegeValueA(NULL, "SeDebugPrivilege", &pDebugPriv)) {
		wprintf(L"LookupPrivilegeValueA()");
		return FALSE;
	}

	// Adjust token privilege 
	TOKEN_PRIVILEGES tokPrivs;
	tokPrivs.PrivilegeCount = 1;
	tokPrivs.Privileges[0].Luid = pDebugPriv;
	tokPrivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hTok, FALSE, &tokPrivs, 0, NULL, NULL)) {
		wprintf(L"AdjustTokenPrivileges()");
		return FALSE;
	}

	// Query token privileges to confirm whether 
	BOOL bRes;
	PRIVILEGE_SET tokPrivSet;
	tokPrivSet.Control = PRIVILEGE_SET_ALL_NECESSARY;
	tokPrivSet.PrivilegeCount = 1;
	tokPrivSet.Privilege[0].Luid = pDebugPriv;
	if (!PrivilegeCheck(hTok, &tokPrivSet, &bRes)) {
		wprintf(L"PrivilegeCheck()");
		return FALSE;
	}

	CloseHandle(hProc);
	CloseHandle(hTok);
	hProc = NULL;
	hTok = NULL;
	
	return bRes;
}