// Name: Hunter Anderson
// Email: andershu@oregonstate.edu
// Date: 2 March 2025
// Description: Small shell!
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include<fcntl.h>

#define INPUT_LENGTH 2048
#define MAX_ARGS 512

struct cli {
  // Adapted from sample_parser.c starter code
  char *argv[MAX_ARGS + 1];  // Hold up to MAX_ARGS number of arguments
  int argc;                  // Count the number of arguments passed
  char *input_file;          // Hold the user-selected input file
  char *output_file;         // Hold the selected output file
  bool is_bg;                // Determine if process is a background process
};

struct children {
  // Create a LL of child processes
  int pid;                // Hold PID for child
  struct children *next;  // Point to next child
};

struct cli *parse_input(struct cli *curr_command) {
  // Adapted from sample_parser.c starter code
  char input[INPUT_LENGTH];                                              // Hold user input
  curr_command->argc = 0;
  // Get input
  printf(": ");                       // Print command prompt
  fflush(stdout);                     // Flush stdoutput
  fgets(input, INPUT_LENGTH, stdin);  // Get input from stdinput
  
  // Tokenize input
  char *token = strtok(input, " \n");
  while(token) {
    // Determine if user entered a comment
    if(!strncmp(token, "#", 1) && curr_command->argc == 0) {
      break;
    }
    // Determine if user specified input file
    else if(!strcmp(token, "<")) {
      // Copy user inputted input file to command struct
      curr_command->input_file = strdup(strtok(NULL, " \n"));
      curr_command->argv[curr_command->argc] = strdup(curr_command->input_file);
      curr_command->argc++;
    }
    // Determine if user specified output file
    else if(!strcmp(token, ">")) {
      // Copy user inputted output file to struct
      curr_command->output_file = strdup(strtok(NULL, " \n"));
    }
    // Determine if user specified background process
    else if (!strcmp(token, "&")) {
      // Set bg to true
      curr_command->is_bg = true;
    }
    else if (strcmp(token, " ")) {
      // Set next argument field to token, increment argument
      curr_command->argv[curr_command->argc] = strdup(token);
      curr_command->argc++;
    }
    token = strtok(NULL, " \n");
  }
  curr_command->argv[curr_command->argc] = '\0'; // Null terminate arg vector
  return curr_command;
}

void newChild(struct children *head, int pid) {
  /* A helper method to record a new child process */
  if(!head->pid) {
    head->pid = pid;
  }
  else {
    struct children *temp = head;
    while(temp->next != NULL) {
      temp = temp->next;
    }
    struct children *new = malloc(sizeof(struct children));
    new->pid = pid;
    new->next = NULL;
    temp->next = new;
  }
}

void printArgs(struct cli *cli) {
  // A debugging function to print the stored arguments at the current input
  for(int i = 0; i < cli->argc; i++) {
    if(cli->argc == 0) {
      printf("No args\n");
      fflush(stdout);
      break;
    }
    printf("Arg #%d: %s\n", i + 1, cli->argv[i]);
    fflush(stdout);
  }
}

void printChildren(struct children *head) {
  // A debugging method to print the active children
  struct children *temp = head;
  int i = 1;
  while(temp!=NULL) {
    printf("Child # %d: %d\n", i, temp->pid);
    i++;
    temp = temp->next;
  }
}

void removeChild(struct children *head, struct children *previous, struct children *removeNode) {
  /*
  A helper method to remove an inactive child 
  Receives the head of the LL, the previous child, and the child to remove
  */
 struct children *temp = head;
 if(removeNode == head && head->next == NULL) {
  // Remove head, head is the only node
  head->pid = 0;
  head->next = NULL;
 }
 else if(removeNode == head && head->next != NULL) {
  // Remove head, head is not the only node
  head = head->next;
  free(temp);
 }
 else {
  // Remove some other node
  previous->next = removeNode->next;
  free(removeNode);
 }
}

