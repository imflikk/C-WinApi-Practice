#include "all_headers.h"


// Create function to print help menu
void PrintHelpMenu() {
    printf("Choose an option:\n");
    printf("1. Get current processes\n");
    printf("2. Get current directory\n");
    printf("3. Get directory contents\n");
    printf("4. Get local users\n");
    printf("5. Log keys\n");
    printf("6. Get environment variables\n");
    printf("7. Read from file\n");
    printf("8. Write to file\n");
    printf("9. Download file over HTTP\n");
    printf("10. Get content from URL over HTTP\n");
    printf(">> ");
}

int main()
{
    
    // Print help menu
    PrintHelpMenu();

    int helpOption;
    scanf("%d", &helpOption);

    // Define variables for various options
    char* urlContent;
    char fileName[100];
    char contents[1000];

    switch (helpOption) {
		case 1:
			GetCurrentProcesses();
			break;
		case 2:
			GetDirectory();
			break;
		case 3:
			GetDirectoryContents();
			break;
		case 4:
			GetLocalUsers();
			break;
		case 5:
			Log_Keys();
			break;
		case 6:
			GetEnvironVars();
			break;
		case 7:
			// Read from file
            // Get file name to read from user
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
			break;
		case 8:
			// Write to file
            printf("Enter the name of the file to write to: ");
            scanf("%s", fileName);

            // Check length of file name
            if (strlen(fileName) > 100) {
                printf("File name too long. Exiting...\n");
                return 1;
            }

            // Get contents to write to file, including spaces
            printf("Enter the contents to write to the file: ");
			scanf(" %[^\n]s", contents);

            // Check length of contents
            if (strlen(contents) > 1000) {
                printf("Contents too long. Exiting...\n");
                return 1;
            }

            WriteToFile(fileName, contents);
			break;
        case 9:
            // Download file over HTTP
            DownloadToFile("http://192.168.50.193:8000/httptest", "C:\\Users\\flikk\\desktop\\httptest-file.txt");
            break;
        case 10:
            // Get content from URL over HTTP
            urlContent = GetContentFromURL("http://192.168.50.193:8000/httptest");
            printf("URL content: %s", urlContent);
            break;
        default:
            printf("Invalid option. Exiting...\n");
            return 1;
    }


    return 0;
    
}

