# GEMINI.md

## Project Overview

This project is a custom shell implementation in C called `msh`. It provides a basic command-line interface that supports:

*   Executing simple commands.
*   Piping multiple commands together.
*   Input and output redirection (`<`, `>`, `!>`).
*   Running commands in the background (`&`).
*   Built-in commands: `mycalc` for arithmetic operations and `mytime` to show the shell's uptime.

The project is built using a `Makefile` and is tested using a shell script (`test_runner.sh`).

## Building and Running

### Building the shell

To compile the `msh` executable, run the following command:

```bash
make
```

This will generate the `msh` executable in the root directory.

### Cleaning the build

To remove the compiled files, you can run:

```bash
make clean
```

### Running the shell

To start the interactive shell, execute the following command:

```bash
./msh
```

### Running the tests

The project includes a test script that verifies the shell's functionality. To run the tests, execute the following command:

```bash
sh test_runner.sh <your_zip_file>
```

Replace `<your_zip_file>` with the name of the zip file containing the project files.

## Development Conventions

*   The code is written in C.
*   The project uses a `Makefile` for build automation.
*   The code includes headers for standard C libraries like `stdio.h`, `stdlib.h`, `string.h`, `unistd.h`, and `sys/wait.h`.
*   The shell uses the `execvp` function to execute commands.
*   The `mycalc` command supports addition, multiplication, and division.
*   The `mytime` command displays the time elapsed since the shell was started.
*   The project uses a shared library `libparser.so` for parsing commands.
*   File `authors.txt` lists the contributors.