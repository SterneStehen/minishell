#include "minishell.h"

// Функция для подсчёта количества переменных окружения
int count_envp(char **envp)
{
    int count = 0;
    while (envp[count] != NULL) 
	{
        count++;
    }
    return count;
}

t_command *init_command(char **envp) 
{
    int env_count = count_envp(envp);
    t_command *command = malloc(sizeof(t_command));
    if (!command) {
        perror("Ошибка выделения памяти для команды");
        exit(EXIT_FAILURE);
    }

    // Инициализация полей команды
    command->cmd = NULL;  // Пока что нет команды
    command->args = NULL; // Пока что нет аргументов

    // Выделение памяти для копии переменных окружения
    command->envp = malloc((env_count + 1) * sizeof(char *));
    if (!command->envp) {
        perror("Ошибка выделения памяти для переменных окружения");
        free(command);
        exit(EXIT_FAILURE);
    }

    // Копирование переменных окружения
    for (int i = 0; i < env_count; i++) {
        command->envp[i] = strdup(envp[i]);
        if (!command->envp[i]) {
            perror("Ошибка копирования переменной окружения");
            while (i--) free(command->envp[i]); // Освобождение ранее выделенной памяти
            free(command->envp);
            free(command);
            exit(EXIT_FAILURE);
        }
    }
    command->envp[env_count] = NULL; // NULL-терминатор для массива переменных окружения

    return command;
}
