#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {

    //Initialize Variables
    char buffer[255];
    int length;
    pid_t pid;
    int can_run = 1;
    //Initialize
    int parent_to_child[2];
    int child_to_parent[2];

    //Create the pipes
    if (pipe(pipe_parent_to_child) == -1) {
        perror("pipe_parent_to_child");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe_child_to_parent) == -1) {
        perror("pipe_child_to_parent");
        exit(EXIT_FAILURE);
    }

    //FORK
    pid = fork();

    if (pid == -1) {
        perror("Problem creating child");
        can_run = 0;
    }

    //Close pipes the parent/child will not ever user before the loop
    if (pid == 0)
    {
        close(child_to_parent[0]);
        close(parent_to_child[1]);
    }
    else
    {
        close(child_to_parent[1]);
        close(parent_to_child[0]);
    }

    //While Loop
    while(can_run) {

        if (pid == 0)   //IF child
        {
            //Wait on parent writing

            //Read Pipe

            //If CLOSE exit

            //Flip characters

            //Write to secondary pipe
        }
        else    //IF Parent
        {
            //Get New User Input

            //Wait for response

            //If input was close exit.

            //Ouput Response
        }
    }
    return 0;
}