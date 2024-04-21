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


int main(int ac, char **av, char **envv) {
    char *input;
    t_command *command = NULL;

    // Инициализация команды с аргументами командной строки и переменными окружения
    init_command(&command, ac, av, envv);

    while (g_keep_running) {
        input = readline("minishell> ");
        if (!input) {
            write(STDOUT_FILENO, "exit\n", 5);
            break;
        }
        if (*input) {
            add_history(input);
        }

        parse_input(input, command);
        execute_command(command);
        free(input); // Освобождаем введенную строку после использования

        // Не нужно освобождать command каждый раз в цикле, если оно используется повторно
    }

    // Очистка истории перед выходом
    rl_clear_history();
    
    // Освобождение ресурсов команды в конце работы
    if (command) {
        free_command(command); // Эта функция должна также освободить command->envp
        free(command); // Освобождение структуры command
        command = NULL;
    }

    return 0;
}