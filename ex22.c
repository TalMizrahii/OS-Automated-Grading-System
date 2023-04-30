// Tal Mizrahi 206960890

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
 * @return If the value isn't 3 the program exits, 1 otherwise.
 */
int argNumCheck(int argc) {
    if (argc != 2) {
        errorPrint("NOT ENOUGH ARGUMENTS!");
        exit(-1);
    }
    return 1;
}

/**
 * The function receive a path to a file an opens it with the "open" system call.
 * If the opening failed, the program close.
 * @param pathToFile The path to the file.
 * @return The value to the file descriptor of the file.
 */
int openFile(char *pathToFile){
    int fd = open(pathToFile, O_RDONLY);
    if(fd < 0){

    }

}

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    // Check if the amount of arguments is valid.
    argNumCheck(argc);

}