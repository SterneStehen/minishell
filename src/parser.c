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


// Функция для добавления простой команды в список команд
void add_simple_cmd(t_tools *tools, t_simple_cmds *cmd) {
    if (tools->simple_cmds == NULL) {
        tools->simple_cmds = cmd; // Если список пуст, устанавливаем первую команду
    } else {
        t_simple_cmds *temp = tools->simple_cmds;
        while (temp->next != NULL) { // Проходим до конца списка команд
            temp = temp->next;
        }
        temp->next = cmd; // Добавляем команду в конец списка
        cmd->prev = temp; // Устанавливаем указатель на предыдущую команду
    }
}

// Основная функция для синтаксического анализа строки
void parser(t_tools *tools) {
    t_lexer *current = tools->lexer_list; // Указатель на текущий элемент списка лексеров
    t_simple_cmds *cmd = NULL; // Указатель на текущую команду
    t_lexer *redirection = NULL; // Указатель на текущие перенаправления
    int arg_count = 0; // Счетчик аргументов

    while (current != NULL) { // Проходим по списку лексеров
        if (current->token == PIPE) { // Если токен - пайп
            if (cmd != NULL) {
                add_simple_cmd(tools, cmd); // Добавляем текущую команду в список команд
                cmd = NULL; // Сбрасываем указатель на команду
                arg_count = 0; // Сбрасываем счетчик аргументов
            }
        } else if (current->token == GREAT || current->token == GREAT_GREAT || current->token == LESS || current->token == LESS_LESS) {
            // Если токен - перенаправление
            if (cmd == NULL) {
                cmd = malloc(sizeof(t_simple_cmds)); // Выделяем память для новой команды
                ft_bzero(cmd, sizeof(t_simple_cmds)); // Инициализируем структуру нулями
            }
            t_lexer *new_redirection = malloc(sizeof(t_lexer)); // Выделяем память для нового перенаправления
            ft_memcpy(new_redirection, current, sizeof(t_lexer)); // Копируем текущий лексер в новое перенаправление
            new_redirection->next = NULL; // Устанавливаем указатель на следующий элемент
            if (redirection == NULL) {
                redirection = new_redirection; // Если список перенаправлений пуст, устанавливаем первое перенаправление
            } else {
                t_lexer *temp = redirection;
                while (temp->next != NULL) { // Проходим до конца списка перенаправлений
                    temp = temp->next;
                }
                temp->next = new_redirection; // Добавляем новое перенаправление в конец списка
            }
            cmd->num_redirections++; // Увеличиваем счетчик перенаправлений
        } else {
            // Если токен - аргумент команды
            if (cmd == NULL) {
                cmd = malloc(sizeof(t_simple_cmds)); // Выделяем память для новой команды
                ft_bzero(cmd, sizeof(t_simple_cmds)); // Инициализируем структуру нулями
            }
            cmd->str = realloc(cmd->str, sizeof(char *) * (arg_count + 2)); // Перевыделяем память для аргументов команды
            cmd->str[arg_count] = ft_strdup(current->str); // Копируем текущий аргумент в структуру команды
            cmd->str[arg_count + 1] = NULL; // Завершаем массив аргументов NULL
            arg_count++; // Увеличиваем счетчик аргументов
        }
        current = current->next; // Переходим к следующему лексеру
    }

    if (cmd != NULL) {
        add_simple_cmd(tools, cmd); // Добавляем последнюю команду в список команд
    }

    if (redirection != NULL) {
        tools->redirections = redirection; // Устанавливаем список перенаправлений в структуру tools
    }
}


// // Функция для добавления простой команды в список команд
// void add_simple_cmd(t_tools *tools, t_simple_cmds *cmd) {
//     if (tools->simple_cmds == NULL) {
//         tools->simple_cmds = cmd; // Если список пуст, устанавливаем первую команду
//     } else {
//         t_simple_cmds *temp = tools->simple_cmds;
//         while (temp->next != NULL) { // Проходим до конца списка команд
//             temp = temp->next;
//         }
//         temp->next = cmd; // Добавляем команду в конец списка
//         cmd->prev = temp; // Устанавливаем указатель на предыдущую команду
//     }
// }

// // Основная функция для синтаксического анализа строки
// void parser(t_tools *tools) {
//     t_lexer *current = tools->lexer_list; // Указатель на текущий элемент списка лексеров
//     t_simple_cmds *cmd = NULL; // Указатель на текущую команду
//     t_lexer *redirection = NULL; // Указатель на текущие перенаправления
//     int arg_count = 0; // Счетчик аргументов

//     while (current != NULL) { // Проходим по списку лексеров
//         if (current->token == PIPE) { // Если токен - пайп
//             if (cmd != NULL) {
//                 add_simple_cmd(tools, cmd); // Добавляем текущую команду в список команд
//                 cmd = NULL; // Сбрасываем указатель на команду
//                 arg_count = 0; // Сбрасываем счетчик аргументов
//             }
//         } else if (current->token == GREAT || current->token == GREAT_GREAT || current->token == LESS || current->token == LESS_LESS) {
//             // Если токен - перенаправление
//             if (cmd == NULL) {
//                 cmd = malloc(sizeof(t_simple_cmds)); // Выделяем память для новой команды
//                 ft_bzero(cmd, sizeof(t_simple_cmds)); // Инициализируем структуру нулями
//             }
//             t_lexer *new_redirection = malloc(sizeof(t_lexer)); // Выделяем память для нового перенаправления
//             ft_memcpy(new_redirection, current, sizeof(t_lexer)); // Копируем текущий лексер в новое перенаправление
//             new_redirection->next = NULL; // Устанавливаем указатель на следующий элемент
//             if (redirection == NULL) {
//                 redirection = new_redirection; // Если список перенаправлений пуст, устанавливаем первое перенаправление
//             } else {
//                 t_lexer *temp = redirection;
//                 while (temp->next != NULL) { // Проходим до конца списка перенаправлений
//                     temp = temp->next;
//                 }
//                 temp->next = new_redirection; // Добавляем новое перенаправление в конец списка
//             }
//             cmd->num_redirections++; // Увеличиваем счетчик перенаправлений
//         } else {
//             // Если токен - аргумент команды
//             if (cmd == NULL) {
//                 cmd = malloc(sizeof(t_simple_cmds)); // Выделяем память для новой команды
//                 ft_bzero(cmd, sizeof(t_simple_cmds)); // Инициализируем структуру нулями
//             }
//             cmd->str = realloc(cmd->str, sizeof(char *) * (arg_count + 2)); // Перевыделяем память для аргументов команды
//             cmd->str[arg_count] = ft_strdup(current->str); // Копируем текущий аргумент в структуру команды
//             cmd->str[arg_count + 1] = NULL; // Завершаем массив аргументов NULL
//             arg_count++; // Увеличиваем счетчик аргументов
//         }
//         current = current->next; // Переходим к следующему лексеру
//     }

//     if (cmd != NULL) {
//         add_simple_cmd(tools, cmd); // Добавляем последнюю команду в список команд
//     }

//     if (redirection != NULL) {
//         tools->redirections = redirection; // Устанавливаем список перенаправлений в структуру tools
//     }
// }
