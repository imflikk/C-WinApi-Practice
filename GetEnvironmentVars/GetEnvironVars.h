#include <windows.h>
#include <stdio.h>

// Individual variables: https://stackoverflow.com/questions/31906192/how-to-use-environment-variable-in-a-c-program
// All variables: https://stackoverflow.com/questions/16765545/how-to-list-all-environment-variables-in-a-c-c-app

void GetEnvironVars() {
    char* compName = getenv("COMPUTERNAME");
    char* userDomain = getenv("USERDOMAIN");
    char* userName = getenv("USERNAME");

    char* separator = "----------------------------------------------------------\n";

    printf("%s", separator);
    printf("[*] Example of specific environment variables:\n");
    printf("%s", separator);
    printf("\tUser %s\\%s logged into computer %s\n", userDomain, userName, compName);
    printf("%s", separator);

    extern char** environ;

    printf("[*] Example of all environment variables:\n");
    printf("%s", separator);
    for (char** current = environ; *current; current++) {
        printf("\t%s\n", * current);
    }
    printf("%s", separator);
}