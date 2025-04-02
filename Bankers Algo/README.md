------------------------- READ ME FILE -------------------------

Summary: This file will give a description of the code and how to run it.

----------------------------------------------------------------

Description: This program creates the verification of the Banker's Algorithm.
            

----------------------------------------------------------------

=========================== HOW TO RUN =========================

Step 1: Open VS Code with WSL extension. We have used Linux to compile the code.

Step 2: Open Terminal with WSL. Ensure code can compile with 
        given compiler in Linux. (NOTE: I have used g++ bankers.cpp)

Step 3: Upon successful compilation, call the .exe or .out file, in my case I have called
        using "./a.out"

Step 4: After typing the execute, look in the terminal to see the outputs being printed.

        For example: 
                        Test case 1: Run safe test:
                        System is in a safe state.
                        Safe Sequence: [1, 3, 4, 2, 0]

                        Test case 2:
                        System is in a safe state.
                        Safe Sequence: [1, 3, 4, 2, 0]
                        Resources allocated to process 1.

                        Test case 3:
                        System is in a safe state.
                        Safe Sequence: [3, 4, 1, 2, 0]
                        Resources allocated to process 4.
----------------------------------------------------------------