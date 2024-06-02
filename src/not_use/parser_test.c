#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Определение токенов и структур

// Определение токенов (ранее t_tokens)
typedef enum s_tokens
{
	PIPE = 1,    // Токен для канала (|)
	GREAT,       // Токен для перенаправления вывода (>)
	GREAT_GREAT, // Токен для добавления вывода (>>)
	LESS,        // Токен для перенаправления ввода (<)
	LESS_LESS,   // Токен для here-document (<<)
	INVALID = -1
}							t_tokens;

// Структура для лексем (ранее t_lexer)
typedef struct s_lexer
{
	char *str;            // Строка, представляющая лексему
	t_tokens token;       // Тип токена
	int i;                // Индекс в исходной строке
	struct s_lexer *next; // Указатель на следующую лексему в списке
	struct s_lexer *prev; // Указатель на предыдущую лексему в списке
}							t_lexer;

// Объединенная структура инструментов (ранее t_tools и t_parser_tools)
typedef struct s_tools
{
	char *args;                        // Аргументы командной строки
	char **paths;                      // Массив путей к исполняемым файлам
	char **envp;                       // Массив переменных окружения
	struct s_simple_cmds *simple_cmds; // Указатель на список простых команд
	t_lexer *lexer_list;               // Указатель на список лексем
	t_lexer					*redirections;  // Указатель на список лексем для перенаправлений
	int num_redirections; // Количество перенаправлений
	char *pwd;            // Текущая рабочая директория
	char *old_pwd;        // Предыдущая рабочая директория
	int						pipes; // Количество конвейеров (pipe) в текущей команде
	int *pid;     // Массив идентификаторов процессов
	bool heredoc; // Флаг наличия here-document
	bool reset;   // Флаг необходимости сброса состояния
}							t_tools;

// Структура для простых команд (ранее t_simple_cmds)
typedef struct s_simple_cmds
{
	char					**str; // Массив строковых аргументов команды
	int						(*builtin)(t_tools *, struct s_simple_cmds *); // Указатель на функцию встроенной команды
	int						num_redirections; // Количество перенаправлений для команды
	char					*hd_file_name; // Имя файла для here-document
	t_lexer					*redirections; // Указатель на список перенаправлений
	struct s_simple_cmds	*next; // Указатель на следующую команду в списке
	struct s_simple_cmds	*prev; // Указатель на предыдущую команду в списке
}							t_simple_cmds;







// Прототипы функций
int count_special_symbols(t_lexer *lexer_list);
int count_arguments(t_lexer *lexer_list);
t_lexer* find_next_command(t_lexer *lexer_list);
char* join_strings(t_lexer *lexer_list, int count);
void add_redirection(t_tools *tools, t_lexer *lexer);
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
t_lexer* find_next_command(t_lexer *lexer_list) {
    while (lexer_list && lexer_list->token != PIPE) {
        lexer_list = lexer_list->next;
    }
    if (lexer_list) {
        return lexer_list->next;
    }
    return NULL;
}

