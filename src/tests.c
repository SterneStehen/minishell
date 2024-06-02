#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/minishell.h"

// Вспомогательная функция для вывода содержимого списка лексеров
void print_lexer_list(t_lexer *lexer_list) {
    while (lexer_list != NULL) {
        printf("Token: %d, Str: %s\n", lexer_list->token, lexer_list->str);
        lexer_list = lexer_list->next;
    }
}

// Вспомогательная функция для вывода содержимого простых команд
void print_simple_cmds(t_simple_cmds *cmds) {
    while (cmds != NULL) {
        printf("Command: ");
        for (int i = 0; cmds->str[i] != NULL; i++) {
            printf("%s ", cmds->str[i]);
        }
        printf("\nRedirections: ");
        t_lexer *redir = cmds->redirections;
        while (redir != NULL) {
            printf("Token: %d, Str: %s ", redir->token, redir->str);
            redir = redir->next;
        }
        printf("\n");
        cmds = cmds->next;
    }
}

// Тесты для лексера
void test_lexer() {
    t_tools tools;

    // Инициализация структуры tools
    memset(&tools, 0, sizeof(t_tools));

    // Тест 1: Простая команда с пайпами и редиректами
    tools.args = strdup("echo \"Hello World\" | grep Hello > output.txt");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    print_lexer_list(tools.lexer_list);

    // Очистка списка лексеров
    t_lexer *tmp;
    while (tools.lexer_list != NULL) {
        tmp = tools.lexer_list;
        tools.lexer_list = tools.lexer_list->next;
        free(tmp->str);
        free(tmp);
    }
    free(tools.args);

    // Тест 2: Команда с here-doc и несколькими пайпами
    tools.args = strdup("cat << EOF | grep foo | sort > sorted.txt");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    print_lexer_list(tools.lexer_list);

    // Очистка списка лексеров
    while (tools.lexer_list != NULL) {
        tmp = tools.lexer_list;
        tools.lexer_list = tools.lexer_list->next;
        free(tmp->str);
        free(tmp);
    }
    free(tools.args);

    // Тест 3: Команда с переменными окружения
    tools.args = strdup("echo $HOME $PATH");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    print_lexer_list(tools.lexer_list);

    // Очистка списка лексеров
    while (tools.lexer_list != NULL) {
        tmp = tools.lexer_list;
        tools.lexer_list = tools.lexer_list->next;
        free(tmp->str);
        free(tmp);
    }
    free(tools.args);

    // Тест 4: Команда с одинарными и двойными кавычками
    tools.args = strdup("echo 'single quotes' \"double quotes\"");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    print_lexer_list(tools.lexer_list);

    // Очистка списка лексеров
    while (tools.lexer_list != NULL) {
        tmp = tools.lexer_list;
        tools.lexer_list = tools.lexer_list->next;
        free(tmp->str);
        free(tmp);
    }
    free(tools.args);

    // Тест 5: Команда с специальными символами
    tools.args = strdup("echo \"Special characters: !@#$%^&*()_+\"");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    print_lexer_list(tools.lexer_list);

    // Очистка списка лексеров
    while (tools.lexer_list != NULL) {
        tmp = tools.lexer_list;
        tools.lexer_list = tools.lexer_list->next;
        free(tmp->str);
        free(tmp);
    }
    free(tools.args);

    // Тест 6: Команда с несколькими редиректами
    tools.args = strdup("grep foo < input.txt > output.txt 2> error.log");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    print_lexer_list(tools.lexer_list);

    // Очистка списка лексеров
    while (tools.lexer_list != NULL) {
        tmp = tools.lexer_list;
        tools.lexer_list = tools.lexer_list->next;
        free(tmp->str);
        free(tmp);
    }
    free(tools.args);

    printf("test_lexer passed\n");
}

