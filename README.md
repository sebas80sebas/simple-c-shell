# Simple C Shell (msh)

This project is a simple shell implementation written in C. It was developed as a practical exercise for an operating systems course, demonstrating fundamental concepts such as process management, inter-process communication (pipes), and I/O redirection.

## Features

*   **Command Execution:** Execute standard shell commands.
*   **Pipelining:** Connect multiple commands using pipes (`|`).
*   **Input/Output Redirection:** Redirect input (`<`), output (`>`), and error output (`!>`).
*   **Background Processes:** Run commands in the background using `&`.
*   **Built-in Commands:**
    *   `mycalc`: A simple calculator supporting `add`, `mul`, and `div` operations. It also manages an accumulator (`Acc`) environment variable for chained calculations.
    *   `mytime`: Displays the elapsed time since the shell was started in `HH:MM:SS` format.

## Building the Shell

To compile the `msh` executable, navigate to the project's root directory and run:

```bash
make
```

This will create an executable named `msh`.

## Running the Shell

To start the interactive shell, execute the compiled program:

```bash
./msh
```

You will see a prompt `MSH>>` where you can enter commands.

## Running Tests

The project includes a comprehensive test suite to verify the shell's functionality. To run the tests, use the `test_runner.sh` script:

```bash
sh test_runner.sh <your_zip_file>
```

Replace `<your_zip_file>` with the name of the zip file containing the project files (this script is designed to run in a temporary directory created by unzipping the project).

