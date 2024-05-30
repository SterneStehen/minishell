#include "../include/minishell.h"

// Функция для проверки существования файла
bool file_exists(char *filepath) {
    struct stat buffer;
    return (stat(filepath, &buffer) == 0);
}

// Функция для нахождения исполняемого файла
char *find_executable(char *command, char **paths) {
    char *filepath = NULL;
    for (int i = 0; paths[i]; i++) {
        filepath = malloc(strlen(paths[i]) + strlen(command) + 2);
        if (!filepath)
            return NULL;
        sprintf(filepath, "%s/%s", paths[i], command);

        if (file_exists(filepath))
            return filepath;

        free(filepath);
        filepath = NULL;
    }
    return NULL;
}
