#pragma once
#include <stdio.h>
#include <corecrt_malloc.h>


char* GetFileContents(const char* fileName) {
	FILE* fptr;

	// Open a file in read mode
	fptr = fopen(fileName, "r");

	// If file does not exist, return NULL
	if (fptr == NULL) {
		printf("Cannot open file \n");
		return NULL;
	}

	// Read contents from file
	char* fileContentsBuffer = (char*)malloc(1000);

	// fread reads bytes past the null terminator, so use fgets instead for regular text files
	//fread(fileContentsBuffer, 1000, 1, fptr);
	fgets(fileContentsBuffer, 1000, fptr);

	// Close the file
	fclose(fptr);

	return fileContentsBuffer;
}

void WriteToFile(const char* fileName, const char* contents) {
	FILE* fptr;

	// Open a file in write mode
	fptr = fopen(fileName, "w");

	// If file does not exist, return NULL
	if (fptr == NULL) {
		printf("Cannot open file \n");
		return;
	}

	// Write contents to file
	fprintf(fptr, "%s", contents);

	// Close the file
	fclose(fptr);
}