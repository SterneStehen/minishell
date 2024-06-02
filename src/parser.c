/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <smoreron@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 21:51:20 by smoreron          #+#    #+#             */
/*   Updated: 2024/05/30 21:51:20 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../include/minishell.h"

// Прототипы функций
int count_special_symbols(t_lexer *lexer_list);
int count_arguments(t_lexer *lexer_list);
t_lexer *find_next_command(t_lexer *lexer_list);
char *join_strings(t_lexer *lexer_list, int count);
void add_redirection(t_tools *tools, t_lexer *lexer, t_simple_cmds *cmd);
void remove_redirections(t_tools *tools);
void initialize_command(t_tools *tools, t_lexer **lexer_list);
void handle_error(const char *error_message);
int parser(t_tools *tools);
void free_lexer_list(t_lexer *lexer_list);
void free_cmd_list(t_simple_cmds *cmd_list);
t_lexer *new_lexer_node(char *str, t_tokens token, int index);
t_simple_cmds *new_cmd_node(void);

// Функция для парсинга
int parser(t_tools *tools) {
    t_lexer *current = tools->lexer_list;
    tools->pipes = count_special_symbols(current);

    while (current) {
        if (current->token == PIPE) {
            current = current->next;
            continue;
        }

        initialize_command(tools, &current);
        current = find_next_command(current);
    }

    return 0; // Возвращаем 0 при успешном завершении
}

// Подсчет специальных символов (pipe)
int count_special_symbols(t_lexer *lexer_list) {
    int count = 0;
    while (lexer_list) {
        if (lexer_list->token == PIPE) {
            count++;
        }
        lexer_list = lexer_list->next;
    }
    return count;
}

// Подсчет аргументов до следующего специального символа
int count_arguments(t_lexer *lexer_list) {
    int count = 0;
    while (lexer_list && lexer_list->token != PIPE && lexer_list->token < GREAT) {
        count++;
        lexer_list = lexer_list->next;
    }
    return count;
}

// Поиск следующей команды
t_lexer *find_next_command(t_lexer *lexer_list) {
    while (lexer_list && lexer_list->token != PIPE) {
        lexer_list = lexer_list->next;
    }
    if (lexer_list) {
        return lexer_list->next;
    }
    return NULL;
}

// Объединение строк с пробелами
char *join_strings(t_lexer *lexer_list, int count) {
    int length = 0;
    t_lexer *current = lexer_list;

    for (int i = 0; i < count; i++) {
        length += strlen(current->str) + 1; // +1 for space or null terminator
        current = current->next;
    }

    char *result = (char *)malloc(length);
    if (!result) {
        perror("Failed to allocate memory for command arguments");
        return NULL;
    }

    result[0] = '\0';
    current = lexer_list;
    for (int i = 0; i < count; i++) {
        strcat(result, current->str);
        if (i < count - 1) {
            strcat(result, " ");
        }
        current = current->next;
    }

    return result;
}

// Добавление редирекций в список
void add_redirection(t_tools *tools, t_lexer *lexer, t_simple_cmds *cmd) {
    t_lexer *redirection_node = new_lexer_node(lexer->str, lexer->token, lexer->i);
    if (!redirection_node) {
        handle_error("Failed to allocate memory for redirection");
        return;
    }

    if (cmd->redirections == NULL) {
        cmd->redirections = redirection_node;
    } else {
        t_lexer *current = cmd->redirections;
        while (current->next) {
            current = current->next;
        }
        current->next = redirection_node;
        redirection_node->prev = current;
    }

    cmd->num_redirections++;
    if (lexer->token == LESS_LESS) {
        tools->heredoc = true;
    }
}

// Удаление редирекций из списка токенов
void remove_redirections(t_tools *tools) {
    t_lexer *current = tools->lexer_list;
    t_lexer *prev = NULL;
    while (current) {
        if (current->token >= GREAT) {
            t_lexer *to_remove = current;
            if (prev) {
                prev->next = current->next;
            } else {
                tools->lexer_list = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            }
            current = current->next;
            free(to_remove->str);
            free(to_remove);
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void initialize_command(t_tools *tools, t_lexer **lexer_list) {
    t_simple_cmds *cmd = new_cmd_node();
    if (!cmd) {
        handle_error("Failed to allocate memory for command");
        return;
    }

    int arg_count = count_arguments(*lexer_list);
    cmd->str = (char **)malloc((arg_count + 1) * sizeof(char *));
    if (!cmd->str) {
        handle_error("Failed to allocate memory for command arguments");
        free(cmd);
        return;
    }

    for (int i = 0; i < arg_count; i++) {
        cmd->str[i] = strdup((*lexer_list)->str);
        *lexer_list = (*lexer_list)->next;
    }
    cmd->str[arg_count] = NULL;

    if (tools->simple_cmds == NULL) {
        tools->simple_cmds = cmd;
    } else {
        t_simple_cmds *current = tools->simple_cmds;
        while (current->next) {
            current = current->next;
        }
        current->next = cmd;
        cmd->prev = current;
    }

    // Обработка редирекций для команды
    while (*lexer_list && (*lexer_list)->token >= GREAT) {
        add_redirection(tools, *lexer_list, cmd);
        *lexer_list = (*lexer_list)->next;
    }
}

// Обработка ошибок
void handle_error(const char *error_message) {
    fprintf(stderr, "%s\n", error_message);
    // Дополнительная логика обработки ошибок, если необходимо
}

// Создание нового узла лексемы
t_lexer *new_lexer_node(char *str, t_tokens token, int index) {
    t_lexer *node = (t_lexer *)malloc(sizeof(t_lexer));
    if (!node) {
        perror("Failed to allocate memory for lexer node");
        return NULL;
    }
    node->str = strdup(str);
    node->token = token;
    node->i = index;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// Создание нового узла команды
t_simple_cmds *new_cmd_node(void) {
    t_simple_cmds *cmd = (t_simple_cmds *)malloc(sizeof(t_simple_cmds));
    if (!cmd) {
        perror("Failed to allocate memory for command node");
        return NULL;
    }
    cmd->str = NULL;
    cmd->builtin = NULL;
    cmd->num_redirections = 0;
    cmd->hd_file_name = NULL;
    cmd->redirections = NULL;
    cmd->next = NULL;
    cmd->prev = NULL;
    return cmd;
}

void free_lexer_list(t_lexer *lexer_list) {
    t_lexer *temp;
    while (lexer_list) {
        temp = lexer_list;
        lexer_list = lexer_list->next;
        if (temp->str)
            free(temp->str);
        free(temp);
    }
}

// Освобождение списка команд
void free_cmd_list(t_simple_cmds *cmd_list) {
    t_simple_cmds *temp;
    int i;
    while (cmd_list) {
        temp = cmd_list;
        cmd_list = cmd_list->next;
        if (temp->str) {
            i = 0;
            while (temp->str[i]) {
                free(temp->str[i]);
                i++;
            }
            free(temp->str);
        }
        if (temp->redirections) {
            free_lexer_list(temp->redirections);
        }
        free(temp);
    }
}