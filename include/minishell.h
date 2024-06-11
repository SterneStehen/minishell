/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <smoreron@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 13:46:41 by mgraaf            #+#    #+#             */
/*   Updated: 2024/06/11 12:44:34 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../include/libft/libft.h"

# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <unistd.h>
# include <errno.h>
# include <sys/stat.h>
# include <signal.h>
#include <sys/wait.h>


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
	int	pipes; // Количество конвейеров (pipe) в текущей команде
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

// Глобальная структура состояния (ранее t_global)
typedef struct s_global
{
	int error_num;    // Код последней ошибки
	int stop_heredoc; // Флаг остановки обработки here-document
	int in_cmd;       // Флаг выполнения команды
	int in_heredoc;   // Флаг обработки here-document
}							t_global;

extern t_global g_global; // Объявление глобальной переменной состояния


void						init_tools(t_tools *tools, char **envp);
void						free_PWD(t_tools *tools);
char						**dupl_arr(char **arr);
void						free_array(char **array);
int							find_PWD(t_tools *tools);
int							find_paths(t_tools *tools); 
void init_signal_handlers(void);
int loop(t_tools *tools);

// Прототипы функций
int							parse_envp(t_tools *tools);
int							renew(t_tools *tools);
void						init_stri(int i, int j, t_tools *tools);
char						**expander(t_tools *tools, char **str);
char						*expander_str(t_tools *tools, char *str);
size_t						dollar_sign(char *str);
char						*char_to_str(char c);
int							after_dol_lenght(char *str, int j);
void						free_things(char *tmp2, t_tools *tools, int i);
void						print_parser(t_simple_cmds simple_cmds);
char						*delete_quotes_value(char *str);
void						sigint_handler(int sig);
void						sigquit_handler(int sig);
void						init_signals(void);
char						*delete_quotes(char *str, char c);
char						*delete_quotes_export(char *str, char c);
int							question_mark(char **tmp);

// Встроенные функции
int 						(*builtin_arr(char *str))(t_tools *tools, t_simple_cmds *simple_cmd);
char						**dupl_arr(char **arr);
void						free_array(char **array);
int							find_PWD(t_tools *tools);
int							find_paths(t_tools *tools);

// init shell
void						init_tools(t_tools *tools, char **envp);
int							find_paths(t_tools *tools);
void						free_PWD(t_tools *tools);
int							find_PWD(t_tools *tools);
void						free_array(char **array);
char						**dupl_arr(char **arr);

// lexer

// Прототипы вспомогательных функций
int	renew(t_tools *tools);                               
		// Прототип функции для сброса инструментов
void	ft_error(char *error_message, t_tools *tools);
		// Прототип функции для обработки ошибок
int	dubl_sign(const char *str);                       
		// Прототип функции для подсчета парных кавычек
int	token_reader(t_tools *tools);                        
		// Прототип функции для лексического анализа строки
int 	parser(t_tools *tools);                             
		// Прототип функции для синтаксического анализа строки
void	prepare_executor(t_tools *tools);                   
		// Прототип функции для подготовки и выполнения команд
void create_heredoc(t_simple_cmds *cmd);
void expand_environment_variables(t_simple_cmds *cmd, char **envp);

#endif
