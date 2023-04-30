#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define ERROR (-1)
#define STANDARD_ERROR_FD 2

/**
 * Closing two files.
 * @param fd1 The first file to close.
 * @param fd2 The second file to close.
 */
void closeFiles(int fd1, int fd2) {
    close(fd1);
    close(fd2);
}

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
    if (argc != 3) {
        errorPrint("NOT ENOUGH ARGUMENTS!");
        exit(-1);
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
    if ((fd = open(filePath, O_RDONLY)) <= ERROR) {
        return fd;
    }
    // If the path wasn't a valid path, return an error.
    errorPrint("NOT A VALID PATH!");
    exit(-1);
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
    errorPrint("CAN NOT READ FILE!");
    exit(-1);
}

int isSpace(char ch) {
    if (ch == '\n' || ch == ' ' || ch == '\r') {
        return 1;
    }
    return 0;
}

int similarEnd(int fd, char ch) {
    do {
        if (isSpace(ch)) {
            continue;
        } else {
            return 2;
        }
    } while (readByteFile(fd, &ch));
    return 3;
}


int isLetter(char c1) {
    if ((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) {
        return 1;
    }
    return 0;
}


int unequal(int fd1, int fd2, char ch1, char ch2) {
    if(isSpace(ch1)){

    }

    if (isLetter(ch1) && isLetter(ch2)) {

    }
}


/**
 * Reading both files and comparing char by char if the files are similar, identical or different.
 * @param fd1 The file descriptor of the first file.
 * @param fd2 The file descriptor of the second file.
 * @return 1 for identical, 3 for similar and 2 otherwise.
 */
int readAndCompare(int fd1, int fd2) {
    // Creating chars to store the reading from the files.
    char ch1, ch2;
    // Return values from the read.
    long x1, x2;
    int similar = 0, identical = 1, different = 0;
    while (1) {
        // Read from the files one byte.
        x1 = readByteFile(fd1, &ch1);
        x2 = readByteFile(fd2, &ch2);
        // If both file ended, check only for similarity flag and return it.
        if (x1 == 0 && x2 == 0) {
            return similar ? 3 : 1;
        }
        if (x1 == 0) {
            return similarEnd(fd2, ch2);
        }
        if (x2 == 0) {
            return similarEnd(fd1, ch1);
        }
//        if(unequal())

    }

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
    if ((fd1 = openFile(argv[1])) <= ERROR || (fd2 = openFile(argv[2])) <= ERROR) {
        // Print error message.
        errorPrint("CAN'T OPEN FILES");
        // Close the files.
        closeFiles(fd1, fd2);
        // If the opening failed, return an error.
        exit(-1);
    }


    return 0;
}
