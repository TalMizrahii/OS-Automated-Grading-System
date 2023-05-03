// Tal Mizrahi 206960890

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define ERROR (-1)
#define STANDARD_ERROR_FD 2
#define IDENTICAL 1
#define SIMILAR 3
#define NON_EQUAL 2


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
 * Closing two files.
 * @param fd1 The first file to close.
 * @param fd2 The second file to close.
 */
void closeFiles(int fd1, int fd2) {
    if (close(fd1) <= ERROR || close(fd2) <= ERROR) {
        errorPrint("Error in: close\n");
        exit(-1);
    }
}

/**
 * This functions is trying to open a file.
 * @param filePath The path to the file.
 * @return The file descriptor number of the file if succeeded, -1 otherwise.
 */
int openFile(char *filePath) {
    int fd;
    // If the file opened successfully, return its file descriptor number.
    if ((fd = open(filePath, O_RDONLY)) <= ERROR) {
        // If the path wasn't a valid path, print an error.
        errorPrint("Error in: open\n");
        exit(-1);
    }
    return fd;
}

/**
 * Checking if a char is a space.
 * @param ch The char to check.
 * @return 1 if it is, 0 otherwise.
 */
int isSpace(char ch) {
    if (ch == ' ' || ch == '\n' || ch == '\r') {
        return 1;
    }
    return 0;
}

/**
 * Checking if a character is lower case, and if it is making it upper case. otherwise it return untouched.
 * @param ch The char to check.
 * @return The character as upper case or untouched.
 */
char upper(char ch) {
    if (ch >= 65 && ch <= 90) {
        ch = (char) (ch + 32);
    }
    return ch;
}

/**
 * Reading one byte from a file.
 * @param fd the file descriptor number of the file.
 * @param ch the pointer to the char to store the result.
 * @return the return value of the read system call.
 */
long readByteFile(int fd, char *ch) {
    long retVal;
    // If the reading was successful, return the return value of read.
    if ((retVal = read(fd, ch, 1)) > ERROR) {
        return retVal;
    }
    // Otherwise, exit.
    errorPrint("Error in: read\n");
    exit(-1);
}

/**
 * Checking if both filed are similar.
 * @param fd1 The file descriptor number of the first file.
 * @param fd2 The file descriptor number of the second file.
 * @return 1 if the files are *at least* similar, 0 otherwise.
 */
int similar(int fd1, int fd2) {
    // Creating chars to store the reading from the files.
    char ch1 = 0, ch2 = 0;
    // Storing the return values of the reading byte from a file function.
    long x1, x2;
    do {
        // Reading byte from each file.
        x1 = readByteFile(fd1, &ch1);
        x2 = readByteFile(fd2, &ch2);
        // If the files ended, break the loop.
        if (x1 == 0 && x2 == 0) {
            break;
        }
        // If the character that was read is a space, continue reading.
        while (isSpace(ch1) && readByteFile(fd1, &ch1));
        while (isSpace(ch2) && readByteFile(fd2, &ch2));
        // If the characters are not just similar, break the loop.
        if (upper(ch1) != upper(ch2)) {
            break;
        }
    } while (ch1 != EOF && ch2 != EOF);
    // Close the files we used.
    closeFiles(fd1, fd2);
    // If the loop ended and the files are both 0, the files are similar.
    if (x1 == 0 && x2 == 0) {
        return SIMILAR;
    }
    // If the loop ended but the files not, the files are not similar.
    return 0;
}

/**
 * Checking if both filed are identical.
 * @param fd1 The file descriptor number of the first file.
 * @param fd2 The file descriptor number of the second file.
 * @return 1 if the files are identical, 0 otherwise.
 **/
int identical(int fd1, int fd2) {
    // Creating chars to store the reading from the files.
    char ch1, ch2;
    // Return values from the read.
    long x1, x2;
    while (1) {
        // Read from the files one byte.
        x1 = readByteFile(fd1, &ch1);
        x2 = readByteFile(fd2, &ch2);
        if (x1 == 0 && x2 == 0) {
            closeFiles(fd1, fd2);
            return IDENTICAL;
        }
        if ((x1 == 0) || (x2 == 0) || (ch1 != ch2)) {
            closeFiles(fd1, fd2);
            return 0;
        }
    }
}

/**
 * Given two files, open them and store the file descriptor numbers in fd1 and fd2.
 * @param fd1 The file descriptor of the first file.
 * @param fd2 The file descriptor of the second file.
 * @param file1 The path to the first file.
 * @param file2 The path to the second file.
 */
void openFiles(int *fd1, int *fd2, char *file1, char *file2) {
    if ((*fd1 = openFile(file1)) <= ERROR || (*fd2 = openFile(file2)) <= ERROR) {
        // If the opening failed, return an error.
        exit(-1);
    }
}

/**
 * Checking if the amount of arguments are valid to 3.
 * @param argc the amount of arguments the program received.
 * @return If the value isn't 3 the program exits, 1 otherwise.
 */
int argNumCheck(int argc) {
    if (argc != 3) {
        write(1, "NOT ENOUGH ARGUMENTS!\n", 22);
        exit(-1);
    }
    return 1;
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
    argNumCheck(argc);
    // Create two integers for the file's file descriptors.
    int fd1, fd2;
    // Open the files.
    openFiles(&fd1, &fd2, argv[1], argv[2]);
    // Check if the files are identical.
    if (identical(fd1, fd2)) {
        return IDENTICAL;
    }
    // Check if the files are similar.
    openFiles(&fd1, &fd2, argv[1], argv[2]);
    if (similar(fd1, fd2)) {
        return SIMILAR;
    }
    // If the files are not identical or similar, return 2.
    return NON_EQUAL;
}
