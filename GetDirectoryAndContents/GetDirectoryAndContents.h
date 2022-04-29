#include <Windows.h>
#include <stdio.h>
#include <assert.h>

// Listing files in directory: https://docs.microsoft.com/en-us/windows/win32/fileio/listing-the-files-in-a-directory

const wchar_t separator[67] = L"-----------------------------------------------------------------\n";

void GetDirectory()
{
    TCHAR tszBuffer[MAX_PATH];
    DWORD dwRet;
    char slash = '\\';

    dwRet = GetCurrentDirectory( MAX_PATH, tszBuffer );
    assert( dwRet != 0 );

    printf("Current directory: %s\n", tszBuffer);

}

void GetDirectoryContents()
{
    TCHAR tszBuffer[MAX_PATH];
    DWORD dwRet;
    char slash[3] = "\\*";

    dwRet = GetCurrentDirectory( MAX_PATH, tszBuffer );
    assert( dwRet != 0 );

    wprintf(separator);
    wprintf(L"[*] Current Directory: %s\n", tszBuffer);

    strcat(tszBuffer, slash);
    
    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError=0;


    hFind = FindFirstFile(tszBuffer, &ffd);

    if (INVALID_HANDLE_VALUE == hFind) 
    {
        wprintf(L"FindFirstFile error: %d", dwError);
    } 

    do
    {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            wprintf(L"%-16s %-30s\n", "<DIR>",ffd.cFileName);
        }
        else
        {
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;
            wprintf(L"\t%-8ld %-30s\n", filesize.QuadPart, ffd.cFileName);
        }
    }
    while (FindNextFile(hFind, &ffd) != 0);
    
    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES) 
    {
        wprintf(L"Some other error: %d", dwError);
    }

    wprintf(separator);

    FindClose(hFind);
    //return dwError;
}