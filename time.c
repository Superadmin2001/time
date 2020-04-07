#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define PAGE_SIZE 0x1000

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        printf("Too few arguments!\n");
        return -1;
    }
    
    int shmFd = shm_open("/time", O_CREAT | O_RDWR, 0666);
    if(shmFd == -1)
    {
        printf("shm_open failed!\n");
        return -1;
    }
    
    ftruncate(shmFd, PAGE_SIZE);
    void *shmPtr = mmap(0, PAGE_SIZE, PROT_WRITE, MAP_SHARED, shmFd, 0);
    
    struct timeval startTime, endTime;
    
    pid_t pid = fork();
    
    if(pid == 0)
    {
        char **args = (char**)malloc(sizeof(char*)*argc);
        for(int i = 0; i < argc-1; i++)
            args[i] = (char*)malloc(sizeof(char)* strlen(argv[i+1]));
        args[argc-1] = NULL;
        
        for(int i = 0; i < argc-1; i++)
            strcpy(args[i], argv[i+1]);
        
        gettimeofday(&startTime, NULL);
        *((suseconds_t*)shmPtr) = startTime.tv_usec;
        
        execvp(argv[1], args);
    }
    else if(pid > 0)
    {
        wait(NULL);
        gettimeofday(&endTime, NULL);
        printf("time: %d microseconds\n", endTime.tv_usec - *((suseconds_t*)shmPtr));
        
        shm_unlink("/time");
    }
    else
    {
        printf("Fork failed!\n");
        shm_unlink("/time");
    }
    
    return 0;
}