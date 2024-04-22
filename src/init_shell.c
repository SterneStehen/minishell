#include "../inc/minishell.h"

void init_command(t_command **command, int ac, char **av, char **envv) 
{
    (void)ac;
	(void)av;
	*command = malloc(sizeof(t_command));
    if (*command == NULL) 
	{
        perror("Failed to allocate memory for command");
        exit(EXIT_FAILURE);
    }
    (*command)->cmd = NULL;
    (*command)->envp = NULL;
	(*command)->path = NULL;

    // Копируем начальное окружение
    int i = 0;
    for (; envv[i] != NULL; i++); // Считаем переменные окружения

    (*command)->envp = malloc((i + 1) * sizeof(char *));
    if ((*command)->envp == NULL) {
        perror("Failed to allocate memory for envp");
        exit(EXIT_FAILURE);
    }

    for (int j = 0; j < i; j++) {
        (*command)->envp[j] = ft_strdup(envv[j]);
        if ((*command)->envp[j] == NULL) {
            perror("Failed to duplicate environment variable");
            exit(EXIT_FAILURE);
        }
    }
    (*command)->envp[i] = NULL;
}
