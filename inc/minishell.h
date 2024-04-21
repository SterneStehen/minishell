#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdbool.h>
#include "libft/libft.h"

#define MAX_COMMAND_LENGTH 1024
extern char **environ;

typedef struct s_command
{
    char **cmd; 
	char **envp;
} t_command;

void init_command(t_command **command, int ac, char **av, char **envv);
void execute_command(t_command *command);
void init_shell(void);
void display_prompt(void);

void ft_free_array(char **arr);
void free_exit(t_command *command);
//void free_commands(t_command *command, int num);
void print_history();

//my_func
void handle_cd(t_command *command);
void handle_echo(t_command *command);
void handle_pwd();
void handle_env();

//new
void parse_input(char *input, t_command *command);
void copy_environment(t_command *command);
void parse_command(char *input, t_command *command);
void free_command(t_command *command);



#endif
