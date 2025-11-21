#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {
    //Initialize Variables
    int wait_status;
    int max_input = 254;
    pid_t pid;
    int i = 0;
    char single_char;


    while (1){
        //FORK
        int parent_to_child[2];
        int child_to_parent[2];

        //Create the pipes
        if (pipe(parent_to_child) == -1) {
            perror("problem with pipe parent_to_child");
            exit(-1);
        }
        if (pipe(child_to_parent) == -1) {
            perror("problem with pipe child_to_parent");
            close(parent_to_child[0]);
            close(parent_to_child[1]);
            exit(-1);
        }

        pid = fork();

        if (pid == -1) {
            perror("Problem creating child during fork");
            close(parent_to_child[0]);
            close(parent_to_child[1]);
            close(child_to_parent[0]);
            close(child_to_parent[1]);
            exit(-1);
        }

        if (pid == 0)
        {
            close(child_to_parent[0]);
            close(parent_to_child[1]);

            char child_buffer[255];
            i = 0;
            //Wait on parent writing
            while (read(parent_to_child[0], &single_char, 1) > 0)
            {
                child_buffer[i] = single_char;
                i++;
            }
            child_buffer[i] = '\0';

            //If output is close exit
            if (strcmp(child_buffer, "exit") == 0) {
                close(child_to_parent[1]);
                close(parent_to_child[0]);
                exit(1);
            }

            for (int j = 0; j < i; ++j) {
                if (isupper(child_buffer[j]))
                {
                    child_buffer[j] = (char)tolower(child_buffer[j]);
                }
                else if (islower(child_buffer[j]))
                {
                    child_buffer[j] = (char)toupper(child_buffer[j]);
                }
            }

            printf("The String from Child to Parent is: %s\n", child_buffer);

            //Write to secondary pipe
            write(child_to_parent[1], child_buffer, strlen(child_buffer) + 1);

            close(parent_to_child[0]);
            close(child_to_parent[1]);
            exit(1);
        }
        else
        {
            close(child_to_parent[1]);
            close(parent_to_child[0]);

            char parent_buffer[255];

            printf("Please Enter Your Sentence: ");
            //Get New User Input
            fgets(parent_buffer, max_input, stdin);
            parent_buffer[strcspn(parent_buffer, "\n")] = '\0';

            //Send to Child.
            write(parent_to_child[1], parent_buffer, strlen(parent_buffer) + 1);
            close(parent_to_child[1]);
            //If input was exit, exit.
            if (strcmp(parent_buffer, "exit") == 0) {
                close(child_to_parent[0]);
                exit(1);
            }

            wait(&wait_status);

            i = 0;
            while (read(child_to_parent[0], &single_char, 1) > 0)
            {
                parent_buffer[i] = single_char;
                i++;
            }
            parent_buffer[i] = '\0';

            //Output Child Response
            printf("The String Received by Parent is: %s\n", parent_buffer);

        }
    }
    return 0;
}