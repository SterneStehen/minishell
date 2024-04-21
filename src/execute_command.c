#include "../inc/minishell.h"

int handle_builtin_commands(t_command *command) {
    if (strcmp(command->cmd[0], "cd") == 0) {
        handle_cd(command);
    } else if (strcmp(command->cmd[0], "echo") == 0) {
        handle_echo(command);
    } else if (strcmp(command->cmd[0], "pwd") == 0) {
        handle_pwd(command);
    } else if (strcmp(command->cmd[0], "history") == 0) {
        print_history();
    } else if (strcmp(command->cmd[0], "export") == 0 || strcmp(command->cmd[0], "unset") == 0) {
        fprintf(stderr, "minishell: %s not fully implemented\n", command->cmd[0]);
        return 1;
    } else if (strcmp(command->cmd[0], "env") == 0) {
        handle_env(command);
    } else if (strcmp(command->cmd[0], "exit") == 0) {
        free_exit(command);
    } else {
        return 0;  // Команда не является встроенной
    }

    return 1;  // Команда была встроенной и обработана
}

void execute_external_command(t_command *command) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Дочерний процесс: попытка выполнить внешнюю команду
        if (execve(command->cmd[0], command->cmd, command->envp) == -1) {
            perror("minishell: execve error");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        // Ошибка при попытке создания процесса
        perror("minishell: fork error");
    } else {
        // Родительский процесс: ожидание завершения дочернего процесса
        do {
            if (waitpid(pid, &status, 0) == -1) {
                perror("minishell: waitpid error");
                exit(EXIT_FAILURE);
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        if (WIFEXITED(status)) {
            printf("Process exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Process killed by signal %d\n", WTERMSIG(status));
        }
    }
}

void execute_command(t_command *command) 
{
    if (command->cmd == NULL || command->cmd[0] == NULL) 
	{
        return;  // Пустая команда
    }

    if (!handle_builtin_commands(command)) 
	{
        execute_external_command(command);  // Исполнение внешней команды
    }
}



// void execute_command(t_command *command) 
// {
//     pid_t pid;
//     int status;

//     if (command->cmd == NULL) 
// 	{
// 		return;
//     }

//     if (strcmp(command->cmd, "cd") == 0)
// 	{
//         if (command->args[1] != NULL) 
// 		{
//             if (chdir(command->args[1]) != 0) 
// 			{
//                 perror("minishell: cd");
//             }
//         } 
// 		else 
// 		{
//             fprintf(stderr, "minishell: cd: missing argument\n");
//         }
//     } 
// 	else if (strcmp(command->cmd, "echo") == 0) 
// 	{
//         int i = 1;
//         bool n_flag = false;
//         if (command->args[i] && strcmp(command->args[i], "-n") == 0)
// 		{
//             n_flag = true;
//             i++;
//         }
//         while (command->args[i]) 
// 		{
//             printf("%s", command->args[i]);
//             if (command->args[i + 1]) 
// 			{
//                 printf(" ");
//             }
//             i++;
//         }
//         if (!n_flag) 
// 		{
//             printf("\n");
//         }
//     } 
// 	else if (strcmp(command->cmd, "pwd") == 0) 
// 	{
//         char cwd[1024];
//         if (getcwd(cwd, sizeof(cwd)) != NULL) 
// 		{
//             printf("%s\n", cwd);
//         } 
// 		else 
// 		{
//             perror("minishell: pwd");
//         }
//     } 
// 	else if (ft_strcmp(command->cmd, "export") == 0 || ft_strcmp(command->cmd, "unset") == 0) 
// 	{
//         fprintf(stderr, "minishell: %s not fully implemented\n", command->cmd);
//         // Дальнейшая реализация 'export' и 'unset'
//     } 
// 	else if (ft_strcmp(command->cmd, "env") == 0) 
// 	{
//         for (int i = 0; environ[i]; i++) 
// 		{
//             printf("%s\n", environ[i]);
//         }
//     }
// 	else if (ft_strcmp(command->cmd, "exit") == 0)
// 	{
//         free_exit(command);
// 		exit(0);
//     }
// 	else if (ft_strcmp(command->cmd, "history") == 0) 
// 	{
//         print_history();
// 	}
// 	else 
// 	{
//         // Для внешних команд используем fork и execvp
//         pid = fork();
//         if (pid == 0) {
//             // Дочерний процесс: исполнение внешней команды
//             if (execvp(command->cmd, command->args) == -1) 
// 			{
//                 perror("minishell");
//                 exit(EXIT_FAILURE);
//             }
//         } 
// 		else if (pid < 0) 
// 		{
//             perror("minishell: fork");
//         } else 
// 		{
//             // Родительский процесс: ожидание завершения дочернего процесса
//             waitpid(pid, &status, 0);
//         }
//     }
// }
