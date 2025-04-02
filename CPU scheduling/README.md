------------------------- READ ME FILE -------------------------

Summary: This file will give a description of the code and how to run it.

----------------------------------------------------------------

Description: This program creates the verification of the Banker's Algorithm.
            

----------------------------------------------------------------

=========================== HOW TO RUN =========================

Step 1: Open VS Code with WSL extension. We have used Linux GCC to compile the code.

Step 2: Open Terminal with WSL. Ensure code can compile with 
        given compiler in Linux. We will use the Makefile. 
        It utilizes the "make" command to run the necessary algorithm

Step 3: Ensure that there is a clean batch, so use the command "make clean"
        The existing .o files will disappear.

Step 4: Create the specific algorithm instance with    
         "make (algorithm file name here)".

        For example: 
            make fcfs (will create .o files for first come first served)
    
Step 5: Call the scheduler with the following:
        ./fcfs schedule.txt

        Ensure to pass the text file so the terminal outputs the list of Processes
----------------------------------------------------------------
