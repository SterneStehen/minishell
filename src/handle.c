#include "../inc/minishell.h"

void handle_input(t_tools *tools, char *input) {
    g_global.in_cmd = 1;
    // Здесь нужно реализовать логику для лексинга и парсинга

    // Пример для обработки одной команды
    handle_command(tools, input);

    g_global.in_cmd = 0;
}


// void handle_cd(t_command *command) 
// {
//     if (command->cmd[1] != NULL) {
//         if (chdir(command->cmd[1]) != 0) 
// 		{
//             perror("minishell: cd");
//         }
//     } else {
//         fprintf(stderr, "minishell: cd: missing argument\n");
//     }
// }

// void handle_echo(t_command *command) {
//     int i = 1;
//     bool n_flag = false;

//     if (command->cmd[i] && strcmp(command->cmd[i], "-n") == 0) {
//         n_flag = true;
//         i++;
//     }

//     while (command->cmd[i]) {
//         printf("%s", command->cmd[i]);
//         if (command->cmd[++i]) {
//             printf(" ");
//         }
//     }

//     if (!n_flag) {
//         printf("\n");
//     }
// }

// void handle_pwd() {
//     char cwd[1024];
//     if (getcwd(cwd, sizeof(cwd)) != NULL) 
// 	{
//         printf("%s\n", cwd);
//     } else {
//         perror("minishell: pwd");
//     }
// }

// void handle_env() {
//     char **env = environ;
//     while (*env) {
//         printf("%s\n", *env);
//         env++;
//     }
// }

// void print_history() {
//     HIST_ENTRY **the_history = history_list();
//     if (the_history) {
//         for (int i = 0; the_history[i]; i++) 
// 		{
//             printf("%d: %s\n", i + 1, the_history[i]->line);
//         }
//     }
// }

// void free_exit(t_command *command) 
// {
//     if (command == NULL) 
// 	{
//         rl_clear_history();  // Очистка истории перед выходом, если структура команды пуста
//         exit(0);
//     }

//     free_command(command); // Освобождение ресурсов команды
//     rl_clear_history();    // Очистка истории readline
//     exit(0);
// }



