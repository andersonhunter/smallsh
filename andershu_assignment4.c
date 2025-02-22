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
    if(!strcmp(token, "#") && curr_command->argc == 0) {
      break;
    }
    // Determine if user specified input file
    else if(!strcmp(token, "<")) {
      // Copy user inputted input file to command struct
      curr_command->input_file = strdup(strtok(NULL, " \n"));
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
  return curr_command;
}

void newChild(struct children *head, int pid) {
  /* A helper method to record a new child process */
  printf("Adding new child with pid %d\n", pid);
  fflush(stdout);
  if(!head->pid) {
    printf("New child is the head\n");
    fflush(stdout);
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
    printf("If this worked then <%d> will be a pid\n", new->pid);
    fflush(stdout);
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

int main() {
  struct children *head = malloc(sizeof(struct children));
  while(true) {
    struct cli *curr_command = malloc(sizeof(struct cli));
    int status = 0;
    parse_input(curr_command);
    printf("Entered the following %d args:\n", curr_command->argc);
    printArgs(curr_command);

    // Check if command is exit
    if(!strcmp(curr_command->argv[0], "exit")) {
      // Kill children
      struct children *temp = head;
      // Iterate over children
      while(temp!=NULL) {
        struct children *previous;
        kill(temp->pid, 15);  // Kill the child with SIGTERM
        previous = temp;
        temp = temp->next; 
        free(previous);       // Free the newly-killed child
      }
      // Reset head
      head = malloc(sizeof(struct children));
      // Exit
      break;
    }
  
    // Check if command is cd
    if(!strcmp(curr_command->argv[0], "cd")) {
      // No args, so cd to HOME
      if(curr_command->argc == 1) {
        setenv("PWD", getenv("HOME"), 1);                // Set current working directory to home
      }
      // Check if relative path selected
      else if(!strncmp(curr_command->argv[1], "./", 2)) {
        char* newDir = malloc(MAX_ARGS * sizeof(char));  // Allocate memory for new path
        strcpy(newDir, getenv("PWD"));                   // Copy current working directory to new path
        strcat(newDir, curr_command->argv[1] + 1);       // Append user-defined new directory without leading chars
        setenv("PWD", newDir, 1);                        // Set env variable for cwd to new directory path
        free(newDir);
      }
      // Absolute path chosen
      else {
        setenv("PWD", curr_command->argv[1], 1);         // Set pwd env variable to user-chosen absolute path
      }
    }
    
    // Check if command is pwd
    if(!strcmp(curr_command->argv[0], "pwd")) {
      // Print the current working directory
      printf("%s\n", getenv("PWD"));
      fflush(stdout);
    }

    // Check if command is status
    if(!strcmp(curr_command->argv[0], "status")) {
      switch(status) {
        case 0:
          printf("Exit status %d\n", status);
          break;
      }
    }

    fflush(stdin);
    fflush(stdout);
    free(curr_command);
  }

  return 0;
}

