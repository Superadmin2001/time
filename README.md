# time
takes a program to run as an argument, forks a child process and calls wait syscall, the child process gets current time, writes current time to a shared memory, calls exec syscall, the parent process resumes after waiting of the child process to complete, gets the current time, outputs time required to run a specified program.
