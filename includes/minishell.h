#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> // Для waitpid и связанных констант
#include <sys/stat.h> // Для stat и структуры stat
#include <stdlib.h>   // Для exit и EXIT_FAILURE
#include <string.h>   // Для strdup
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdbool.h>
#include "../libft/libft.h"

#define MAX_COMMAND_LENGTH 1024
extern char **environ;

// Define a structure for a command. This is just an example structure.
typedef struct s_command
{
    char *cmd;      // Command name
    char **args;   // Command arguments
	char **envp; 
} t_command;

// Function prototypes for functions defined in other .c files.
t_command *init_command(char **envp);
void execute_command(t_command *command);
void init_shell(void);
void display_prompt(void);
void parse_input(char *input, t_command *command); // Example parsing function prototype
void ft_free_array(char **array);
void free_command(t_command *command);
void free_execute_command(t_command *command);
void show_history();

#endif
