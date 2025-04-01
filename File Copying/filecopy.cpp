/*
    @filecopy.cpp
    @authors: Nolan Nguyen, Jordan Havelaar
    @description: Function to copy file contents to a destination file. Handles error accordingly

*/

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void error(const std::string& msg) { //error message function for pipes and forks
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <source_file> <destination_file>\n";
        return EXIT_FAILURE;
    }

    const char* sourceFile = argv[1];
    const char* destFile = argv[2];

    // Create a pipe
    int pipeFD[2];
    if (pipe(pipeFD) == -1) {
        error("Error creating pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid_t pid = fork();
    if (pid == -1) {
        error("Error forking process");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { // Parent process
        close(pipeFD[0]); // Close the read end of the pipe

        // Open source file using fstream
        std::ifstream src(sourceFile);
        if (!src) {
            std::cout << "Error: Unable to open source file "<< "'" << sourceFile << "'" << "." << std::endl;
            exit(EXIT_FAILURE);
        }

        // Buffer for copying
        char buffer[25];

        // Read from source file and write to pipe
        while (src.read(buffer, sizeof(buffer))) {
            write(pipeFD[1], buffer, src.gcount());
        }

        // Write any remaining text over buffer size
        if (src.gcount() > 0) {
            write(pipeFD[1], buffer, src.gcount());
        }

        src.close();
        close(pipeFD[1]); // Close the write end of the pipe
        wait(nullptr); // Wait for the child process to finish

        std::cout << "File successfully copied from "  << sourceFile << " to " << destFile << "." << std::endl; //Success print

    } else { // Child process
        close(pipeFD[1]); // Close the write end of the pipe

        // Open destination file using fstream
        std::ofstream dest(destFile);
        if (!dest) {
            error("Error opening destination file");
            exit(EXIT_FAILURE);
        }

        // Read from pipe and write to destination file
        char buffer[4096]; //size change depending on content of input file?
        ssize_t inputRead;

        while ((inputRead = read(pipeFD[0], buffer, sizeof(buffer))) > 0) {
            dest.write(buffer, inputRead);
        }

        dest.close();
        close(pipeFD[0]); // Close the read end of the pipe
    }
    return EXIT_SUCCESS;
}