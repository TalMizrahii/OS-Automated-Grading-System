#include <stdio.h>

/**
 * Checking if the amount of arguments are valid to 3.
 * @param argc the amount of arguments the program received.
 * @return 0 if the value isn't 3, 1 otherwise.
 */
int argNumCheck(int argc){
    if(argc != 3){
        perror("NOT ENOUGH ARGUMENTS!");
        return 0;
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
int main(int argc, char *argv[]){
    // Check if the amount of arguments is valid.
    if(!argNumCheck(argc)){
        return -1;
    }
    return 0;
}
