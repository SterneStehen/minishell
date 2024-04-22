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

char *search_in_path(char *cmd) 
{
    if (strchr(cmd, '/') != NULL) 
	{
        return strdup(cmd);
    }

    // Получаем значение переменной PATH
    char *path_env = getenv("PATH");
    if (path_env == NULL) {
        fprintf(stderr, "PATH is not set.\n");
        return NULL;
    }

    // Создаём копию, так как strtok модифицирует исходную строку
    char *path = strdup(path_env);
    char *full_path = NULL;
    char *dir = strtok(path, ":");

    while (dir != NULL) {
        // Создаём полный путь до файла
        full_path = malloc(strlen(dir) + strlen(cmd) + 2); // +1 для '/' и +1 для '\0'
        if (full_path == NULL) {
            perror("malloc");
            break;
        }
        sprintf(full_path, "%s/%s", dir, cmd);
        
        // Проверяем, существует ли файл
        if (access(full_path, X_OK) == 0) {
            free(path);
            return full_path; // Найденный путь
        }

        free(full_path); // Освобождаем память, если файл не найден
        full_path = NULL;
        dir = strtok(NULL, ":");
    }

    free(path);
    return NULL; // Файл не найден в PATH
}

void parse_command(char *input, t_command *command) {
    bool in_single_quotes = false;
    bool in_double_quotes = false;
    char *start = input;
    int token_length = 0;
    int token_index = 0;

    // Выделение памяти для начального количества токенов
    command->cmd = malloc(sizeof(char *) * (ft_word_count(input, ' ') + 1));
    if (!command->cmd) return; // Проверка успешного выделения памяти

    for (int i = 0; input[i] != '\0'; i++) {
        char current_char = input[i];

        if (current_char == '\'' || current_char == '\"') {
            // Переключение состояния кавычек
            if (current_char == '\'' && !in_double_quotes) {
                in_single_quotes = !in_single_quotes;
            } else if (current_char == '\"' && !in_single_quotes) {
                in_double_quotes = !in_double_quotes;
            }
        } else if (current_char == ' ' && !in_single_quotes && !in_double_quotes) {
            // Завершаем текущий аргумент
            if (token_length > 0) {
                command->cmd[token_index++] = ft_substr(start, 0, token_length);
                token_length = 0;
                start = input + i + 1;
            }
        } else {
            token_length++;
        }
    }

    // Добавление последнего аргумента
    if (token_length > 0) {
        command->cmd[token_index++] = ft_substr(start, 0, token_length);
    }
    command->cmd[token_index] = NULL; // NULL-terminated массив

    // Проверка на незакрытые кавычки
    if (in_single_quotes || in_double_quotes) {
        printf("Syntax error: unclosed quote\n");
        ft_free_array(command->cmd);
        command->cmd = NULL;
        return;
    }

    // Обработка команды
    if (!include_commands(command)) {
        char *full_path = search_in_path(command->cmd[0]);
        if (full_path) {
            command->path = full_path;
        } else {
            printf("Command not found: %s\n", command->cmd[0]);
            ft_free_array(command->cmd);
            command->cmd = NULL;
        }
    } else {
        free(command->path);
        command->path = NULL;
    }
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
