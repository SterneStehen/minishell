#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Определение токенов (ранее t_tokens)
typedef enum s_tokens {
    PIPE = 1,
    GREAT,
    GREAT_GREAT,
    LESS,
    LESS_LESS,
    INVALID = -1
} t_tokens;

// Структура для лексем (ранее t_lexer)
typedef struct s_lexer {
    char *str;
    t_tokens token;
    int i;
    struct s_lexer *next;
    struct s_lexer *prev;
} t_lexer;

// Объединенная структура инструментов (ранее t_tools и t_parser_tools)
typedef struct s_tools {
    char *args;
    char **paths;
    char **envp;
    struct s_simple_cmds *simple_cmds;
    t_lexer *lexer_list;
    t_lexer *redirections;
    int num_redirections;
    char *pwd;
    char *old_pwd;
    int pipes;
    int *pid;
    bool heredoc;
    bool reset;
} t_tools;


// Прототипы объединенных функций для работы с лексерами
t_lexer *new_lex(char *str, t_tokens token);
void add_back(t_lexer **lexer_list, t_lexer *node);
char *get_substring(char const *s, unsigned int start, size_t len);
int new_list(char *str, t_tokens token, t_lexer **lexer_list);
int process_quotes(int i, char *str, char del);
t_tokens audit_token(int c);
int process_token(char *str, int i, t_lexer **lexer_list);
int space(char c);
int skip_whitespaces(char *str, int i);
int words(int i, char *str, t_lexer **lexer_list);

// Определение объединенных функций

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

// Основная функция для тестирования работы лексера
int main() {
    t_tools tools;
    t_lexer *lexer;
    t_lexer *temp;

    // Тестовый случай 1
    printf("Тестовый случай 1:\n");
    tools.args = "echo \"hello world\" | grep hello > output.txt";
    tools.lexer_list = NULL;
    if (!token_reader(&tools)) {
        fprintf(stderr, "Ошибка чтения токенов.\n");
        return 1;
    }

    lexer = tools.lexer_list;
    while (lexer) {
        if (lexer->token)
            printf("Токен: %d\n", lexer->token);
        else
            printf("Строка: %s\n", lexer->str);
        temp = lexer;
        lexer = lexer->next;
        free(temp->str);
        free(temp);
    }

    // Тестовый случай 2
    printf("\nТестовый случай 2:\n");
    tools.args = "cat file.txt | sort | uniq > result.txt";
    tools.lexer_list = NULL;
    if (!token_reader(&tools)) {
        fprintf(stderr, "Ошибка чтения токенов.\n");
        return 1;
    }

    lexer = tools.lexer_list;
    while (lexer) {
        if (lexer->token)
            printf("Токен: %d\n", lexer->token);
        else
            printf("Строка: %s\n", lexer->str);
        temp = lexer;
        lexer = lexer->next;
        free(temp->str);
        free(temp);
    }

    // Тестовый случай 3
    printf("\nТестовый случай 3:\n");
    tools.args = "echo 'single quoted string' | wc -l";
    tools.lexer_list = NULL;
    if (!token_reader(&tools)) {
        fprintf(stderr, "Ошибка чтения токенов.\n");
        return 1;
    }

    lexer = tools.lexer_list;
    while (lexer) {
        if (lexer->token)
            printf("Токен: %d\n", lexer->token);
        else
            printf("Строка: %s\n", lexer->str);
        temp = lexer;
        lexer = lexer->next;
        free(temp->str);
        free(temp);
    }

    // Тестовый случай 4
    printf("\nТестовый случай 4:\n");
    tools.args = "   ls   -la   |   grep '^d'   ";
    tools.lexer_list = NULL;
    if (!token_reader(&tools)) {
        fprintf(stderr, "Ошибка чтения токенов.\n");
        return 1;
    }

    lexer = tools.lexer_list;
    while (lexer) {
        if (lexer->token)
            printf("Токен: %d\n", lexer->token);
        else
            printf("Строка: %s\n", lexer->str);
        temp = lexer;
        lexer = lexer->next;
        free(temp->str);
        free(temp);
    }

    // Тестовый случай 5
    printf("\nТестовый случай 5:\n");
    tools.args = "echo 'hello world' > file.txt";
    tools.lexer_list = NULL;
    if (!token_reader(&tools)) {
        fprintf(stderr, "Ошибка чтения токенов.\n");
        return 1;
    }

    lexer = tools.lexer_list;
    while (lexer) {
        if (lexer->token)
            printf("Токен: %d\n", lexer->token);
        else
            printf("Строка: %s\n", lexer->str);
        temp = lexer;
        lexer = lexer->next;
        free(temp->str);
        free(temp);
    }

    // Тестовый случай 6
    printf("\nТестовый случай 6:\n");
    tools.args = "grep \"pattern\" < input.txt";
    tools.lexer_list = NULL;
    if (!token_reader(&tools)) {
        fprintf(stderr, "Ошибка чтения токенов.\n");
        return 1;
    }

    lexer = tools.lexer_list;
    while (lexer) {
        if (lexer->token)
            printf("Токен: %d\n", lexer->token);
        else
            printf("Строка: %s\n", lexer->str);
        temp = lexer;
        lexer = lexer->next;
        free(temp->str);
        free(temp);
    }

    // Тестовый случай 7
    printf("\nТестовый случай 7:\n");
    tools.args = "echo \"hello\" | tee output.txt";
    tools.lexer_list = NULL;
    if (!token_reader(&tools)) {
        fprintf(stderr, "Ошибка чтения токенов.\n");
        return 1;
    }

    lexer = tools.lexer_list;
    while (lexer) {
        if (lexer->token)
            printf("Токен: %d\n", lexer->token);
        else
            printf("Строка: %s\n", lexer->str);
        temp = lexer;
        lexer = lexer->next;
        free(temp->str);
        free(temp);
    }

    return 0;
}
