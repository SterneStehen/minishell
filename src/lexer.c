/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <smoreron@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:23:50 by smoreron          #+#    #+#             */
/*   Updated: 2024/06/11 12:45:25 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft/libft.h"
#include "../include/minishell.h"



t_lexer *create_lexer_node(char *str, t_tokens token);
void append_lexer_node(t_lexer **lexer_list, t_lexer *node);
char *extract_substring(const char *s, unsigned int start, size_t len);
int process_quotes_dub(const char *str, int start, char quote_char);
t_tokens identify_token(int c);
int handle_token(const char *str, int i, t_lexer **lexer_list);
int is_whitespace(char c);
int skip_spaces(const char *str, int i);
int add_lexer_node(char *str, t_tokens token, t_lexer **lexer_list);
int handle_words(int i, const char *str, t_lexer **lexer_list);
int token_reader(t_tools *tools);
void print_lexer_list(t_lexer *lexer_list);


t_tokens identify_token(int c)
{
    if (c == '|') 
    {
        return PIPE;
    } 
    else if (c == '>') 
    {
        return GREAT;
    } 
    else if (c == '<') 
    {
        return LESS;
    } 
    else 
    {
        return 0;
    }
}


t_lexer *create_lexer_node(char *str, t_tokens token)
{
    t_lexer *new_node = (t_lexer *)malloc(sizeof(t_lexer));
    if (!new_node)
        return NULL;
    new_node->str = str ? strdup(str) : NULL;
    new_node->token = token;
    new_node->i = 0;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

void append_lexer_node(t_lexer **lexer_list, t_lexer *node)
{
    if (!lexer_list || !node)
        return;
    if (!*lexer_list) {
        *lexer_list = node;
        return;
    }
    t_lexer *temp = *lexer_list;
    while (temp->next)
        temp = temp->next;
    temp->next = node;
    node->prev = temp;
}

char *extract_substring(const char *s, unsigned int first, size_t len)
{
    if (!s)
        return NULL;
    char *sub = (char *)malloc(len + 1);
    if (!sub)
        return NULL;
    for (size_t i = 0; i < len && s[first + i]; i++)
        sub[i] = s[first + i];
    sub[len] = '\0';
    return sub;
}

int process_quotes_dub(const char *str, int start, char quote_char)
{
    int offset = 0;
    if (str[start + offset] == quote_char) {
        offset++;
        while (str[start + offset] != quote_char && str[start + offset])
            offset++;
        offset++;
    }
    return offset;
}

int handle_token(const char *str, int i, t_lexer **lexer_list)
{
    t_tokens token = identify_token(str[i]);
    if (token == GREAT && identify_token(str[i + 1]) == GREAT) {
        if (!create_lexer_node(NULL, GREAT_GREAT))
            return -1;
        append_lexer_node(lexer_list, create_lexer_node(NULL, GREAT_GREAT));
        return 2;
    } else if (token == LESS && identify_token(str[i + 1]) == LESS) {
        if (!create_lexer_node(NULL, LESS_LESS))
            return -1;
        append_lexer_node(lexer_list, create_lexer_node(NULL, LESS_LESS));
        return 2;
    } else if (token) {
        append_lexer_node(lexer_list, create_lexer_node(NULL, token));
        return 1;
    }
    return 0;
}
int is_whitespace(char c)
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r') {
        return 1;
    }
    return 0;
}


int skip_spaces(const char *str, int i)
{
    int offset = 0;
    while (is_whitespace(str[i + offset]))
        offset++;
    return offset;
}

int add_lexer_node(char *str, t_tokens token, t_lexer **lexer_list)
{
    t_lexer *node = create_lexer_node(str, token);
    if (!node)
        return 0;
    append_lexer_node(lexer_list, node);
    return 1;
}

int handle_words(int i, const char *str, t_lexer **lexer_list)
{
    int offset = 0;
    while (str[i + offset] && !identify_token(str[i + offset])) {
        offset += process_quotes_dub(str, i + offset, 34); // double quotes
        offset += process_quotes_dub(str, i + offset, 39); // single quotes
        if (is_whitespace(str[i + offset]))
            break;
        offset++;
    }
    if (!add_lexer_node(extract_substring(str, i, offset), 0, lexer_list))
        return -1;
    return offset;
}

int token_reader(t_tools *tools)
{
    int i = 0;
    while (tools->args[i]) {
        i += skip_spaces(tools->args, i);
        int offset = 0;
        if (identify_token(tools->args[i]))
            offset = handle_token(tools->args, i, &tools->lexer_list);
        else
            offset = handle_words(i, tools->args, &tools->lexer_list);
        if (offset < 0)
            return 0;
        i += offset;
    }
    return 1;
}