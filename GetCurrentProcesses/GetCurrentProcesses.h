#include <Windows.h>
#include <stdio.h>
#include <psapi.h>
#include <tlhelp32.h>

#include "..\helpers\helpers.h"

// Getting process list using CreateToolhelp32Snapshot: https://stackoverflow.com/questions/54729026/windows-enumprocesses-some-process-names-as-unknown
// Getting token/user information: https://stackoverflow.com/questions/37002790/gettokeninformation-token-owner-and-lookupaccountsida
int GetCurrentProcesses()
{

    printf("[*] Attempting to add SeDebugPrivilege...\n");
    BOOL privilegeAdded = AddSeDebugPrivileges();

    if (!privilegeAdded)
    {
      printf("[-] Error adding SeDebugPrivilege, continuing as unprivileged.\n");
    }
    else {
      printf("[+] Successfully enabled SeDebugPrivilege!\n\n");
    }

    printf("%-8s\t%-38s\t%s\n", "PID", "Process Name", "Username");
    printf("-----------------------------------------------------------------\n");
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
                  printf("OpenProcessToken failed, error %d\n", dwError);
                  //return 0;
              }

              if (!GetTokenInformation(hToken, TokenOwner, NULL, 0, &len))
              {
                  dwError = GetLastError();
                  if (dwError != ERROR_INSUFFICIENT_BUFFER) 
                  {
                      printf("GetTokenInformation failed, error %d\n", dwError);
                      CloseHandle(hToken);
                      //return 0;
                  }
              }

              PTOKEN_OWNER to = (PTOKEN_OWNER) LocalAlloc(LPTR, len);
              if (!to)
              {
                  dwError = GetLastError();
                  printf("LocalAlloc failed, error %d\n", dwError);
                  CloseHandle(hToken);
                  //return 0;
              }

              if (!GetTokenInformation(hToken, TokenOwner, to, len, &len))
              {
                  dwError = GetLastError();
                  printf("GetTokenInformation (2nd) failed, error %d\n", dwError);
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
                  printf("LookupAccountSid failed, error %d\n", dwError);
                  LocalFree(to);
                  CloseHandle(hToken);
                  //return 0;
              }

              printf("%8d\t%-38S\t%s\n", process.th32ProcessID, process.szExeFile, nameUser);
            }
            else {
              printf("%8d\t%-38S\n", process.th32ProcessID, process.szExeFile);
            }
            
            

            
        } while(Process32Next(hndl, &process));

        CloseHandle(hndl);
    }
}
