#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum s_tokens {
    PIPE = 1,
    GREAT,
    GREAT_GREAT,
    LESS,
    LESS_LESS,
    INVALID = -1
} t_tokens;

typedef struct s_lexer {
    char *str;
    t_tokens token;
    int i;
    struct s_lexer *next;
    struct s_lexer *prev;
} t_lexer;

typedef struct s_tools {
    char *args;
    char **paths;
    char **envp;
    t_simple_cmds *simple_cmds;
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

typedef struct s_simple_cmds {
    char **str;
    int (*builtin)(struct s_tools *, struct s_simple_cmds *);
    int num_redirections;
    char *hd_file_name;
    t_lexer *redirections;
    struct s_simple_cmds *next;
    struct s_simple_cmds *prev;
} t_simple_cmds;



void handle_error(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

t_simple_cmds* new_cmd_node() {
    t_simple_cmds *cmd = (t_simple_cmds *)malloc(sizeof(t_simple_cmds));
    if (cmd) {
        cmd->str = NULL;
        cmd->builtin = NULL;
        cmd->num_redirections = 0;
        cmd->hd_file_name = NULL;
        cmd->redirections = NULL;
        cmd->next = NULL;
        cmd->prev = NULL;
    }
    return cmd;
}

int count_arguments(t_lexer *lexer_list) {
    int count = 0;
    while (lexer_list && lexer_list->token != PIPE && lexer_list->token < GREAT) {
        count++;
        lexer_list = lexer_list->next;
    }
    return count;
}

void add_redirection(t_tools *tools, t_lexer *lexer, t_simple_cmds *cmd) {
    t_lexer *redirection_node = (t_lexer *)malloc(sizeof(t_lexer));
    if (!redirection_node) {
        handle_error("Failed to allocate memory for redirection");
        return;
    }
    redirection_node->str = strdup(lexer->str);
    redirection_node->token = lexer->token;
    redirection_node->i = lexer->i;
    redirection_node->next = NULL;
    redirection_node->prev = NULL;

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

void initialize_command(t_tools *tools, t_lexer **lexer_list) {
    printf("Initializing new command...\n");
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
        if (*lexer_list) {
            cmd->str[i] = strdup((*lexer_list)->str);
            *lexer_list = (*lexer_list)->next;
        } else {
            handle_error("Lexer list ended prematurely");
            free(cmd->str);
            free(cmd);
            return;
        }
    }
    cmd->str[arg_count] = NULL;

    if (tools->simple_cmds == NULL) {
        tools->simple_cmds = cmd;
    } else {
        t_simple_cmds *current = tools->simple_cmds;
        printf("Initial current: %p\n", (void*)current); // Debug print
        while (current && current->next) {
            printf("Current in loop: %p\n", (void*)current); // Debug print
            current = current->next;
        }
        if (current) {
            current->next = cmd;
            cmd->prev = current;
        } else {
            printf("Error: current became NULL\n");
        }
    }

    // Обработка редирекций для команды
    while (*lexer_list && (*lexer_list)->token >= GREAT) {
        add_redirection(tools, *lexer_list, cmd);
        if (*lexer_list) {
            *lexer_list = (*lexer_list)->next;
        }
    }

    printf("Command initialized: %p\n", (void*)cmd); // Debug print
}

int parser(t_tools *tools) {
    t_lexer *current = tools->lexer_list;
    tools->pipes = 0;

    while (current) {
        if (current->token == PIPE) {
            current = current->next;
            continue;
        }

        initialize_command(tools, &current);
        while (current && current->token != PIPE) {
            current = current->next;
        }
        if (current) {
            current = current->next;
        }
    }

    return 0; // Возвращаем 0 при успешном завершении
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

void print_parser(t_simple_cmds *simple_cmds) {
    t_simple_cmds *cmd = simple_cmds;
    int cmd_num = 0;

    while (cmd != NULL) {
        printf("Command %d:\n", cmd_num);
        for (int i = 0; cmd->str[i] != NULL; i++) {
            printf("  Argument %d: %s\n", i, cmd->str[i]);
        }
        printf("  Number of redirections: %d\n", cmd->num_redirections);
        if (cmd->redirections) {
            t_lexer *redir = cmd->redirections;
            int redir_num = 0;
            while (redir) {
                printf("    Redirection %d: %s (token %d)\n", redir_num, redir->str, redir->token);
                redir = redir->next;
                redir_num++;
            }
        }
        if (cmd->hd_file_name) {
            printf("  Here-document file name: %s\n", cmd->hd_file_name);
        }
        cmd_num++;
        cmd = cmd->next;
    }
}

void print_lexer_list(t_lexer *lexer_list) {
    while (lexer_list) {
        printf("Token: %d, String: %s\n", lexer_list->token, lexer_list->str);
        lexer_list = lexer_list->next;
    }
}

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

void add_lexer_node(t_lexer **head, char *str, t_tokens token, int index) {
    t_lexer *new_node = new_lexer_node(str, token, index);
    if (*head == NULL) {
        *head = new_node;
    } else {
        t_lexer *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
        new_node->prev = current;
    }
}

int main() {
    t_tools tools;
    tools.simple_cmds = NULL;
    t_lexer *lexer_list1 = NULL;

    add_lexer_node(&lexer_list1, "echo", INVALID, 0);
    add_lexer_node(&lexer_list1, "\"Hello, World!\"", INVALID, 5);
    add_lexer_node(&lexer_list1, "\"hello\"", INVALID, 20);
    add_lexer_node(&lexer_list1, "|", PIPE, 28);
    add_lexer_node(&lexer_list1, "grep", INVALID, 30);
    add_lexer_node(&lexer_list1, "Hello", INVALID, 35);
    add_lexer_node(&lexer_list1, ">>", GREAT_GREAT, 41);
    add_lexer_node(&lexer_list1, "output.txt", INVALID, 44);

    printf("Lexer list before parsing:\n");
    print_lexer_list(lexer_list1);
    printf("----------------------------\n");

    tools.lexer_list = lexer_list1;
    printf("Test 1:\n");
    parser(&tools);
    print_parser(tools.simple_cmds);

    free_lexer_list(tools.lexer_list);
    free_cmd_list(tools.simple_cmds);

    return 0;
}
