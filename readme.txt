Project Detail: 
Design a multithreaded Sudoku Solution Checker program that accepts a Sudoku solution from the user as input, determines whether the solution is valid or invalid. If invalid, it also specifies which rows, columns or 3x3 subgrids have errors.


Compile String: 
gcc -std=c99 -pthread  a2.c -o a2
		
Output: 
VALID : the input file match the Sudoku problem
INVALID : the input file do not match the Sudoku problem

		
*NOTE
Compiling the program using os_26.txt should return "Valid."
Compiling the program using os.txt should return "Invalid." 
(*I modified the last integer of the os_26.txt file and rename it as os.txt to validate the program)