// Объединение строк с пробелами
char* join_strings(t_lexer *lexer_list, int count) {
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
void add_redirection(t_tools *tools, t_lexer *lexer) {
    t_lexer *redirection_node = new_lexer_node(lexer->str, lexer->token, lexer->i);
    if (!redirection_node) {
        handle_error("Failed to allocate memory for redirection");
        return;
    }

    if (tools->redirections == NULL) {
        tools->redirections = redirection_node;
    } else {
        t_lexer *current = tools->redirections;
        while (current->next) {
            current = current->next;
        }
        current->next = redirection_node;
        redirection_node->prev = current;
    }

    tools->num_redirections++;
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

// Инициализация команды
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
        add_redirection(tools, *lexer_list);
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

// Освобождение списка лексем
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

// Главная функция
int main() {
    t_tools tools;
    t_lexer *lexer1, *lexer2, *lexer3, *lexer4, *lexer5;

    // Тест 1: Простая команда с пайпом
    lexer1 = new_lexer_node("echo", 0, 0);
    lexer2 = new_lexer_node("hello", 0, 1);
    lexer3 = new_lexer_node("|", PIPE, 2);
    lexer4 = new_lexer_node("grep", 0, 3);
    lexer5 = new_lexer_node("world", 0, 4);
    lexer1->next = lexer2;
    lexer2->prev = lexer1; lexer2->next = lexer3;
    lexer3->prev = lexer2; lexer3->next = lexer4;
    lexer4->prev = lexer3; lexer4->next = lexer5;
    lexer5->prev = lexer4;

    tools.lexer_list = lexer1;
    tools.simple_cmds = NULL;
    tools.redirections = NULL;
    tools.num_redirections = 0;
    tools.pipes = 0;
    tools.heredoc = false;
    tools.reset = false;

    printf("Тест 1: Простая команда с пайпом\n");
    if (parser(&tools) == 0) {
        printf("Парсер успешно завершил работу\n");
    } else {
        printf("Произошла ошибка при работе парсера\n");
    }

    t_simple_cmds *cmd = tools.simple_cmds;
    while (cmd) {
        for (int i = 0; cmd->str && cmd->str[i]; i++) {
            printf("Команда: %s\n", cmd->str[i]);
        }
        cmd = cmd->next;
    }
    free_cmd_list(tools.simple_cmds);
    free_lexer_list(lexer1);

    // Тест 2: Команда с редирекцией вывода
    lexer1 = new_lexer_node("echo", 0, 0);
    lexer2 = new_lexer_node("hello", 0, 1);
    lexer3 = new_lexer_node(">", GREAT, 2);
    lexer4 = new_lexer_node("output.txt", 0, 3);
    lexer1->next = lexer2;
    lexer2->prev = lexer1; lexer2->next = lexer3;
    lexer3->prev = lexer2; lexer3->next = lexer4;
    lexer4->prev = lexer3;

    tools.lexer_list = lexer1;
    tools.simple_cmds = NULL;
    tools.redirections = NULL;
    tools.num_redirections = 0;
    tools.pipes = 0;
    tools.heredoc = false;
    tools.reset = false;

    printf("Тест 2: Команда с редирекцией вывода\n");
    if (parser(&tools) == 0) {
        printf("Парсер успешно завершил работу\n");
    } else {
        printf("Произошла ошибка при работе парсера\n");
    }

    cmd = tools.simple_cmds;
    while (cmd) {
        for (int i = 0; cmd->str && cmd->str[i]; i++) {
            printf("Команда: %s\n", cmd->str[i]);
        }
        if (cmd->redirections) {
            t_lexer *redir = cmd->redirections;
            while (redir) {
                printf("Редирекция: %s\n", redir->str);
                redir = redir->next;
            }
        }
        cmd = cmd->next;
    }
    free_cmd_list(tools.simple_cmds);
    free_lexer_list(lexer1);

    // Тест 3: Команда с редирекцией ввода
    lexer1 = new_lexer_node("grep", 0, 0);
    lexer2 = new_lexer_node("hello", 0, 1);
    lexer3 = new_lexer_node("<", LESS, 2);
    lexer4 = new_lexer_node("input.txt", 0, 3);
    lexer1->next = lexer2;
    lexer2->prev = lexer1; lexer2->next = lexer3;
    lexer3->prev = lexer2; lexer3->next = lexer4;
    lexer4->prev = lexer3;

    tools.lexer_list = lexer1;
    tools.simple_cmds = NULL;
    tools.redirections = NULL;
    tools.num_redirections = 0;
    tools.pipes = 0;
    tools.heredoc = false;
    tools.reset = false;

    printf("Тест 3: Команда с редирекцией ввода\n");
    if (parser(&tools) == 0) {
        printf("Парсер успешно завершил работу\n");
    } else {
        printf("Произошла ошибка при работе парсера\n");
    }

    cmd = tools.simple_cmds;
    while (cmd) {
        for (int i = 0; cmd->str && cmd->str[i]; i++) {
            printf("Команда: %s\n", cmd->str[i]);
        }
        if (cmd->redirections) {
            t_lexer *redir = cmd->redirections;
            while (redir) {
                printf("Редирекция: %s\n", redir->str);
                redir = redir->next;
            }
        }
        cmd = cmd->next;
    }
    free_cmd_list(tools.simple_cmds);
    free_lexer_list(lexer1);

    // Тест 4: Команда с here-document
    lexer1 = new_lexer_node("cat", 0, 0);
    lexer2 = new_lexer_node("<<", LESS_LESS, 1);
    lexer3 = new_lexer_node("EOF", 0, 2);
    lexer1->next = lexer2;
    lexer2->prev = lexer1; lexer2->next = lexer3;
    lexer3->prev = lexer2;

    tools.lexer_list = lexer1;
    tools.simple_cmds = NULL;
    tools.redirections = NULL;
    tools.num_redirections = 0;
    tools.pipes = 0;
    tools.heredoc = false;
    tools.reset = false;

    printf("Тест 4: Команда с here-document\n");
    if (parser(&tools) == 0) {
        printf("Парсер успешно завершил работу\n");
    } else {
        printf("Произошла ошибка при работе парсера\n");
    }

    cmd = tools.simple_cmds;
    while (cmd) {
        for (int i = 0; cmd->str && cmd->str[i]; i++) {
            printf("Команда: %s\n", cmd->str[i]);
        }
        if (cmd->redirections) {
            t_lexer *redir = cmd->redirections;
            while (redir) {
                printf("Редирекция: %s\n", redir->str);
                redir = redir->next;
            }
        }
        cmd = cmd->next;
    }
    free_cmd_list(tools.simple_cmds);
    free_lexer_list(lexer1);

    // Тест 5: Команда с несколькими пайпами
    lexer1 = new_lexer_node("cat", 0, 0);
    lexer2 = new_lexer_node("file.txt", 0, 1);
    lexer3 = new_lexer_node("|", PIPE, 2);
    lexer4 = new_lexer_node("grep", 0, 3);
    lexer5 = new_lexer_node("pattern", 0, 4);
    t_lexer *lexer6 = new_lexer_node("|", PIPE, 5);
    t_lexer *lexer7 = new_lexer_node("wc", 0, 6);
    t_lexer *lexer8 = new_lexer_node("-l", 0, 7);

    lexer1->next = lexer2;
    lexer2->prev = lexer1; lexer2->next = lexer3;
    lexer3->prev = lexer2; lexer3->next = lexer4;
    lexer4->prev = lexer3; lexer4->next = lexer5;
    lexer5->prev = lexer4; lexer5->next = lexer6;
    lexer6->prev = lexer5; lexer6->next = lexer7;
    lexer7->prev = lexer6; lexer7->next = lexer8;
    lexer8->prev = lexer7;

    tools.lexer_list = lexer1;
    tools.simple_cmds = NULL;
    tools.redirections = NULL;
    tools.num_redirections = 0;
    tools.pipes = 0;
    tools.heredoc = false;
    tools.reset = false;

    printf("Тест 5: Команда с несколькими пайпами\n");
    if (parser(&tools) == 0) {
        printf("Парсер успешно завершил работу\n");
    } else {
        printf("Произошла ошибка при работе");
        }
}

