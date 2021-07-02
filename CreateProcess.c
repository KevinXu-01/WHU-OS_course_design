#include <stdio.h>
#include<unistd.h>
#include "wait.h"
#include <stdlib.h>

int main()
{
        pid_t pid1 = fork();
        if (pid1 < 0)
        {
                printf("fork failed.\n");
                exit(1);
        }
        else if (pid1 == 0)
        {
                printf("子进程1: PID = %d, 父进程的PID = %d，B\n", getpid(), getppid());
                return 0;
        }
        pid_t pid2 = fork();
        if (pid2 < 0)
        {
                printf("fork failed.\n");
                exit(2);
        }
        else if (pid2 == 0)
        {
                printf("子进程2: PID = %d, 父进程的PID = %d，C\n", getpid(), getppid());
                return 0;
        }
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        printf("父进程: PID = %d, A\n", getpid());
        return 0;
}