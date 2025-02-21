# File-Copy-Program

A simple file copying program that provides two implementations of the same functionality using different methods: low-level file I/O and ANSI standard I/O. The program supports both Windows and Linux operating systems.

## Features

- **Two implementations:** One using low-level file I/O and another using ANSI standard I/O.
- **Cross-platform:** Compatible with both Windows and Linux systems.
- **Efficient file copying:** Allows users to copy files from one location to another with minimal overhead.

## Installation

To get started, clone the repository:

```bash
git clone https://github.com/yourusername/File-Copy-Program.git
cd File-Copy-Program
```

### Building the project

1. **On Linux:**

   - Open a terminal and navigate to the project directory.

   - Use 

     ```
     g++
     ```

      to compile the program:

     ```bash
     g++ -o file_copy file_copy.cpp
     ```

2. **On Windows:**

   - Open a terminal and navigate to the project directory.

   - Use a C++ compiler such as 

     ```
     g++
     ```

      from MinGW or MSVC to build:

     ```bash
     g++ -o file_copy file_copy.cpp
     ```

## Usage

### Copying a file with the low-level I/O method

```bash
./file_copy <source_file> <destination_file>
```

### Copying a file with the ANSI standard I/O method

```bash
./file_copy -ansi <source_file> <destination_file>
```

Replace `<source_file>` and `<destination_file>` with the paths of the source file and destination file, respectively.

## How it Works

- **Low-Level I/O Method:** This method uses direct system calls to read and write file data, providing low-level control over file copying operations.
- **ANSI Standard I/O Method:** This method uses higher-level C library functions (like `fopen`, `fread`, `fwrite`) for file copying, ensuring portability across different platforms.

## License

This project is licensed under the MIT License - see the [LICENSE](https://chatgpt.com/c/LICENSE) file for details.
