#include "minishell.h"


static volatile sig_atomic_t g_keep_running = 1;

void handle_signal(int signal) {
    if (signal == SIGINT) {
        // Обработка нажатия Ctrl+C здесь
        g_keep_running = 0;
        write(STDOUT_FILENO, "\n", 1); // Переход на новую строку
        rl_on_new_line(); // Сброс позиции ввода в readline
        rl_replace_line("", 0); // Очистка текущей строки
        rl_redisplay(); // Повторное отображение приглашения
    }
}

void setup_signals() {
    struct sigaction sa;

    sa.sa_handler = &handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // Чтобы убедиться, что системные вызовы, такие как read(), будут перезапускаться после сигнала
    sigaction(SIGINT, &sa, NULL);
}


//int main(int argc, char **argv, char **envp) 
int main()
{
    // (void)argc;  // Явно игнорируем неиспользуемые переменные
    // (void)argv;

    char *input;
    t_command command;

    setup_signals();

    // Предполагается, что init_command инициализирует начальное состояние оболочки
    //t_command *initial_command = 
	//init_command(envp);
    //(void)initial_command; // Если initial_command не используется, игнорируем её

    while (g_keep_running) 
	{
        input = readline("minishell> ");
        if (!input) 
		{
            write(STDOUT_FILENO, "exit\n", 5);
            break;
        }
        if (*input) 
		{
            add_history(input);
        }

        parse_input(input, &command);
        execute_command(&command);

        free(input); // Освобождаем ввод после использования

        // Освобождение ресурсов команды, если это необходимо после каждой итерации
        //free_command(&command, 0);
    }

    // Если initial_command используется для удержания состояния, освобождаем здесь
    // if (initial_command) {
    //     free_command(initial_command);
    //     free(initial_command); // Убедитесь, что освободили всю выделенную память
    // }
	//free_command(&command);
    return 0;
}