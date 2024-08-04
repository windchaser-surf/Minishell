# Overview

The Minishell project is a group project where we created a simple shell similar to bash using the C programming language. This project helped me understand how shells work, including command execution, process management, and file handling in a Unix-like environment.

## Objectives

The main goal of this project was to build a functional shell that can interpret and execute user commands, manage processes, and handle input/output redirection. I followed strict coding standards to ensure the code is clean and maintainable.

## Key Features

- **Command Execution**: The shell can execute built-in commands and external programs.
- **Multi-Processing**: It supports running multiple processes, allowing for both foreground and background execution.
- **Input/Output Redirection**: Users can redirect input and output using standard shell syntax.
- **Environment Variables**: The shell can manage environment variables, allowing users to set and retrieve them.
- **Command Line Parsing**: The shell includes functionality to parse command lines for proper execution.
- **Signal Handling**: The shell handles signals (like ctrl-C, ctrl-D, and ctrl-\) in the same way as bash, providing a familiar user experience.

## Built-in Functions

I implemented the following built-in functions in my shell:

- `cd` (Change Directory)
- `echo` (Display a line of text)
- `exit` (Exit the shell)
- `pwd` (Print Working Directory)
- `env` (Display environment variables)
- `export` (Set environment variables)
- `unset` (Remove environment variables)

## Technical Details

- **Language**: C
- **Memory Management**: I paid close attention to memory allocation and deallocation to prevent memory leaks.
- **Makefile**: A Makefile was created to simplify the build process, ensuring proper compilation with necessary flags.

## Learning Outcomes

Through this project, I learned about:

- The structure and functionality of Unix-like shells.
- The importance of following coding standards and best practices.
- The details of process management and command execution.
- Debugging techniques and memory management in C.

## Conclusion

The Minishell project was a valuable experience that enhanced my understanding of operating systems and shell programming. I am excited to share this project on GitHub, and I hope it serves as a helpful resource for others interested in shell development.

Feel free to explore the code, contribute, or reach out with any questions!
