<h1 align="center">
<br>
  <a href="https://github.com/TalMizrahii/OS-Automated-Grading-System"><img src="https://logodownload.org/wp-content/uploads/2022/05/linux-logo-1.png" alt="HTML" width="200"></a>
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

// Clone this repository.
```bash
git clone https://github.com/TalMizrahii/OS-Automated-Grading-System
```
// Go into the repository:
```bash
cd OS-Automated-Grading-System
```

// Compile:
```bash
 gcc -o comp.out ex21.c
```

// For identical check:
```bash
 ./comp.out textComparison/identical/ross/ross1.txt textComparison/identical/ross/ross2.txt
```


// For similar check:
```bash
 ./comp.out textComparison/similar/moo/joey1.txt textComparison/similar/moo/joey2.txt
```


// For different check:
```bash
 ./comp.out textComparison/different/french/joey.txt textComparison/different/french/phoebe.txt
```

//To observe the return value you can execute:  
```bash
 echo $?
```

## Part 2 - Automated Grading System.
### About
The second part of the assignment is to create an automated grading system. The system receives a configuration file as an argument, which contains three lines. The first line is a path to a directory of students, the second is a path to an input file, and the last line is a path to an output file. The system processes the file by traversing all directories (up to depth 1 only) in the students directory and checks the .c files inside each student directory. The system redirects the default input (file descriptor 1) to the file from line 2 in the configuration file and redirects the standard output file descriptor to a new test file in the student directory. Then, the program executes the code from part 1 (comp.out) and compares the output of the student to the correct output file given in line 3 of the configuration file. If any errors occurred during the run of the student file, the system redirects them to a new errors.txt file in its current working directory. The results and grading of the students are saved in a new file called results.csv.

### Implementation
The main function reads the command-line arguments and calls several other functions to carry out the grading process. The finishTheProgram() function is responsible for closing any opened file descriptors, while traverseUsersDir() is responsible for traversing each student directory and grading their C program.

There are several helper functions in the program, such as argNumCheck(), createErrorFile(), openFilePath(), openOutputInput(), and createResultFile(). These functions are used to perform basic error checking, open files, and create files.

The program uses several system calls, such as open(), read(), write(), mkdir(), chdir(), execvp(), and remove(). It also uses several standard library functions, such as strcmp(), sprintf(), strcat(), clock(), time(), and rewinddir().

Overall, the program designed to automate the process of grading C programs, making it faster and more accurate than a manual grading process.


### Execution
To clone and run this program, you'll need [Git](https://git-scm.com) installed on your computer. From your command line:

// Clone this repository.
```bash
git clone https://github.com/TalMizrahii/OS-Automated-Grading-System
```
// Go into the repository:
```bash
cd OS-Automated-Grading-System
```
// If you didnt compile the ex21.c file, you can do it by:
```bash
 gcc -o comp.out ex21.c
```
// To compile the second part:
```bash
 gcc ex22.c
```

// To run the program:
```bash
 ./a.out gradingTest/conf.txt
```

The result are shown in the results.csv file created by the program.
## Author
* [@Tal Mizrahi](https://github.com/TalMizrahii)

