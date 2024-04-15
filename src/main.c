#include "../inc/minishell.h"

static volatile sig_atomic_t g_keep_running = 1;


void handle_signal(int signal) 
{
    if (signal == SIGINT) 
	{
        g_keep_running = 0;
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void setup_signals() 
{
    struct sigaction sa;

    sa.sa_handler = &handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
}

int main() 
{
    char *input;
    t_command *stack = NULL;

    while (g_keep_running) 
	{
        input = readline("minishell> ");
        if (!input) 
		{
            write(STDOUT_FILENO, "exit\n", 5);
            break;
        }
        if (input && *input) 
		{
    		add_history(input);
		}
		parse_input(input, &stack);
        free(input);
        stack = NULL;
    }
	rl_clear_history();
    return 0;
}
