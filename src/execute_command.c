

#include "minishell.h"

void execute_command(t_command *command) {
    pid_t pid;
    int status;

    if (command->cmd == NULL) 
	{
		return;
    }

    if (strcmp(command->cmd, "cd") == 0)
	{
        if (command->args[1] != NULL) 
		{
            if (chdir(command->args[1]) != 0) 
			{
                perror("minishell: cd");
            }
        } 
		else 
		{
            fprintf(stderr, "minishell: cd: missing argument\n");
        }
    } 
	else if (strcmp(command->cmd, "echo") == 0) 
	{
        int i = 1;
        bool n_flag = false;
        if (command->args[i] && strcmp(command->args[i], "-n") == 0)
		{
            n_flag = true;
            i++;
        }
        while (command->args[i]) 
		{
            printf("%s", command->args[i]);
            if (command->args[i + 1]) 
			{
                printf(" ");
            }
            i++;
        }
        if (!n_flag) 
		{
            printf("\n");
        }
    } 
	else if (strcmp(command->cmd, "pwd") == 0) 
	{
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) 
		{
            printf("%s\n", cwd);
        } 
		else 
		{
            perror("minishell: pwd");
        }
    } 
	else if (strcmp(command->cmd, "history") == 0) 
	{
        void show_history();
    } 
	else if (strcmp(command->cmd, "export") == 0 || strcmp(command->cmd, "unset") == 0) 
	{
        fprintf(stderr, "minishell: %s not fully implemented\n", command->cmd);
        // Дальнейшая реализация 'export' и 'unset'
    } 
	else if (strcmp(command->cmd, "env") == 0) 
	{
        // Вывод переменных окружения
        for (int i = 0; environ[i]; i++) {
            printf("%s\n", environ[i]);
        }
    }
	else if (strcmp(command->cmd, "exit") == 0)
	{
        exit(0);
    }
	else 
	{
        // Для внешних команд используем fork и execvp
        pid = fork();
        if (pid == 0) {
            // Дочерний процесс: исполнение внешней команды
            if (execvp(command->cmd, command->args) == -1) 
			{
                perror("minishell");
                exit(EXIT_FAILURE);
            }
        } 
		else if (pid < 0) 
		{
            // Ошибка при создании процесса
            perror("minishell: fork");
        } else {
            // Родительский процесс: ожидание завершения дочернего процесса
            waitpid(pid, &status, 0);
        }
    }
	free_execute_command(command);
}
