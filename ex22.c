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
 * @return The value to the file descriptor of the file.
 */
int openFilePath(char *pathToFile, int flag) {
    int fd = open(pathToFile, flag);
    if (fd <= ERROR) {
        writeToScreen("Error in: open\n");
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
        exit(1);
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
        exit(1);
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
        exit(1);
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

int compileCFile(char *pathToCFile) {
    int status;
    pid_t pid;
    pid = fork();
    if(pid <= ERROR){
        writeToScreen("Error in: fork\n");
        exit(-1);
    }
    if(pid == CHILD_PROCESS){
        char* argumentList[] = {"gcc", pathToCFile, NULL}; // NULL terminated array of char* strings
        if(execvp("gcc", argumentList) <= ERROR){
            writeToScreen("Error in: execvp\n");
            printf("\n1\n");
            exit(-1);
        }
        printf("\n2\n");
        exit(1);
    }
    else{
        wait(&status);
        status /= 256;
        printf("status = %d\n", status);
    }
}

/**
 *
 * @param dir
 * @param pathToDir
 */
void traverseUsersDir(DIR *dir, char *pathToDir, int inputFd, int outputFd, int resultsFd, int errorFd) {
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
        strcpy(userDirPath, pathToDir);
        strcat(userDirPath, "/");
        strcat(userDirPath, entry->d_name);
        // Create a string for the .c file inside the directory.
        char cFilePath[MAX_PATH] = {0};
        // Search for a .c file in the user's directory.
        if(!findCFileInUsers(userDirPath, cFilePath)){
            continue; // IF NO C FILE DO SOMTHING!
        }
        printf("--%s\n", cFilePath);
        compileCFile(cFilePath);
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
    int inputFd = openOutputInput(inputFilePath, "Input file not exist\n");
    // Open the output file using the path we extracted from the configuration file.
    int outputFd = openOutputInput(outputFilePath, "Output file not exist\n");
    // Create result file named results.csv.
    int resultsFd = createResultFile();


    traverseUsersDir(usersDir, usersFolderPath, inputFd, outputFd, resultsFd, errorFd);

}