#include "../inc/minishell.h"

int ft_audit_input(char *str)
{
	int i;
	i = 0;
	bool flag = true;
	

	while (str[i])
	{
		if (str[i] == '\\')
		{
			printf ("%s", "error input. Try again");
			return 1;
		}
		if (str[i] == ';')
		{
			printf ("%s", "error input. Try again");
			return 1;
		}
		if (str[i] == '"')
		{
			flag = !flag;
		}
		i++;
	}
	if (flag == true)
		return 0;
	else
		return 1;
}



// Разбирает команду из ввода
void parse_command(char *input, t_command *command) 
{
    command->cmd = ft_split(input, ' ');
}

// Копирует текущие переменные окружения
void copy_environment(t_command *command) 
{
    char **new_envp;
    int i = 0;

    if (command->envp == NULL) return; // Нет данных для копирования

    // Подсчитываем количество переменных окружения
    while (command->envp[i] != NULL) {
        i++;
    }

    // Выделяем память для копии переменных окружения
    new_envp = malloc((i + 1) * sizeof(char *));
    if (new_envp == NULL) {
        perror("Failed to allocate memory for new envp");
        exit(EXIT_FAILURE);
    }

    for (int j = 0; j < i; j++) {
        new_envp[j] = strdup(command->envp[j]);  // Копирование каждой переменной окружения
        if (new_envp[j] == NULL) {
            perror("Failed to duplicate environment variable");
            exit(EXIT_FAILURE);
        }
    }
    new_envp[i] = NULL;  // NULL-terminated array

    // Освобождаем старый envp и обновляем его на новый
    ft_free_array(command->envp);
    command->envp = new_envp;
	//ft_free_array(new_envp);
}


void parse_input(char *input, t_command *command) 
{
    parse_command(input, command);
    copy_environment(command);
}