int main() {
  struct children *head = malloc(sizeof(struct children));
  head->pid = 0;
  while(true) {
    struct cli *curr_command = malloc(sizeof(struct cli));
    curr_command->argc = 0;
    curr_command->is_bg = false;
    int status = 0;
    parse_input(curr_command);
    if(curr_command->argc > 0) {
      // Check if command is exit
      if(!strcmp(curr_command->argv[0], "exit")) {
        // Kill children
        struct children *temp = head;
        // Iterate over children
        while(temp!=NULL) {
          int childStatus;
          struct children *previous;
          kill(temp->pid, 15);                        // Kill the child with SIGTERM
          waitpid(temp->pid, &childStatus, WNOHANG);  // Check child exit status
          if(WIFSIGNALED(childStatus)) {
            // Child was killed by a signal, print the signal
            printf("background pid %d is done: terminated by signal %d", temp->pid, WTERMSIG(childStatus));
          }
          previous = temp;
          temp = temp->next;                          // Advance
          free(previous);                             // Free the newly-killed child
        }
        // Exit
        exit(EXIT_SUCCESS);
      }
  
      // Check if command is cd
      else if(!strcmp(curr_command->argv[0], "cd")) {
        // No args, so cd to HOME
        if(curr_command->argc == 1) {
          chdir(getenv("HOME"));                           // Change cwd to HOME
          setenv("PWD", getenv("HOME"), 1);                // Set current working directory to home
        }
        // Check if relative path selected
        else if(!strncmp(curr_command->argv[1], "./", 2)) {
          char* newDir = malloc(MAX_ARGS * sizeof(char));  // Allocate memory for new path
          strcpy(newDir, getenv("PWD"));                   // Copy current working directory to new path
          strcat(newDir, curr_command->argv[1] + 1);       // Append user-defined new directory without leading chars
          chdir(newDir);                                   // Change cwd to new dir
          setenv("PWD", newDir, 1);                        // Set env variable for cwd to new directory path
          free(newDir);
        }
        // Absolute path chosen
        else {
          setenv("PWD", curr_command->argv[1], 1);         // Set pwd env variable to user-chosen absolute path
        }
      }
    
      // Check if command is pwd
      else if(!strcmp(curr_command->argv[0], "pwd")) {
        // Print the current working directory in a child process
        pid_t spawnpid = -5;  // Hold child's PID
        int childStatus;
        spawnpid = fork();    // Fork into child process
        switch (spawnpid) {
          case -1:
            // Error, set error status
            status = 1;
            exit(1);
            break;
          case 0:
            // Child process, pwd
            execvp(curr_command->argv[0], curr_command->argv);
            perror("execlp");
            exit(EXIT_FAILURE);
          default:
            // Parent process, record child
            // Check if background process
            if(curr_command->is_bg == true) {
              newChild(head, spawnpid);
              printf("background pid is %d\n", spawnpid);
              fflush(stdout);
              break;
            }
            // Foreground process
            else {
              waitpid(spawnpid, &childStatus, 0);
              break;
            }
            break;
        }
      }
      
      // Check if command is ls
      else if(!strcmp(curr_command->argv[0], "ls")) {
        pid_t spawnpid = -5;
        int childStatus;
        spawnpid = fork();
        switch (spawnpid) {
          case -1:
            // Error, set error status
            status = 1;
            exit(1);
            break;
          case 0:
            // Child process, list files
            execvp(curr_command->argv[0], curr_command->argv);
            perror("execvp");
            exit(EXIT_FAILURE);
          default:
            // Parent process, ck if background process
            if(curr_command->is_bg == true) {
              newChild(head, spawnpid);
              printf("background pid is %d\n", spawnpid);
              fflush(stdout);
              break;
            }
            else {
              // Background process
              waitpid(spawnpid, &childStatus, 0);
              break;
            }
            break;
        }
      }

      // Check if command is wc
      else if(!strcmp(curr_command->argv[0], "wc")) {
        pid_t spawnpid = -5;
        int childStatus;
        int fd;
        spawnpid = fork();
        switch(spawnpid) {
          case -1:
            break;
          case 0:
            // Child process
            // Check if file exists
            fd = open(curr_command->input_file, O_RDONLY);
            if(fd == -1) {
              // File does not exist, return an error
              printf("%s: no such file or directory\n", curr_command->input_file);
              fflush(stdout);
              exit(EXIT_FAILURE);
            }
            else {
              // File exists, close file and call wc
              close(fd);
              execvp(curr_command->argv[0], curr_command->argv);
              perror("execvp");
              break;
            }
          default:
            // Parent process
            if(curr_command->is_bg == true) {
              // Background process, add child to LL
              newChild(head, spawnpid);
              printf("background pid is %d\n", spawnpid);
              fflush(stdout);
              break;
            }
            else {
              waitpid(spawnpid, &childStatus, 0);
              break;
            }
            break;
        }
      }

      // Check if command is cat
      else if(!strcmp(curr_command->argv[0], "cat")) {
        pid_t spawnpid = -5;
        int childStatus;
        spawnpid = fork();
        switch(spawnpid) {
          case -1:
            break;
          case 0:
            // Child process
            execvp(curr_command->argv[0], curr_command->argv);
            perror("execvp");
            break;
          default:
            if(curr_command->is_bg == true) {
              newChild(head, spawnpid);
              printf("background pid is %d\n", spawnpid);
              fflush(stdout);
              break;
            }
            else {
              waitpid(spawnpid, &childStatus, 0);
              break;
            }
            break;
        }
      }

      // Check if command is sleep
      else if(!strcmp(curr_command->argv[0], "sleep")) {
        pid_t spawnpid = -5;
        int childStatus;
        spawnpid = fork();
        switch(spawnpid) {
          case -1:
            break;
          case 0:
            // Child process
            execvp(curr_command->argv[0], curr_command->argv);
            perror("execvp");
            break;
          default:
            if(curr_command->is_bg == true) {
              newChild(head, spawnpid);
              printf("background pid is %d\n", spawnpid);
              fflush(stdout);
              break;
            }
            else {
              waitpid(spawnpid, &childStatus, 0);
            }
            break;
        }
      }

      // Check if command is status
      else if(!strcmp(curr_command->argv[0], "status")) {
        switch(status) {
          case 0:
            printf("Exit status %d\n", status);
            break;
        }
      }
    }
    // Check if any background children have finished
    struct children *temp = head;
    struct children *previous = head;
    while(temp!=NULL && temp->pid != 0) {
      int childStatus;
      waitpid(temp->pid, &childStatus, WNOHANG);
      if(WIFEXITED(childStatus)) {
        // Background process completed by exiting
        printf("background pid %d is done: exit value %d\n", temp->pid, WEXITSTATUS(childStatus));
        fflush(stdout);
        removeChild(head, previous, temp);
        temp = previous;
      }
      else if(WIFSIGNALED(childStatus)) {
        // Background process completed via signal termination
        printf("background pid %d is done: terminated by signal %d\n", temp->pid, WTERMSIG(childStatus));
        fflush(stdout);
        removeChild(head, previous, temp);
        temp = previous;
      }
      temp = temp->next;
    }
    fflush(stdout);
    free(curr_command);
  }

  return 0;
}

