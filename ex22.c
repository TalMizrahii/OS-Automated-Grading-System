// Tal Mizrahi 206960890

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>

#define ERROR (-1)
#define MAX_PATH 200
#define ONE_BYTE 1
#define ALL_ACCESS 0777
#define CHILD_PROCESS 0
#define IDENTICAL 1
#define SIMILAR 3
#define NON_EQUAL 2


/**
 * A function to write to the default output (screen).
 * @param msg The message to print.
 */
void writeToScreen(char *msg) {
    if (write(STDOUT_FILENO, msg, strlen(msg) * sizeof(char)) <= ERROR) {
        exit(-1);
    }
}


/**
 * Closing a file. If an error accord, close the program.
 * @param fd The file descriptor of the file to close.
 */
void closeFile(int fd) {
    // Close the file.
    if (close(fd) <= ERROR) {
        writeToScreen("Error in: close\n");
        exit(-1);
    }
}

/**
 * Constructing a path by concatenating all args until hitting NULL.
 * @param args The strings to concatenating.
 * @param path The string to save the result.
 */
void constructPath(char *args[], char *path) {
    // A counter for the args position.
    int i = 0;
    // Check if the argument is not NULL.
    if (args[0] == NULL) {
        return;
    }
    // Copy the first argument to the result path.
    strcpy(path, args[0]);
    // Raise the counter to the second argument.
    i++;
    // Go over all arguments until the null.
    while (args[i] != NULL) {
        // Concatenate the path.
        strcat(path, args[i]);
        i++;
    }
}

/**
 * Writing to a results file a string created from data regard to a users score.
 * @param resultsFd The file descriptor of the results file.
 * @param name The name of the user.
 * @param score The score he received.
 * @param cause The cause of the score.
 */
void writeToResults(int resultsFd, char *name, char *score, char *cause) {
    // Declare an array of chars to store the result.
    char userResult[MAX_PATH] = {0};
    // Copy the name to the full content array.
    strcpy(userResult, name);
    // Add comma to separate the columns.
    strcat(userResult, ",");
    // Concatenate the score.
    strcat(userResult, score);
    // Add comma to separate the columns.
    strcat(userResult, ",");
    // Concatenate the cause of the score.
    strcat(userResult, cause);
    // Add "\n" to indicate the end of the line.
    strcat(userResult, "\n");
    // write to the file and check for an error.
    if (write(resultsFd, userResult, strlen(userResult)) <= ERROR) {
        exit(-1);
    }
}


/**
 * Checking if the amount of arguments are valid to 3.
 * @param argc the amount of arguments the program received.
 * @return If the value isn't 3 the program exits, 1 otherwise.
 */
int argNumCheck(int argc) {
    if (argc != 2) {
        writeToScreen("NOT ENOUGH ARGUMENTS!\n");
        exit(-1);
    }
    return 1;
}

/**
 * The function receive a path to a file an opens it with the "open" system call.
 * If the opening failed, the program close.
 * @param pathToFile The path to the file.
 * @param flag The flag of file opening (O_RDONLY, etc.).
 * @return The value to the file descriptor of the file.
 */
