#include "../include/minishell.h"



// Прототипы функций


// Остальные вспомогательные функции

// Вспомогательная функция для копирования массива строк
char	**dupl_arr(char **arr)
{
	char **copy;
	size_t i = 0;

	// Подсчитываем количество строк в исходном массиве
	while (arr[i] != NULL)
		i++;

	// Выделяем память для нового массива строк
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);

	// Копируем строки
	for (i = 0; arr[i] != NULL; i++)
	{
		copy[i] = strdup(arr[i]);
		if (copy[i] == NULL)
		{
			free_array(copy);
			return (NULL);
		}
	}
	// Завершаем массив NULL
	copy[i] = NULL;

	return (copy);
}

// Вспомогательная функция для освобождения массива строк
void	free_array(char **array)
{
	if (array)
	{
		for (size_t i = 0; array[i] != NULL; i++)
		{
			free(array[i]);
		}
		free(array);
	}
}

// Улучшенная функция для инициализации PWD и OLDPWD
int	find_PWD(t_tools *tools)
{
	int i = 0;

	free_PWD(tools);

	while (tools->envp[i])
	{
		if (!strncmp(tools->envp[i], "PWD=", 4))
		{
			tools->pwd = strdup(tools->envp[i] + 4);
			if (!tools->pwd)
			{
				free_PWD(tools);
				return (0);
			}
		}
		if (!strncmp(tools->envp[i], "OLDPWD=", 7))
		{
			tools->old_pwd = strdup(tools->envp[i] + 7);
			if (!tools->old_pwd)
			{
				free_PWD(tools);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

// Функция для освобождения PWD и OLDPWD
void	free_PWD(t_tools *tools)
{
	if (tools->pwd)
	{
		free(tools->pwd);
		tools->pwd = NULL;
	}
	if (tools->old_pwd)
	{
		free(tools->old_pwd);
		tools->old_pwd = NULL;
	}
}

// Объединенная функция для нахождения переменной PATH и инициализации путей
int	find_paths(t_tools *tools)
{
	char *path_from_envp = NULL;
	int i;
	char *tmp;

	// Поиск переменной PATH в envp
	for (int j = 0; tools->envp[j]; j++)
	{
		if (strncmp(tools->envp[j], "PATH=", 5) == 0)
		{
			path_from_envp = strdup(tools->envp[j] + 5);
			break ;
		}
	}

	if (!path_from_envp)
	{
		fprintf(stderr, "Error: PATH variable not found in environment.\n");
		return (EXIT_FAILURE);
	}

	tools->paths = ft_split(path_from_envp, ':');
	free(path_from_envp);

	if (!tools->paths)
	{
		fprintf(stderr, "Error: Failed to split PATH variable.\n");
		return (EXIT_FAILURE);
	}

	for (i = 0; tools->paths[i]; i++)
	{
		size_t len = ft_strlen(tools->paths[i]);
		if (len > 0 && tools->paths[i][len - 1] != '/')
		{
			tmp = ft_strjoin(tools->paths[i], "/");
			if (!tmp)
			{
				fprintf(stderr,
					"Error: Failed to allocate memory for path.\n");
				free_array(tools->paths);
				return (EXIT_FAILURE);
			}
			free(tools->paths[i]);
			tools->paths[i] = tmp;
		}
	}

	return (EXIT_SUCCESS);
}

// Функция для инициализации структуры t_tools
void	init_tools(t_tools *tools, char **envp)
{
	tools->args = NULL;
	tools->paths = NULL;
	tools->envp = dupl_arr(envp); // Копирование envp
	tools->simple_cmds = NULL;
	tools->lexer_list = NULL;
	tools->redirections = NULL;
	tools->num_redirections = 0;
	tools->pwd = NULL;
	tools->old_pwd = NULL;
	tools->pipes = 0;
	tools->pid = NULL;
	tools->heredoc = false;
	tools->reset = false;

	printf("hi init");

	// Инициализация PWD и OLDPWD
	if (!find_PWD(tools))
	{
		fprintf(stderr, "Error initializing PWD and OLDPWD\n");
		free_array(tools->envp);
		exit(EXIT_FAILURE);
	}

	// Инициализация путей
	if (find_paths(tools) != EXIT_SUCCESS)
	{
		fprintf(stderr, "Error initializing paths\n");
		free(tools->pwd);
		free(tools->old_pwd);
		free_array(tools->envp);
		exit(EXIT_FAILURE);
	}
}