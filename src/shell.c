#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>

#include "builtin.h"

char currentPath[2048];


int launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    for(int i = 0; i < num_builtins_output(); i++) {
            if(strcmp(args[0], builtin_str_output[i]) == 0) {
                status = (*builtin_func_output[i])(args);
                if(status) {
                    exit(EXIT_SUCCESS);
                }
                else {
                    exit(EXIT_FAILURE);
                }
            }
        }

        if(execvp(args[0], args) == -1) {
            perror("sh");
        }
        exit(EXIT_FAILURE);

  } else if (pid < 0) {
    perror("sh");

  } else {
        do {
            waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }

  return 1;
}

int execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    return 1;
  }

  for (i = 0; i < num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return launch(args);
}

char *read_line(void)
{
    int size = 1024;

    char *buffer = malloc(sizeof(char) * size);

    if(!buffer) {
        perror("allocation error");
        exit(EXIT_FAILURE);
    }

    int c = getchar(); 
    int index = 0;
    while(c != '\n') {
        if(c == EOF) {
            exit(EXIT_SUCCESS);
        }

        if(index >= size) {
            size *= 2;
            buffer = realloc(buffer, size);
            if(!buffer) {
                perror("allocation error");
                exit(EXIT_FAILURE);
            }
        }

        buffer[index] = c;
        index++;
        c = getchar();
    }

    buffer[index] = '\0';

    return buffer;
}



char **split_line(char *line)
{
  char* delim = " \t\n";
  int size = 128;
  int pos = 0;
  char **tokens = malloc(size * sizeof(char*));
  char *token;

  if (!tokens) {
    perror("allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, delim);
  while (token != NULL) {
    tokens[pos] = token;
    pos++;

    if (pos >= size) {
      size *= 2;
      tokens = realloc(tokens, size * sizeof(char*));
      if (!tokens) {
        perror("allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, delim);
  }
  tokens[pos] = NULL;
  return tokens;
}

void loop(void)
{
    char * clear = "clear";
    char * argsClear[3];
    argsClear[0] = clear;
    argsClear[1] = NULL;
    pid_t cpid = fork();
    if(cpid == 0) {
        execvp(argsClear[0],argsClear);
    }
    wait(NULL);
    printf("Shell\n");

    char *line;
    int status;

    do {
        getcwd(currentPath,2048);
        line = read_line();
        strcpy(commandHistory[lineNumber],line);
        lineNumber++;
        if(*line != '\0') {
            char** job = split_line(line);
            if(job == NULL){
                status = 1;
            }
            else status = execute(job);
        }
        else {
            status = 1;
        }
    } while (status);
}


int main(int argc, char **argv)
{
  loop();
  return 0;
}