int openFilePath(char *pathToFile, int flag) {
    int fd = open(pathToFile, flag);
    if (fd <= ERROR) {
        writeToScreen("Error in: open\n");
        exit(-1);
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
    // Check if the command succeeded.
    if (read(fd, &ch, ONE_BYTE) <= ERROR) {
        writeToScreen("Error in: read\n");
        exit(-1);
    }
    // While the char isn't a new line char or EOF, concatenate the char to the line string.
    while (ch != '\n' && ch != EOF) {
        strcat(line, &ch);
        if (read(fd, &ch, ONE_BYTE) <= ERROR) {
            writeToScreen("Error in: read\n");
            exit(-1);
        }
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
 * Opening a directory. If it does not exist, we print a specific error message.
 * @param path The path to the directory.
 * @return The pointer to the directory data type.
 */
DIR *openDirectory(char *path) {
    // Declare a pointer to a directory.
    DIR *dir;
    // Try to open it.
    if ((dir = opendir(path))) {
        return dir;
    }
    // Print an error if failed and exit.
    writeToScreen("Not a valid directory\n");
    exit(-1);
}

/**
 * Opening the input or output files from the configuration file.
 * @param path The path to the files.
 * @param errorMsg The error message if the action failed.
 * @return The file descriptor of the file.
 */
int openOutputInput(char *path, char *errorMsg) {
    // Open the file.
    int fd = open(path, O_RDONLY);
    // If the action failed, print an error.
    if (fd <= ERROR) {
        writeToScreen(errorMsg);
        exit(-1);
    }
    // Return the file descriptor of the file.
    return fd;
}

/**
 * Creating a file named results.scv to store the results of the program.
 * @return If succeeded, the file descriptor of the file. If not, the program prints an error and exits.
 */
int createResultFile() {
    int resultFd = open("results.csv", O_CREAT | O_TRUNC | O_WRONLY, ALL_ACCESS);
    if (resultFd <= ERROR) {
        writeToScreen("Error in: open\n");
        exit(-1);
    }
    return resultFd;
}

/**
 * Creating a file named results.scv to store the results of the program.
 * @return If succeeded, the file descriptor of the file. If not, the program prints an error and exits.
 */
int createErrorFile() {
    int errorFd = open("errors.txt", O_CREAT | O_APPEND | O_RDWR, ALL_ACCESS);
    if (errorFd <= ERROR) {
        writeToScreen("Error in: open\n");
        exit(-1);
    }
    // Redirect stderr to the file descriptor of errors.txt.
    if (dup2(errorFd, STDERR_FILENO) <= ERROR) {
        writeToScreen("Error in: dup2\n");
        exit(-1);
    }
    return errorFd;
}


/**
 * Checking if a file is a valid .c file.
 * @param fileName The file's name.
 * @return 1 if the file ends with .c, 0 otherwise.
 */
int validC(char *fileName) {
    // Check if the file's last char is 'c' and the next to last is '.'.
    if (fileName[strlen(fileName) - 1] == 'c' && fileName[strlen(fileName) - 2] == '.') {
        // This is a .c file.
        return 1;
    }
    // It's not a .c file.
    return 0;
}

/**
 * Construct a path to a new test file to store the output from a user's program,
 * and redirect the standard input and output to the input file and the test file accordingly.
 * @param userDirPath The path to the user's directory to open inside it the output file.
 * @param inputFd The file descriptor of the input file.
 * @return The file descriptor of the test file.
 */
int redirectComparisonFile(char *userDirPath, int inputFd) {
    // Create a path to the test file.
    char fullPathToCompTxt[MAX_PATH] = {0};
    char* args[] = {userDirPath, "/testComp.txt", NULL};
    constructPath(args, fullPathToCompTxt);
    // Open the test file.
    int testFd = open(fullPathToCompTxt, O_CREAT | O_TRUNC | O_RDWR, ALL_ACCESS);
    // Check if the file opened.
    if (testFd <= ERROR) {
        writeToScreen("Error in: open\n");
        exit(-1);
    }
    // Redirect the standard input and output.
    dup2(inputFd, STDIN_FILENO);
    dup2(testFd, STDOUT_FILENO);
    // Return the file descriptor of the test file.
    return testFd;
}

/**
 *
 * @param pathToCFile
 * @param userDirPath
 * @param fullPathToExec
 * @param execName
 * @return
 */
int executeVP(char *argumentList[], int inputFd, char *userDirPath) {
    // Create a status int to save the exit status of the child.
    int status;
    // Fork the process to execute the gcc command.
    pid_t pid;
    pid = fork();
    // Validate no error occur.
    if (pid <= ERROR) {
        writeToScreen("Error in: fork\n");
        exit(-1);
    }
    // If it;s the child process.
    if (pid == CHILD_PROCESS) {
        if (inputFd) {
            redirectComparisonFile(userDirPath, inputFd);
            chdir(userDirPath);
        }
        // Execute the compilation line.
        if (execvp(argumentList[0], argumentList) <= ERROR) {
            writeToScreen("Error in: execvp1\n");
            exit(-1);
        }
        exit(1);
    } else {
        wait(&status);
    }
    // Check if the compilation phase succeeded. If so, return 1.
    return status;
}


/**
 * Given a path to a user directory, the function is traversing all files looking for a .c file.
 * If it finds one, the function construct a full path to this file and saves it in a given array.
 * @param pathToUserDir The path to the user directory.
 * @param cFilePath An array to save the full path to the .c file (if found).
 * @return 1 if it found a .c file, 0 otherwise.
 */
int findCFileInUsers(char *pathToUserDir, char *cFilePath) {
    // Open the user's directory.
    DIR *directory = opendir(pathToUserDir);
    struct dirent *entry;
    // Traverse all files inside it.
    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_type == DT_REG && validC(entry->d_name)) {
            // Construct the full path of the entry.
            strcpy(cFilePath, pathToUserDir);
            strcat(cFilePath, "/");
            strcat(cFilePath, entry->d_name);
            // found a .c file.
            closedir(directory);
            return 1;
        }
    }
    // Didn't find a .c file.
    closedir(directory);
    return 0;
}


/**
 * Compile .c file using the executeVP function.
 * @param pathToCFile The path to the .c file.
 * @param userDirPath The path to the user's directory.
 * @param fullPathToExec The path to the execution file.
 * @param execName The name of the execution file.
 * @return The status of the compilation command.
 */
int compileCFile(char *pathToCFile, char *userDirPath, char *fullPathToExec, char *execName) {
    char *args[] = {userDirPath, "/", execName, NULL};
    constructPath(args, fullPathToExec);
    char *argumentList[] = {"gcc", "-o", fullPathToExec, pathToCFile, NULL};
    // Compile the file.
    return executeVP(argumentList, 0, NULL);
}

/**
 * compare the output file of the user with the correct output and return the result.
 * Do it using the executeVP function.
 * @param userDirPath The user's directory.
 * @param outputPath The path to the correct output file.
 * @return The return status of the execution.
 */
int compareFiles(char *userDirPath, char *outputPath) {
    // construct a path to the test file.
    char fullPathToCompTxt[MAX_PATH] = {0};
    char *args[] = {userDirPath, "/testComp.txt", NULL};
    constructPath(args, fullPathToCompTxt);
    // Construct an argument list for the execVP function to run.
    char *argumentList[] = {"./comp.out", fullPathToCompTxt, outputPath, NULL};
    // Return the result.
    return executeVP(argumentList, 0, NULL);
}

/**
 * Running executable file named a.out from a given directory of a user.
 * @param inputFd The output of the executable will be directed to this file descriptor.
 * @param userDirPath The path to the user's directory.
 * @return The return value of the a.out program
 */
int runExecFile(int inputFd, char* userDirPath) {
    // Create an argument list for the executeVP command.
    char *args[] = {"./a.out", NULL};
    // Run the program using the executeVP command and return it's return value.
    return executeVP(args, inputFd, userDirPath);
}

void writeToResultsAfterRun(int status, int resultsFd){
    switch (expression) {
        
    }
}

/**
 *
 * @param dir
 * @param pathToDir
 */
void traverseUsersDir(DIR *dir, char *pathToDir, char *inputFilePath, char *outputPath, int resultsFd) {
    // Initiate a dirnet to store the data on each file in the directory to traverse.
    struct dirent *entry;
    // go over all files in the directory.
    while ((entry = readdir(dir)) != NULL) {
        // Ignore the current, parent directories, and all files different from d_type 4 witch is a directory.
        if (entry->d_type != 4 || strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        // Construct the full path of the user's directory.
        char userDirPath[MAX_PATH] = {0};
        char *path[] = {pathToDir, "/", entry->d_name, NULL};
        constructPath(path, userDirPath);
        // Create a string for the .c file inside the directory.
        char cFilePath[MAX_PATH] = {0};
        // Search for a .c file in the user's directory.
        if (!findCFileInUsers(userDirPath, cFilePath)) {
            writeToResults(resultsFd, entry->d_name, "0", "FILE_C_NO");
            continue; // IF NO C FILE DO SOMTHING!
        }
        char fullPathToExec[MAX_PATH] = {0};
        if (compileCFile(cFilePath, userDirPath, fullPathToExec, "a.out")) {
            // If the compilation didn't work.
            writeToResults(resultsFd, entry->d_name, "10", "COMPILATION_ERROR");
            continue;
        }

        // Open the output file using the path we extracted from the configuration file.
        int inputFd = openOutputInput(inputFilePath, "Input file not exist\n");
        // Run the users execution file.
        runExecFile(inputFd, userDirPath);
        // Close the input file to restore the file descriptor.
        closeFile(inputFd);
        // Create args list to execute the compare program.
        int status = compareFiles(userDirPath, outputPath);
        writeToResultsAfterRun(status, resultsFd);
        
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
    // Redirecting all errors to a new file called errors.txt.
    int errorFd = createErrorFile();
    // Open the configuration file.
    int confFd = openFilePath(argv[1], O_RDONLY);
    // Initiate three strings, all 0, to store the lines from the configuration file.
    char usersFolderPath[MAX_PATH] = {0};
    char inputFilePath[MAX_PATH] = {0};
    char outputFilePath[MAX_PATH] = {0};
    // Get the lines from the configuration file.
    readConfiguration(usersFolderPath, inputFilePath, outputFilePath, confFd);
    // Open the directory from the first line of the configuration file.
    DIR *usersDir = openDirectory(usersFolderPath);
    // Open the output file using the path we extracted from the configuration file.
    int outputFd = openOutputInput(outputFilePath, "Output file not exist\n");
    close(outputFd);
    // Create result file named results.csv.
    int resultsFd = createResultFile();
    char compFilePath[MAX_PATH] = {0};
    //int compFile = compileCFile("ex21.c", ".", compFilePath, "comp.out");
    traverseUsersDir(usersDir, usersFolderPath, inputFilePath, outputFilePath, resultsFd);

    return 0;
}
