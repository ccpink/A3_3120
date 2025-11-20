#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {
    //Initialize Variables
    int max_input = 254;
    int length;
    pid_t pid;
    int can_run = 1;
    //Initialize
    int parent_to_child[2];
    int child_to_parent[2];

    //Create the pipes
    if (pipe(parent_to_child) == -1) {
        perror("pipe_parent_to_child");
        exit(EXIT_FAILURE);
    }
    if (pipe(child_to_parent) == -1) {
        perror("pipe_child_to_parent");
        exit(EXIT_FAILURE);
    }

    //FORK
    pid = fork();

    if (pid == -1) {
        perror("Problem creating child");
        return 0;
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



    if (pid == 0)   //IF Child
    {
        while (1){
            char child_buffer[255];
            //Wait on parent writing
            while (read(parent_to_child[0], &child_buffer, 1) > 0)
            {

            }
            //Read Pipe

            //If output is close exit
            if (strcmp(child_buffer, "close") == 0) {
                close(child_to_parent[1]);
                close(parent_to_child[0]);
                exit(1);
            }

            //Flip characters

            //Write to secondary pipe
            write(child_to_parent[1], child_buffer, strlen(child_buffer) + 1);
        }
    }
    else    //IF Parent
    {
        while (1){
            char parent_buffer[255];

            //Get New User Input
            fgets(parent_buffer, max_input, stdin);

            //Send to Child.
            write(parent_to_child[1], parent_buffer, strlen(parent_buffer) + 1);

            //If input was close exit.
            if (strcmp(parent_buffer, "close") == 0) {
                close(child_to_parent[0]);
                close(parent_to_child[1]);
                exit(1);
            }

            while (read(child_to_parent[0], &parent_buffer, 1) > 0)
            {

            }
            //Ouput Child Response


        }
    }

    return 0;
}