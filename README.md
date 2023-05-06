<h1 align="center">
<br>
  <a href="https://github.com/TalMizrahii/OS-Automated-Grading-System"><img src="https://www.codelivly.com/wp-content/uploads/2023/01/bash.jpg" alt="HTML" width="200"></a>
  <br>
  Operating Systems
  <br>
</h1>
<h4 align="center">The second 2-part assignment for Operating Systems course, Bar Ilan university.

## Part 1 - File comperasion.
### About
The first part of this exercise is implemented in the file ex21.c. The program accepts two paths (relative or absolute) and conducts a comparison between them. If the files are identical (all characters are the same in value and position), the program returns 1. If the files are similar (the difference is only in spaces or newline characters), the program will return 3. If the files are not similar or identical, the program will return 2.

### Implementation

The code implements two functions, identical() and similar(), to compare two files. The functions take two file descriptors as input and return an integer that represents whether the two files are identical, similar, or not equal.

The identical() function reads both files byte by byte and compares them. If the files are identical, the function returns 1; otherwise, it returns 0.

The similar() function reads both files byte by byte and compares them while ignoring any white space characters. If the files are similar, the function returns 3; if they are not similar, it returns 2. If the two files have reached their end simultaneously, the function returns 1, indicating that the files are at least similar.

The strLength() function calculates the length of a given string, and the errorPrint() function writes an error message to the standard error output. The closeFiles() function closes two files, and the openFile() function opens a file for reading.

The code uses several system calls and standard library functions, including read(), write(), close(), open(), exit(), and sizeof().

### Execution
To clone and run this program, you'll need [Git](https://git-scm.com) installed on your computer. From your command line:

```bash
# Clone this repository.
$ git clone https://github.com/TalMizrahii/OS-Automated-Grading-System

# Go into the repository.
$ cd OS-Automated-Grading-System

# Compile.
$ gcc -o comp.out ex21.c
```
This command will create comp.out. file. I added a test file to this repo that you can test the program on.



  
## Part 2 - Bash: Tennis game
### About
The code is a shell script that simulates a board game with two players. Each player has an initial score of 50 points. The game is played in rounds, and in each round, the players enter a number between 0 and their remaining score. The player with the higher number wins the round, and their score is deducted by the amount they chose. The player who first reaches a score of 0 loses the game. The board is displayed to the screen after each round, showing the players' scores and the position of a token that moves left or right depending on the outcome of the round.

![TennisBoard](https://user-images.githubusercontent.com/92651125/187092788-ba91ed10-3f83-4096-abcf-27a88c23179a.png) 

### Implementation
The code contains several functions, each responsible for a different task. Here's a brief explanation of each function:

 * board() - This function displays the game board to the screen. It consists of an upper and lower part, and the position of the token is displayed based on the value of the $position variable.
 * playerPick() - This function asks each player to enter their choice of a number between 0 and their remaining score. It checks the validity of the input and deducts the choice from the player's score.
 * setScore() - This function sets the position of the token based on the result of the round. If player 1 wins, the token moves to the right, and if player 2 wins, the token moves to the left.
 * presentPicks() - This function displays the choices made by each player in the current round.
 * goldenTicket() - This function checks if any player has won the game by reaching a score of 0 or moving the token to the far right or left of the board.

### Execution

To run the script, simply execute it in a Bash shell. The script requires no arguments or input from the user. The game will continue until one of the players loses, and the winner will be displayed to the screen.

```bash
$ ./tennis.sh
```

## Part 3 - Mini Shell
### About
This code is a simple shell that allows the user to execute basic commands, including built-in commands like "cd" and "history". The program uses fork() to create a new process to run the command and waits for the process to complete using waitpid(). The history of executed commands and their process IDs are stored in an array of structs.

The shell can accept any number of arguments, where each argument (if any) will contain a full path to a folder,
and when the program runs, any command that enters can run if it is a command that is already known from the folder.

### Implementation
The program is implemented in C and consists of several functions:

addCommand() - Adds the last executed command to the history of commands.
 * displayHistory() - Displays the history of executed commands.
 * shellCommand() - Executes the command entered by the user.
 * getUserInput() - Gets user input from the command line.
 * splitCommandAndArgs() - Splits the command and arguments entered by the user.
 * specialCommands() - Executes the built-in commands like "cd" and "history".
 * main() - The main function of the program that runs the main loop of commands.
 
### Execution

To run the program, simply compile the code using a C compiler and execute the resulting executable. The program will run in the command line and prompt the user for input. To exit the program, enter "exit" in the command line.

Here is an example of how to compile and run the program on Linux:

```bash
# Compile
$ gcc -o shell myshell.c
```
```bash
# Run
$ ./shell
```

## Author
* [@Tal Mizrahi](https://github.com/TalMizrahii)

