#include <Windows.h>
#include <stdio.h>
#include <psapi.h>
#include <tlhelp32.h>

// Getting process list using CreateToolhelp32Snapshot: https://stackoverflow.com/questions/54729026/windows-enumprocesses-some-process-names-as-unknown
// Getting token/user information: https://stackoverflow.com/questions/37002790/gettokeninformation-token-owner-and-lookupaccountsida
int main()
{

    wprintf(L"[*] Attempting to add SeDebugPrivilege...\n");
    BOOL privilegeAdded = AddSeDebugPrivileges();

    if (!privilegeAdded)
    {
      wprintf(L"[-] Error adding SeDebugPrivilege, continuing as unprivileged.\n");
    }
    else {
      wprintf(L"[+] Successfully enabled SeDebugPrivilege!\n\n");
    }

    wprintf(L"%-8s\t%-38s\t%s\n", "PID", "Process Name", "Username");
    wprintf(L"-----------------------------------------------------------------\n");
    HANDLE hndl = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPMODULE, 0);
    HANDLE hToken;
    DWORD len = 0;
    DWORD dwError;

    if(hndl)
    {
        PROCESSENTRY32 process = { sizeof(PROCESSENTRY32) };
        Process32First(hndl, &process);
        do
        {
            // Get a handle on the current process in the list
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, process.th32ProcessID);
            if (hProcess)
            {
              // Open current process token
              if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
              {
                  dwError = GetLastError();
                  wprintf(L"OpenProcessToken failed, error %d\n", dwError);
                  //return 0;
              }

              if (!GetTokenInformation(hToken, TokenOwner, NULL, 0, &len))
              {
                  dwError = GetLastError();
                  if (dwError != ERROR_INSUFFICIENT_BUFFER) 
                  {
                      wprintf(L"GetTokenInformation failed, error %d\n", dwError);
                      CloseHandle(hToken);
                      //return 0;
                  }
              }

              PTOKEN_OWNER to = (PTOKEN_OWNER) LocalAlloc(LPTR, len);
              if (!to)
              {
                  dwError = GetLastError();
                  wprintf(L"LocalAlloc failed, error %d\n", dwError);
                  CloseHandle(hToken);
                  //return 0;
              }

              if (!GetTokenInformation(hToken, TokenOwner, to, len, &len))
              {
                  dwError = GetLastError();
                  wprintf(L"GetTokenInformation (2nd) failed, error %d\n", dwError);
                  LocalFree(to);
                  CloseHandle(hToken);
                  //return 0;
              }

              char nameUser[256] = {0};
              char domainName[256] = {0};
              DWORD nameUserLen = 256;
              DWORD domainNameLen = 256;
              SID_NAME_USE snu;

              if (!LookupAccountSidA(NULL, to->Owner, nameUser, &nameUserLen, domainName, &domainNameLen, &snu))
              {
                  dwError = GetLastError();
                  wprintf(L"LookupAccountSid failed, error %d\n", dwError);
                  LocalFree(to);
                  CloseHandle(hToken);
                  //return 0;
              }

              wprintf(L"%8d\t%-38s\t%s\n", process.th32ProcessID, process.szExeFile, nameUser);
            }
            else {
              wprintf(L"%8d\t%-38s\n", process.th32ProcessID, process.szExeFile);
            }
            
            

            
        } while(Process32Next(hndl, &process));

        CloseHandle(hndl);
    }
}

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
	if (!AdjustTokenPrivileges(hTok, FALSE, &tokPrivs, NULL, NULL, NULL)) {
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