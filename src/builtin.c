#include "builtin.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void __attribute__((constructor)) builtin_init();

void builtin_init(void) {
    for(int i = 0; i < 1024; i++) {
        commandHistory[i] = (char*) malloc(1024);
    }
}


char *builtin_str_output[] = {
  "help",
  "history",
};

int (*builtin_func_output[]) (char **) = {
  &sh_help,
  &sh_history,
};

char *builtin_str[] = {
  "cd",
  "exit",
};

int (*builtin_func[]) (char **) = {
  &sh_cd,
  &sh_exit,
};

int sh_num_builtins_output() {
    return sizeof(builtin_str_output) / sizeof(char*);
}

int sh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char*);
}

int sh_cd(char **args) {
    if (args[1] == NULL) {
        char * home = getenv("HOME");
        if (chdir(home) != 0) {
            perror("sh");
        }
        return 1;
    } else {
        if(strcmp(args[1], "--help") == 0){ //Taken from the GNU cd help page
            printf("cd: cd [-L|[-P [-e]] [-@]] [dir] \n");
            return 1;
        }
        if (chdir(args[1]) != 0) {
            perror("sh");
        }
  }

  return 1;
}

int sh_help(char **args) {
    printf("SHELL\n");

    printf("Commands that have output:\n");
    for (int i = 0; i < sh_num_builtins_output(); i++) {
        printf("  %s\n", builtin_str_output[i]);
    }

    printf("Commands that have don't have output:\n");
    for (int i = 0; i < sh_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    return 1;
}

// EXIT
int sh_exit(char **args) {
    return 0;
}

// HISTORY
int lineNumber = 0;
int sh_history(char ** args) {
    for(int i = 0; i <  lineNumber; i++) {
        printf("%d: %s\n",i+1,commandHistory[i]);
    }
    return 1;
}

