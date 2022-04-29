#include <windows.h>
#include <stdio.h>
#include <Lm.h>

#pragma comment(lib, "netapi32.lib")

int GetLocalUsers()
{
    DWORD res, i=0;
    TCHAR lpServerName[255] = TEXT("");
    DWORD dwLevel = 1;
    DWORD dwIndex = 0;
    DWORD dwEntriesRequested = 100;
    DWORD dwMaxLength = MAX_PREFERRED_LENGTH;
    DWORD lpReturnedCount = 0;
    PNET_DISPLAY_USER pBuffer, p;

    printf("Local users\n");
    printf("-----------------------------\n");

    do
    {
        res = NetQueryDisplayInformation(lpServerName, dwLevel, dwIndex, dwEntriesRequested, dwMaxLength, &lpReturnedCount, (PVOID*) &pBuffer);

        if ((res == ERROR_SUCCESS) || (res == ERROR_MORE_DATA))
        {
            p = pBuffer;
            for (; lpReturnedCount > 0; lpReturnedCount--)
            {
                DWORD flags = p->usri1_flags;

                // Bitwise AND against 32 to check PASSWD_NOTREQD flag
                if (flags & 32)
                {
                    printf("%-20S %-20s\t - UserAccountFlags: %d\n", p->usri1_name, "(Password not required)", p->usri1_flags);
                }
                else {
                    printf("%-40S\t - UserAccountFlags: %d\n", p->usri1_name, p->usri1_flags);
                }

                

                p++;
            }
        }

    } while (res == ERROR_MORE_DATA); // end do

}