// Tal Mizrahi 206960890

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define ERROR (-1)
#define STANDARD_ERROR_FD 2
#define MAX_PATH 150
#define ONE_BYTE 1

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
int openFilePath(char *pathToFile, int flag) {
    int fd = open(pathToFile, flag);
    if (fd <= ERROR) {
        errorPrint("Error in: open\n");
        exit(1);
    }
    return fd;
}

/**
 * Reading a line from the configuration file, assuming each line ends with '\n' character.
 * @param line The line to read to for storing.
 * @param fd The file descriptor number of the configuration file.
 */
void readToLine(char *line, int fd) {
    // Initiate a char and read one byte from the file.
    char ch;
    read(fd, &ch, ONE_BYTE);
    // While the char isn't a new line char or EOF, concatenate the char to the line string.
    while (ch != '\n' && ch != EOF) {
        strcat(line, &ch);
        read(fd, &ch, ONE_BYTE);
    }
}

/**
 * A control flow function to read the configuration file by calling readToLine three times.
 * @param line1 The first line in the file contains the path to the users directory.
 * @param line2 The second line of the file contains the path to the input file.
 * @param line3 The third path to the file contais the path to the
 * @param fd
 */
void readConfiguration(char *line1, char *line2, char *line3, int fd) {
    readToLine(line1, fd);
    readToLine(line2, fd);
    readToLine(line3, fd);
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
    // Open the configuration file.
    int confFd = openFilePath(argv[1], O_RDONLY);
    // Initiate three strings, all 0, to store the lines from the configuration file.
    char folderPath[MAX_PATH] = {0};
    char inputFilePath[MAX_PATH] = {0};
    char outputFilePath[MAX_PATH] = {0};
    // Get the lines from the configuration file.
    readConfiguration(folderPath, inputFilePath, outputFilePath, confFd);
    // Open the output file using the path we extracted from the configuration file.
    int outputFd = openFilePath(outputFilePath, O_RDONLY);

}