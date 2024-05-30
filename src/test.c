#include "../include/minishell.h"
# include "../include/libft/libft.h"
#include <assert.h>

// Тест функции is_whitespace
void test_is_whitespace() {
    assert(is_space(' ') == true);
    assert(is_space('\t') == true);
    assert(is_space('\n') == true);
    assert(is_space('a') == false);
    assert(is_space('1') == false);
    printf("test_is_whitespace passed\n");
}

// Тест функции skip_whitespace
void test_skip_whitespace() {
    const char *str = "   abc";
    next_space(&str);
    assert(*str == 'a');
    str = "\t\n  \txyz";
    next_space(&str);
    assert(*str == 'x');
    printf("test_skip_whitespace passed\n");
}

// Тест функции add_lexer_node
void test_add_lexer_node() {
    t_lexer *lexer_list = NULL;

    assert(add_lexer(&lexer_list, "word", -1) == 1);
    assert(lexer_list != NULL);
    assert(strcmp(lexer_list->str, "word") == 0);
    assert(lexer_list->token == -1);

    assert(add_lexer(&lexer_list, "|", PIPE) == 1);
    assert(lexer_list->next != NULL);
    assert(strcmp(lexer_list->next->str, "|") == 0);
    assert(lexer_list->next->token == PIPE);

    // Освобождение памяти
    free(lexer_list->str);
    free(lexer_list->next->str);
    free(lexer_list->next);
    free(lexer_list);

    printf("test_add_lexer_node passed\n");
}

// Тест функции handle_quotes
void test_handle_quotes() {
    const char *str = "\"hello world\"";
    char result[50];
    char *res_ptr = result;
    handle_quotes(&str, &res_ptr, '"');
    *res_ptr = '\0';
    assert(strcmp(result, "hello world") == 0);

    str = "'another test'";
    res_ptr = result;
    handle_quotes(&str, &res_ptr, '\'');
    *res_ptr = '\0';
    assert(strcmp(result, "another test") == 0);

    printf("test_handle_quotes passed\n");
}

// Тест функции get_token
void test_get_token() {
    const char *str = "|";
    assert(get_token(&str) == PIPE);
    assert(*str == '\0');

    str = ">";
    assert(get_token(&str) == GREAT);
    assert(*str == '\0');

    str = ">>";
    assert(get_token(&str) == GREAT_GREAT);
    assert(*str == '\0');

    str = "<";
    assert(get_token(&str) == LESS);
    assert(*str == '\0');

    str = "<<";
    assert(get_token(&str) == LESS_LESS);
    assert(*str == '\0');

    str = "abc";
    assert(get_token(&str) == -1);
    assert(strcmp(str, "abc") == 0);

    printf("test_get_token passed\n");
}

// Вспомогательная функция для тестирования token_reader
void print_lexer_list(t_lexer *lexer_list) {
    while (lexer_list != NULL) {
        printf("Token: %d, Str: %s\n", lexer_list->token, lexer_list->str);
        lexer_list = lexer_list->next;
    }
}

// Тест функции token_reader
void test_token_reader() {
    t_tools tools;
    tools.args = strdup("echo \"Hello World\" | grep Hello > output.txt");

    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);

    print_lexer_list(tools.lexer_list);

    // Освобождение памяти
    t_lexer *temp;
    while (tools.lexer_list != NULL) {
        temp = tools.lexer_list;
        tools.lexer_list = tools.lexer_list->next;
        free(temp->str);
        free(temp);
    }
    free(tools.args);

    printf("test_token_reader passed\n");
}


// Вспомогательные функции для проверки
void print_array(char **arr) {
    for (int i = 0; arr[i] != NULL; i++) {
        printf("%s\n", arr[i]);
    }
}

void test_init_tools(char **envp) {
    t_tools tools;

    printf("hi tools");
    // Инициализация структуры tools
    init_tools(&tools, envp);

    // Проверка копирования переменных окружения
    printf("Testing environment variables copy:\n");
    print_array(tools.envp);

    // Проверка инициализации PWD и OLDPWD
    printf("\nTesting PWD and OLDPWD initialization:\n");
    printf("PWD: %s\n", tools.pwd);
    printf("OLDPWD: %s\n", tools.old_pwd);

    // Проверка парсинга переменной PATH
    printf("\nTesting PATH initialization:\n");
    print_array(tools.paths);

    // Освобождение ресурсов
    free(tools.pwd);
    free(tools.old_pwd);
    free_array(tools.envp);
    free_array(tools.paths);
}



int main(int argc, char **argv, char **envp)
 {
    if (argc > 10)
        return 0;
    (void) *argv;
    printf("%s", "hi main");
    test_init_tools(envp);
    test_is_whitespace();
    test_skip_whitespace();
    test_add_lexer_node();
    test_handle_quotes();
    test_get_token();
    test_token_reader();
    return 0;
}
