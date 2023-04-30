#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define ERROR (-1)
#define STANDARD_ERROR_FD 2

/**
 * An "strlen" like function to get the value of a string in chars.
 * @param str The string to evaluate.
 * @return The number of chars in the string.
 */
int strLength(const char *str) {
    int count = 0;
    const char *c = str;
    while (*c != '\0') {
        count++;
        c++;
    }
    return count;
}

/**
 * A "perror" like function to write to the Standard error (diagnostic) output.
 * @param errorMessage The message to print.
 */
void errorPrint(char *errorMessage) {
    int charNum = strLength(errorMessage);
    write(STANDARD_ERROR_FD, errorMessage, charNum * sizeof(char));
}

/**
 * Checking if the amount of arguments are valid to 3.
 * @param argc the amount of arguments the program received.
 * @return 0 if the value isn't 3, 1 otherwise.
 */
int argNumCheck(int argc) {
    if (argc != 3) {
        errorPrint("NOT ENOUGH ARGUMENTS!");
        return 0;
    }
    return 1;
}

/**
 * This functions is trying to open a file.
 * @param filePath The path to the file.
 * @return The file descriptor number of the file if succeeded, -1 otherwise.
 */
int openFile(char *filePath) {
    int fd;
    // If the file opened successfully, return its file descriptor number.
    if ((fd = open(filePath, O_RDONLY)) != ERROR) {
        return fd;
    }
    // If the path wasn't a valid path, return an error.
    errorPrint("NOT A VALID PATH!");
    return ERROR;
}

/**
 * Reading both files and comparing char by char if the files are similar, identical or different.
 * @param fd1 The file descriptor of the first file.
 * @param fd2 The file descriptor of the second file.
 * @return 1 for identical, 3 for similar and 2 otherwise.
 */
int readAndCompare(int fd1, int fd2) {

}


/**
 * The main function of the program.
 * @param argc the amount of arguments the system receive, should be exactly 3.
 * @param argv the array of chars containing the arguments. index 0 should contain the name of the program,
 * index 1 a path to the first file and index 2 a path to the second file.
 * @return 1 if the files are identical in content, 3 if they are similar (different only in \n or capital letters)
 * or 2 otherwise.
 */
int main(int argc, char *argv[]) {
    // Check if the amount of arguments is valid.
    if (!argNumCheck(argc)) {
        return ERROR;
    }
    // Create two integers for the file's file descriptors.
    int fd1, fd2;
    // Open the files.
    if ((fd1 = openFile(argv[1])) == ERROR || (fd2 = openFile(argv[2])) == ERROR) {
        // If the opening failed, return an error.
        return ERROR;
    }


    return 0;
}
