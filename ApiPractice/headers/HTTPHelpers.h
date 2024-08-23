#include <windows.h>
#include <wininet.h>
#include <urlmon.h>
#include <stdio.h>

// Link with Urlmon.lib
#pragma comment(lib, "urlmon.lib")
// Link with wininet.lib
#pragma comment(lib, "wininet.lib")

// https://learn.microsoft.com/en-us/previous-versions/windows/internet-explorer/ie-developer/platform-apis/ms775123(v=vs.85)
bool DownloadToFile(char* url, char* filePath) {
    HRESULT hr;

    hr = URLDownloadToFileA(NULL, url, filePath, 0, NULL);
    if (SUCCEEDED(hr)) {
        printf("File downloaded successfully to %s\n", filePath);
        return true;
    }
    else {
        printf("Failed to download file. HRESULT: 0x%lx\n", hr);
        return false;
    }
}

// Reference for start: https://stackoverflow.com/questions/47486240/downloading-data-via-wininet
// Chatgpt helped modify this function to return the content
char* GetContentFromURL(const char* url) {
    HINTERNET hInternet = InternetOpenA("TestUserAgent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        printf("InternetOpen() failed. Error: %lu\n", GetLastError());
        return NULL;
    }

    HINTERNET hConnect = InternetOpenUrlA(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        printf("InternetOpenUrl() failed. Error: %lu\n", GetLastError());
        InternetCloseHandle(hInternet);
        return NULL;
    }

    char buffer[4096];
    DWORD bytesRead;
    BOOL bRead;
    size_t totalSize = 0;
    char* content = NULL;

    do {
        bRead = InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead);
        if (bRead && bytesRead > 0) {
            buffer[bytesRead] = '\0'; // Null-terminate the buffer
            size_t newSize = totalSize + bytesRead + 1;
            char* newContent = (char*)realloc(content, newSize);
            if (newContent == NULL) {
                printf("Memory allocation failed\n");
                free(content);
                InternetCloseHandle(hConnect);
                InternetCloseHandle(hInternet);
                return NULL;
            }
            content = newContent;
            memcpy(content + totalSize, buffer, bytesRead + 1);
            totalSize += bytesRead;
        }
    } while (bRead && bytesRead > 0);

    if (!bRead && GetLastError() != ERROR_SUCCESS) {
        printf("InternetReadFile() failed. Error: %lu\n", GetLastError());
        free(content);
        content = NULL;
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return content;
}