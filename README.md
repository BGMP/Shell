Shell
===
Linux Shell emulator written in C.

This program allows for the execution of Linux system calls via commands. The implementation is as simple as it can get, 
wrapping all input into a series of arguments to then interpret and work upon. Error handling is also exaggerated to
illustrate the practical applications of errno.

### Compilation
In order to compile, run the build script found at this project's root:
```
chmod +x build.sh
./build.sh
```
The script will create a directory within the project root called `build/`, and within it, you'll find the binary
'Shell'.

To compile manually:
```
cc src/main.c -I include/ -o Shell
```
The binary will also be called 'Shell'.

### Running
To run the shell, simply execute the `Shell` binary.

```
./Shell
```

### Commands
List of available commands:
  * `cwd`: Prints out the current directory.
  * `seconds`: Prints out the amount of seconds elapsed from January 1st, 1970 (EPOCH).
  * `stats`: Prints out the system uptime, RAM info and current processes.
  * `ls`: Prints out all files from the current directory.
  * `mkdir <path>`: Creates a new directory named 'path'. By default, the directory is generated within the path where 
  the user is located at when executing the command. If 'path' contains more than one directory (a/b/c/dir) the
  directory 'dir' will be generated within the specified path.
    * path: Name of the new directory.
  * `cd <path>`: Moves the user into directory 'path'.
    * path: Name of the path where the user wants to move into.
  * `exit`: Terminate the program.

#### Special commands
  * `MAKEITGOOD`: Enables a mode where the Shell prints out with randomly coloured text. In order to disable this, simply
  run this command for a second time.
  * `JOKER`: Enables a mode where the Shell prints out with randomly coloured background. In order to disable this,
  simply run this command for a second time.
