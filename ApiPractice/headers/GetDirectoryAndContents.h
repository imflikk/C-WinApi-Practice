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

    printf("Current directory: %S\n", tszBuffer);

}

void GetDirectoryContents()
{
    TCHAR tszBuffer[MAX_PATH];
    DWORD dwRet;
    //char slash[3] = "\\*";

    dwRet = GetCurrentDirectory( MAX_PATH, tszBuffer );
    assert( dwRet != 0 );

    wprintf(separator);
    printf("[*] Current Directory: %S\n", tszBuffer);

    wcscat(tszBuffer, L"\\*");
    
    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError=0;


    hFind = FindFirstFile(tszBuffer, &ffd);

    if (INVALID_HANDLE_VALUE == hFind) 
    {
        printf("FindFirstFile error: %d", dwError);
    } 

    do
    {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            printf("%-16s %-30S\n", "<DIR>",ffd.cFileName);
        }
        else
        {
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;
            printf("\t%-8ld %-30S\n", filesize.QuadPart, ffd.cFileName);
        }
    }
    while (FindNextFile(hFind, &ffd) != 0);
    
    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES) 
    {
        printf("Some other error: %d", dwError);
    }

    wprintf(separator);

    FindClose(hFind);
    //return dwError;
}