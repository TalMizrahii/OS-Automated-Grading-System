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
// Go into the repository.
```bash
cd OS-Automated-Grading-System
```

// Compile.
```bash
 gcc -o comp.out ex21.c
```
This command will create comp.out. file. I added a test file to this repo that you can test the program on.
  

## Part 2 - 
### About


### Implementation

### Execution


## Author
* [@Tal Mizrahi](https://github.com/TalMizrahii)