// Тесты для парсера
void test_parser() {
    t_tools tools;

    // Инициализация структуры tools
    memset(&tools, 0, sizeof(t_tools));

    // Тест 1: Простая команда с пайпами и редиректами
    tools.args = strdup("echo \"Hello World\" | grep Hello > output.txt");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    parser(&tools);
    print_simple_cmds(tools.simple_cmds);

    // Очистка списка команд и аргументов
    t_simple_cmds *cmd_tmp;
    while (tools.simple_cmds != NULL) {
        cmd_tmp = tools.simple_cmds;
        tools.simple_cmds = tools.simple_cmds->next;
        for (int i = 0; cmd_tmp->str[i] != NULL; i++) {
            free(cmd_tmp->str[i]);
        }
        free(cmd_tmp->str);
        free(cmd_tmp);
    }
    free(tools.args);

    // Тест 2: Команда с here-doc и несколькими пайпами
    tools.args = strdup("cat << EOF | grep foo | sort > sorted.txt");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    parser(&tools);
    print_simple_cmds(tools.simple_cmds);

    // Очистка списка команд и аргументов
    while (tools.simple_cmds != NULL) {
        cmd_tmp = tools.simple_cmds;
        tools.simple_cmds = tools.simple_cmds->next;
        for (int i = 0; cmd_tmp->str[i] != NULL; i++) {
            free(cmd_tmp->str[i]);
        }
        free(cmd_tmp->str);
        free(cmd_tmp);
    }
    free(tools.args);

    // Тест 3: Команда с переменными окружения
    tools.args = strdup("echo $HOME $PATH");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    parser(&tools);
    print_simple_cmds(tools.simple_cmds);

    // Очистка списка команд и аргументов
    while (tools.simple_cmds != NULL) {
        cmd_tmp = tools.simple_cmds;
        tools.simple_cmds = tools.simple_cmds->next;
        for (int i = 0; cmd_tmp->str[i] != NULL; i++) {
            free(cmd_tmp->str[i]);
        }
        free(cmd_tmp->str);
        free(cmd_tmp);
    }
    free(tools.args);

    // Тест 4: Команда с одинарными и двойными кавычками
    tools.args = strdup("echo 'single quotes' \"double quotes\"");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    parser(&tools);
    print_simple_cmds(tools.simple_cmds);

    // Очистка списка команд и аргументов
    while (tools.simple_cmds != NULL) {
        cmd_tmp = tools.simple_cmds;
        tools.simple_cmds = tools.simple_cmds->next;
        for (int i = 0; cmd_tmp->str[i] != NULL; i++) {
            free(cmd_tmp->str[i]);
        }
        free(cmd_tmp->str);
        free(cmd_tmp);
    }
    free(tools.args);

    // Тест 5: Команда с специальными символами
    tools.args = strdup("echo \"Special characters: !@#$%^&*()_+\"");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    parser(&tools);
    print_simple_cmds(tools.simple_cmds);

    // Очистка списка команд и аргументов
    while (tools.simple_cmds != NULL) {
        cmd_tmp = tools.simple_cmds;
        tools.simple_cmds = tools.simple_cmds->next;
        for (int i = 0; cmd_tmp->str[i] != NULL; i++) {
            free(cmd_tmp->str[i]);
        }
        free(cmd_tmp->str);
        free(cmd_tmp);
    }
    free(tools.args);

        // Тест 6: Команда с несколькими редиректами
    tools.args = strdup("grep foo < input.txt > output.txt 2> error.log");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    parser(&tools);
    print_simple_cmds(tools.simple_cmds);

    // Очистка списка команд и аргументов
    while (tools.simple_cmds != NULL) {
        cmd_tmp = tools.simple_cmds;
        tools.simple_cmds = tools.simple_cmds->next;
        for (int i = 0; cmd_tmp->str[i] != NULL; i++) {
            free(cmd_tmp->str[i]);
        }
        free(cmd_tmp->str);
        free(cmd_tmp);
    }
    free(tools.args);

    // Тест 7: Команда с пустыми аргументами
    tools.args = strdup("echo \"\"");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    parser(&tools);
    print_simple_cmds(tools.simple_cmds);

    // Очистка списка команд и аргументов
    while (tools.simple_cmds != NULL) {
        cmd_tmp = tools.simple_cmds;
        tools.simple_cmds = tools.simple_cmds->next;
        for (int i = 0; cmd_tmp->str[i] != NULL; i++) {
            free(cmd_tmp->str[i]);
        }
        free(cmd_tmp->str);
        free(cmd_tmp);
    }
    free(tools.args);

    // Тест 8: Команда с последовательными пайпами
    tools.args = strdup("ls | grep foo | sort | uniq");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    parser(&tools);
    print_simple_cmds(tools.simple_cmds);

    // Очистка списка команд и аргументов
    while (tools.simple_cmds != NULL) {
        cmd_tmp = tools.simple_cmds;
        tools.simple_cmds = tools.simple_cmds->next;
        for (int i = 0; cmd_tmp->str[i] != NULL; i++) {
            free(cmd_tmp->str[i]);
        }
        free(cmd_tmp->str);
        free(cmd_tmp);
    }
    free(tools.args);

    // Тест 9: Команда с сочетанием всех возможных токенов
    tools.args = strdup("echo 'Hello' > file.txt | cat < file.txt | grep Hello >> result.txt");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    parser(&tools);
    print_simple_cmds(tools.simple_cmds);

    // Очистка списка команд и аргументов
    while (tools.simple_cmds != NULL) {
        cmd_tmp = tools.simple_cmds;
        tools.simple_cmds = tools.simple_cmds->next;
        for (int i = 0; cmd_tmp->str[i] != NULL; i++) {
            free(cmd_tmp->str[i]);
        }
        free(cmd_tmp->str);
        free(cmd_tmp);
    }
    free(tools.args);

    // Тест 10: Команда с неправильными синтаксическими конструкциями
    tools.args = strdup("ls | | grep foo");
    tools.lexer_list = NULL;
    assert(token_reader(&tools) == 1);
    parser(&tools);
    print_simple_cmds(tools.simple_cmds);

    // Очистка списка команд и аргументов
    while (tools.simple_cmds != NULL) {
        cmd_tmp = tools.simple_cmds;
        tools.simple_cmds = tools.simple_cmds->next;
        for (int i = 0; cmd_tmp->str[i] != NULL; i++) {
            free(cmd_tmp->str[i]);
        }
        free(cmd_tmp->str);
        free(cmd_tmp);
    }
    free(tools.args);

    printf("test_parser passed\n");
}

int main() {
    test_lexer();
    test_parser();
    return 0;
}
