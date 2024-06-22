#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// // Определение типов токенов
// typedef enum e_tokens
// {
//     PIPE,
//     GREAT,
//     LESS,
//     GREAT_GREAT,
//     LESS_LESS
// } t_tokens;

// // Структура для представления токенов
// typedef struct s_lexer
// {
//     char            *str;
//     t_tokens        token;
//     int             i;
//     struct s_lexer  *next;
//     struct s_lexer  *prev;
// } t_lexer;

// // Структура для инструментов
// typedef struct s_tools
// {
//     char        *args;
//     t_lexer     *lexer_list;
// } t_tools;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    PIPE,
    GREAT,
    LESS,
    GREAT_GREAT,
    LESS_LESS
} t_tokens;

typedef struct s_lexer {
    char *str;
    t_tokens token;
    int index;
    struct s_lexer *next;
    struct s_lexer *prev;
} t_lexer;

typedef struct {
    char *args;
    t_lexer *lexer_list;
} t_tools;




t_lexer *create_lexer_node(char *str, t_tokens token);
void append_lexer_node(t_lexer **lexer_list, t_lexer *node);
char *extract_substring(const char *s, unsigned int start, size_t len);
int process_quotes(const char *str, int start, char quote_char);
t_tokens identify_token(int c);
int handle_token(const char *str, int i, t_lexer **lexer_list);
int is_whitespace(char c);
int skip_spaces(const char *str, int i);
int add_lexer_node(char *str, t_tokens token, t_lexer **lexer_list);
int handle_words(int i, const char *str, t_lexer **lexer_list);
int parse_tokens(t_tools *tools);
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
    new_node->index = 0;
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

int process_quotes(const char *str, int start, char quote_char)
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
        offset += process_quotes(str, i + offset, 34); // double quotes
        offset += process_quotes(str, i + offset, 39); // single quotes
        if (is_whitespace(str[i + offset]))
            break;
        offset++;
    }
    if (!add_lexer_node(extract_substring(str, i, offset), 0, lexer_list))
        return -1;
    return offset;
}

int parse_tokens(t_tools *tools)
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

void print_lexer_list(t_lexer *lexer_list)
{
    t_lexer *temp = lexer_list;
    while (temp) {
        printf("Token: %d, String: %s\n", temp->token, temp->str);
        temp = temp->next;
    }
}


int main() {
    t_tools tools;

    // Первый тест
    tools.args = strdup("echo \"Hello, World!\" \"hello\" | grep Hello >> output.txt");
    tools.lexer_list = NULL;

    printf("Test 1:\n");
    if (!parse_tokens(&tools)) {
        fprintf(stderr, "Error parsing tokens in test 1\n");
    } else {
        print_lexer_list(tools.lexer_list);
    }

    // Освобождение памяти после первого теста
    t_lexer *temp;
    while (tools.lexer_list) {
        temp = tools.lexer_list;
        tools.lexer_list = tools.lexer_list->next;
        free(temp->str);
        free(temp);
    }
    free(tools.args);

    // Второй тест
    tools.args = strdup("ls -la | grep txt < files.txt");
    tools.lexer_list = NULL;

    printf("\nTest 2:\n");
    if (!parse_tokens(&tools)) {
        fprintf(stderr, "Error parsing tokens in test 2\n");
    } else {
        print_lexer_list(tools.lexer_list);
    }

    // Освобождение памяти после второго теста
    while (tools.lexer_list) {
        temp = tools.lexer_list;
        tools.lexer_list = tools.lexer_list->next;
        free(temp->str);
        free(temp);
    }
    free(tools.args);

    // Третий тест
    tools.args = strdup("cat file1.txt file2.txt | sort | uniq > sorted.txt");
    tools.lexer_list = NULL;

    printf("\nTest 3:\n");
    if (!parse_tokens(&tools)) {
        fprintf(stderr, "Error parsing tokens in test 3\n");
    } else {
        print_lexer_list(tools.lexer_list);
    }

    // Освобождение памяти после третьего теста
    while (tools.lexer_list) {
        temp = tools.lexer_list;
        tools.lexer_list = tools.lexer_list->next;
        free(temp->str);
        free(temp);
    }
    free(tools.args);

    return 0;
}
