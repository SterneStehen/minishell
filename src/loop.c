/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <smoreron@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:43:01 by smoreron          #+#    #+#             */
/*   Updated: 2024/05/31 11:48:53 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" // Подключение заголовочного файла minishell.h

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <smoreron@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:43:01 by smoreron          #+#    #+#             */
/*   Updated: 2024/05/31 11:48:53 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Функция для обрезки пробелов с начала и конца строки
char	*ft_cut(const char *s1, const char *set)
{
	size_t start; // Индекс начала обрезанной строки
	size_t end;   // Индекс конца обрезанной строки
	size_t len;   // Длина обрезанной строки
	char *str;    // Указатель на обрезанную строку
	if (!s1 || !set)
		return (NULL); // Проверка на NULL входных значений
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++; // Пропуск символов из набора set в начале строки
	end = ft_strlen(s1);
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--; // Пропуск символов из набора set в конце строки
	len = end - start; // Определение длины обрезанной строки
	str = ft_substr(s1, start, len); // Создание новой строки из подстроки s1
	return (str); // Возвращение обрезанной строки
}

// Улучшенная функция для сброса инструментов
int	renew(t_tools *tools)
{
	t_simple_cmds *cmd;
	t_lexer *lexer;

	if (tools->args)
	{
		free(tools->args);  // Освобождение памяти, занятой строкой args
		tools->args = NULL; // Установка указателя args в NULL
	}

	while (tools->simple_cmds)
	{
		cmd = tools->simple_cmds;
		tools->simple_cmds = tools->simple_cmds->next;
		for (int i = 0; cmd->str[i] != NULL; i++)
			free(cmd->str[i]);
		free(cmd->str);
		free(cmd);
	}

	while (tools->lexer_list)
	{
		lexer = tools->lexer_list;
		tools->lexer_list = tools->lexer_list->next;
		free(lexer->str);
		free(lexer);
	}

	while (tools->redirections)
	{
		lexer = tools->redirections;
		tools->redirections = tools->redirections->next;
		free(lexer->str);
		free(lexer);
	}

	tools->num_redirections = 0;
	tools->heredoc = false;
	tools->pipes = 0;
	tools->pid = NULL;
	return (1);
}

// Функция для обработки ошибок
void	ft_error(char *error_message, t_tools *tools)
{
	ft_putendl_fd(error_message, STDERR_FILENO); // Вывод сообщения об ошибке
	renew(tools);                                // Сброс инструментов
}

// Функция для подсчета парных кавычек
int	dubl_sign(const char *str)
{
	int sign = 0; // Счетчик одинарных кавычек
	int sign2 = 0; // Счетчик двойных кавычек
	while (*str)
	{
		if (*str == '\'')
			sign++; // Увеличение счетчика одинарных кавычек
		else if (*str == '"')
			sign2++; // Увеличение счетчика двойных кавычек
		str++;
	}
	return (sign % 2 == 0) && (sign2 % 2 == 0); // Проверка на парность кавычек
}

// Основной цикл выполнения команд в minishell
int	loop(t_tools *tools)
{
	char *str; // Указатель на строку, которая будет обработана
	while (1) // Бесконечный цикл для выполнения команд
	{
		tools->args = readline("minishell> "); // Чтение строки ввода от пользователя
		if (!tools->args)                      // Если ввод пустой (Ctrl-D)
		{
			ft_putendl_fd("exit", STDOUT_FILENO); // Вывод сообщения "exit"
			exit(EXIT_SUCCESS); // Завершение программы
		}
		str = ft_cut(tools->args, " "); // Удаление пробелов с начала и конца строки
		free(tools->args);              // Освобождение памяти для старой строки
		tools->args = str;              // Присвоение новой строки аргументам
		if (tools->args[0] == '\0') // Проверка, является ли строка пустой
		{
			renew(tools); // Сброс инструментов
			continue; // Переход к следующей итерации цикла
		}
		add_history(tools->args); // Добавление строки в историю команд
		if (!dubl_sign(tools->args)) // Проверка на парные кавычки
		{
			ft_error("Mismatched quotes error", tools); // Обработка ошибки, если кавычки не парные
			continue; // Переход к следующей итерации цикла
		}
		if (!token_reader(tools)) // Лексический анализ строки
		{
			ft_error("token error", tools); // Обработка ошибки, если анализ не удался
			continue; // Переход к следующей итерации цикла
		}
		parser(tools); // Синтаксический анализ строки
		prepare_executor(tools); // Подготовка и выполнение команд
		renew(tools); // Сброс инструментов
	}
	return (1);
}
