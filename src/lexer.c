/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <smoreron@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:23:50 by smoreron          #+#    #+#             */
/*   Updated: 2024/06/02 15:15:05 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft/libft.h"
#include "../include/minishell.h"

// Прототипы функций для работы с лексерами


int process_quotes(int i, char *str, char del) {
    int j = 0;
    if (str[i + j] == del) {
        j++;
        while (str[i + j] != del && str[i + j])
            j++;
        j++;
    }
    return j;
}

t_tokens audit_token(int c) {
    static int token_arr[3][2] = {
        {'|', PIPE},
        {'>', GREAT},
        {'<', LESS},
    };
    int i = 0;
    while (i < 3) {
        if (token_arr[i][0] == c)
            return token_arr[i][1];
        i++;
    }
    return 0;
}

int process_token(char *str, int i, t_lexer **lexer_list) {
    t_tokens token = audit_token(str[i]);
    if (token == GREAT && audit_token(str[i + 1]) == GREAT) {
        if (!new_list(NULL, GREAT_GREAT, lexer_list))
            return -1;
        return 2;
    } else if (token == LESS && audit_token(str[i + 1]) == LESS) {
        if (!new_list(NULL, LESS_LESS, lexer_list))
            return -1;
        return 2;
    } else if (token) {
        if (!new_list(NULL, token, lexer_list))
            return -1;
        return 1;
    }
    return 0;
}

int space(char c) {
    return c == ' ' || (c > 8 && c < 14);
}

int skip_whitespaces(char *str, int i) {
    int j = 0;
    while (space(str[i + j]))
        j++;
    return j;
}

int new_list(char *str, t_tokens token, t_lexer **lexer_list) {
    t_lexer *node = new_lex(str, token);
    if (!node)
        return 0;
    add_back(lexer_list, node);
    return 1;
}

int words(int i, char *str, t_lexer **lexer_list) {
    int j = 0;
    while (str[i + j] && !audit_token(str[i + j])) {
        j += process_quotes(i + j, str, 34);
        j += process_quotes(i + j, str, 39);
        if (space(str[i + j]))
            break;
        else
            j++;
    }
    if (!new_list(get_substring(str, i, j), 0, lexer_list))
        return -1;
    return j;
}

int token_reader(t_tools *tools) {
    int i = 0;
    int j;
    while (tools->args[i]) {
        j = 0;
        i += skip_whitespaces(tools->args, i);
        if (audit_token(tools->args[i]))
            j = process_token(tools->args, i, &tools->lexer_list);
        else
            j = words(i, tools->args, &tools->lexer_list);
        if (j < 0)
            return 0;
        i += j;
    }
    return 1;
}

// Реализация вспомогательных функций

t_lexer *new_lex(char *str, t_tokens token) {
    t_lexer *new = (t_lexer *)malloc(sizeof(t_lexer));
    if (!new)
        return NULL;
    new->str = str ? strdup(str) : NULL;
    new->token = token;
    new->i = 0;
    new->next = NULL;
    new->prev = NULL;
    return new;
}

void add_back(t_lexer **lexer_list, t_lexer *node) {
    t_lexer *temp;
    if (!lexer_list || !node)
        return;
    if (!*lexer_list) {
        *lexer_list = node;
        return;
    }
    temp = *lexer_list;
    while (temp->next)
        temp = temp->next;
    temp->next = node;
    node->prev = temp;
}

char *get_substring(char const *s, unsigned int start, size_t len) {
    char *sub;
    size_t i = 0;
    if (!s)
        return NULL;
    sub = (char *)malloc(len + 1);
    if (!sub)
        return NULL;
    while (i < len && s[start + i]) {
        sub[i] = s[start + i];
        i++;
    }
    sub[i] = '\0';
    return sub;
}