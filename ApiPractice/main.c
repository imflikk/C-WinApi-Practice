#include "all_headers.h"



int main()
{
    //GetCurrentProcesses();

    //GetDirectory();

    //GetDirectoryContents();

    //GetLocalUsers();

    //Log_Keys();

    //GetEnvironVars();

    // Get file name to read from user
    char fileName[100];
    printf("Enter the name of the file to read: ");
    scanf("%s", fileName);

    // Check length of file name
    if (strlen(fileName) > 100) {
		printf("File name too long. Exiting...\n");
		return 1;
	}

    char* fileContents = GetFileContents(fileName);
    printf("First 1000 characters of file %s: \n", fileName);
    printf("====================================\n");
    printf("%s", fileContents);

    return 0;
    
}