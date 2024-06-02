/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <smoreron@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 21:19:12 by smoreron          #+#    #+#             */
/*   Updated: 2024/05/23 21:19:12 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../include/minishell.h"



// Прототипы функций
void create_heredoc(t_simple_cmds *cmd);
void expand_environment_variables(t_simple_cmds *cmd, char **envp);


// Функция для получения значения переменной окружения
char *get_env_value(const char *name, char **envp) {
    size_t len = strlen(name);
    for (int i = 0; envp[i] != NULL; i++) {
        if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=') {
            return strdup(envp[i] + len + 1); // Возвращает значение переменной окружения
        }
    }
    return strdup(""); // Возвращает пустую строку, если переменная не найдена
}

// Функция для замены переменных окружения в аргументе
char *replace_env_variable(const char *arg, char **envp) {
    char *result = strdup(arg);
    if (!result) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    char *start = strchr(result, '$');
    while (start) {
        char *end = start + 1;
        while (*end && (isalnum(*end) || *end == '_')) {
            end++;
        }
        size_t var_name_len = end - start - 1;
        char var_name[var_name_len + 1];
        strncpy(var_name, start + 1, var_name_len);
        var_name[var_name_len] = '\0';

        char *value = get_env_value(var_name, envp);
        size_t new_len = strlen(result) - var_name_len + strlen(value);
        char *new_result = malloc(new_len + 1);
        if (!new_result) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        strncpy(new_result, result, start - result);
        strcpy(new_result + (start - result), value);
        strcpy(new_result + (start - result) + strlen(value), end);

        free(result);
        result = new_result;
        free(value);

        start = strchr(result, '$');
    }

    return result;
}

// Функция для расширения переменных окружения в аргументах команды
void expand_environment_variables(t_simple_cmds *cmd, char **envp) {
    for (int i = 0; cmd->str[i] != NULL; i++) {
        char *expanded_arg = replace_env_variable(cmd->str[i], envp);
        free(cmd->str[i]);
        cmd->str[i] = expanded_arg;
    }
}


void close_pipes(int pipes[][2], int num_pipes) {
    for (int i = 0; i < num_pipes; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
}

void dup_and_close(int old_fd, int new_fd) {
    if (dup2(old_fd, new_fd) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }
    close(old_fd);
}

void handle_redirections(t_simple_cmds *cmd) {
    t_lexer *redirection = cmd->redirections;
    while (redirection) {
        int fd = -1;
        if (redirection->token == GREAT) {
            fd = open(redirection->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        } else if (redirection->token == GREAT_GREAT) {
            fd = open(redirection->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
        } else if (redirection->token == LESS) {
            fd = open(redirection->str, O_RDONLY);
        } else if (redirection->token == LESS_LESS) {
            create_heredoc(cmd);
            fd = open(redirection->str, O_RDONLY);
        }

        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        if (redirection->token == GREAT || redirection->token == GREAT_GREAT) {
            dup_and_close(fd, STDOUT_FILENO);
        } else if (redirection->token == LESS || redirection->token == LESS_LESS) {
            dup_and_close(fd, STDIN_FILENO);
        }

        redirection = redirection->next;
    }
}

void execute_command(t_simple_cmds *cmd, t_tools *tools) {
    if (cmd->builtin) {
        cmd->builtin(tools, cmd);
        exit(EXIT_SUCCESS);
    } else {
        printf("Executing command: %s\n", cmd->str[0]); // Отладочная информация
        execvp(cmd->str[0], cmd->str);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}

void create_pipes(int pipes[][2], int num_pipes) {
    for (int i = 0; i < num_pipes; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
}

// Функция для генерации уникального имени файла
char *generate_temp_filename() {
    static int counter = 0;
    char *filename = malloc(64);
    if (!filename) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    sprintf(filename, "/tmp/heredoc_temp_%d_%d", getpid(), counter++);
    return filename;
}

// Функция для обработки heredoc
void create_heredoc(t_simple_cmds *cmd) {
    t_lexer *redir = cmd->redirections; // Указатель на список редиректов

    while (redir) {
        if (redir->token == LESS_LESS) {
            char *temp_filename = generate_temp_filename(); // Генерация уникального имени файла
            int temp_fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600); // Создание временного файла
            if (temp_fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }

            char *delimiter = redir->str; // Строка-делимитер
            char *line = NULL;
            size_t len = 0;
            ssize_t read_len;

            printf("heredoc> ");
            while ((read_len = getline(&line, &len, stdin)) != -1) {
                if (strncmp(line, delimiter, strlen(delimiter)) == 0 && line[strlen(delimiter)] == '\n') {
                    break; // Завершение ввода по делимитеру
                }
                write(temp_fd, line, read_len); // Запись строки в временный файл
                printf("heredoc> ");
            }

            free(line);
            close(temp_fd);

            // Замена строки редиректа на временный файл
            free(redir->str);
            redir->str = temp_filename;
        }
        redir = redir->next; // Переход к следующему редиректу
    }
}
void prepare_executor(t_tools *tools) {
    t_simple_cmds *cmd = tools->simple_cmds;
    int num_pipes = tools->pipes;
    int pipes[num_pipes][2];
    int i = 0;

    tools->pid = malloc(sizeof(int) * (num_pipes + 1));
    if (!tools->pid) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    create_pipes(pipes, num_pipes);

    while (cmd) {
        tools->pid[i] = fork();
        if (tools->pid[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (tools->pid[i] == 0) { // Дочерний процесс
            if (i > 0) {
                dup_and_close(pipes[i - 1][0], STDIN_FILENO);
            }
            if (cmd->next) {
                dup_and_close(pipes[i][1], STDOUT_FILENO);
            }
            close_pipes(pipes, num_pipes);
            expand_environment_variables(cmd, tools->envp); // Расширение переменных окружения
            handle_redirections(cmd);
            execute_command(cmd, tools);
        }
        cmd = cmd->next;
        i++;
    }

    close_pipes(pipes, num_pipes);

    for (i = 0; i <= num_pipes; i++) {
        waitpid(tools->pid[i], NULL, 0);
    }

    free(tools->pid);
}
