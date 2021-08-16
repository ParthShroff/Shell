#ifndef BUILTIN
#define BUILTIN

extern char *builtin_str_output[];
extern int (*builtin_func_output[]) (char **);
extern char *builtin_str[];
extern int (*builtin_func[]) (char **);

int sh_num_builtins_output();
int sh_num_builtins();

// CD
int sh_cd(char **args);

// HELP
int sh_help(char **args);

// EXIT
int sh_exit(char **args);

// HISTORY
char* commandHistory[1024];
extern int lineNumber;
int sh_history(char ** args);

#endif
