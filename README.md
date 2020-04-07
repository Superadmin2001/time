# time
takes program to run as argument, forks child process and calls wait syscall, child process gets current time, writes current time to shared memory, calls exec syscall, parent process resumes after waiting of child process, gets current time, outputs time required to run specified program
