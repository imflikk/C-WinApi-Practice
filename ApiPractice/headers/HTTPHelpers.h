#include <windows.h>
#include <wininet.h>
#include <urlmon.h>
#include <stdio.h>
#include <curl/curl.h>

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

bool DownloadToFileWithCurl(char* url, char* filePath) {
    // Setup curl options and download file from given URL
    CURL* curl;
    FILE* fp;
    CURLcode res;

    curl = curl_easy_init();

    if (curl) {
        fp = fopen(filePath, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);

        if (res == CURLE_OK) {
            printf("File downloaded successfully to %s\n", filePath);
            return true;
        }
        else {
            printf("Failed to download file. Error: %s\n", curl_easy_strerror(res));
            return false;
        }
    } else {
		printf("Failed to initialize curl\n");
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

// Callback function to write data to a buffer
// Reference: https://stackoverflow.com/questions/27007379/how-do-i-get-response-value-using-curl-in-c
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    char** contentPtr = (char**)userp;

    // Reallocate memory to accommodate the new data
    char* newContent = (char*)realloc(*contentPtr, strlen(*contentPtr) + totalSize + 1);
    if (newContent == NULL) {
        printf("Memory allocation failed\n");
        return 0;
    }

    // Append the new data to the buffer
    *contentPtr = newContent;
    strncat(*contentPtr, (char*)contents, totalSize);

    return totalSize;
}

char* GetContentFromURLWithCurl(const char* url) {
    CURL* curl;
    CURLcode res;
    char* content = (char*)malloc(1); // Initial allocation
    content[0] = '\0'; // Null-terminate the initial empty string

    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            return content;
        }
        else {
            printf("Failed to download content. Error: %s\n", curl_easy_strerror(res));
            free(content);
            return NULL;
        }
    }
    else {
        printf("Failed to initialize curl\n");
        free(content);
        return NULL;
    }
}