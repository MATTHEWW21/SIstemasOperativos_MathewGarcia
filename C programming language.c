#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

int main()
{
    char *commands[][3] = 
    {
        {"ls", "-1", NULL},
        {"date", NULL, NULL},
        {"whoami", NULL, NULL},
        {"pwd", NULL, NULL},
        {"ps", "aux", NULL}
        
    };
    
    int num_commands = 5;
    pid_t child_pids[num_commands];
    
    struct timeval start_total, end_total;
    gettimeofday(&start_total, NULL);
    
    printf("--- Process Monitor ---\n");
    printf("Parent PID: %d\n\n", getpid());
    
    printf("Creating child processes...\n");
    
    for (int i = 0; i < num_commands; i++)
    {
        pid_t rc = fork();
        
        if (rc < 0)
        {
            fprintf(stderr, "Fork failed for command %d\n", i);
            exit(1);
        }else if (rc == 0){
            printf("Child %d (PID: %d) executing: %s\n", i, getpid(), commands[i][0]);
            
            execvp(commands[i][0], commands[i]);
            
            fprintf(stderr, "Exec failed for command: %s\n", commands[i][0]);
            exit(1);
        }else{
            child_pids[i] = rc;
            printf("Parent created child %d with PID: %d\n", i, rc);
        }
    }
    
    printf("\nChild processes executing...\n");
    
    for (int i = 0; i < num_commands; i++)
    {
        int status;
        pid_t wc = wait(&status);
        
        if (wc == -1){
            fprintf(stderr, "wait failed\n");
            continue;
        }
        
        int child_index = -1;
        for (int j = 0; j < num_commands; j++)
        {
            if (child_pids[j] == wc)
            {
                child_index = j;
                break;
            }
        }
        
        if (WIFEXITED(status))
        {
            printf("Child %d (PID: %d) completed with exit code: %d\n", child_index, wc, WEXITSTATUS(status));
            
        }else{
            printf("Child %d (PID: %d) terminated abnormally\n", child_index, wc);
        }
    }
    
    gettimeofday(&end_total, NULL);
    double total_time = (end_total.tv_sec - start_total.tv_sec) + (end_total.tv_usec - start_total.tv_usec) / 1000000.0;
    printf("\n total execution time: %.3f seconds\n", total_time);
    
    return 0;
}
