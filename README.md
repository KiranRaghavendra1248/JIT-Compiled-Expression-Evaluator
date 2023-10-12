# Operating-Systems-P1
Application that accepts a string of mathematical expression and returns result in the most complicated way possible. 
A C function to calculate this expression is generated dynamically and a .c file.
This file is then compiled using gcc by calling a child process to perform this task.
This is done using fork(), execv() and waitpid() system calls. As a result of this a shared object file(so file) is created. 
This is loaded into process memory using the dlopen() system call.
To make a function call, a lookup is done using dlsym() to get the address of the dynamically created function in memory. 
Using this address, a system call is made in the main function

# Project Structure and Summary
...
