/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <smoreron@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:23:50 by smoreron          #+#    #+#             */
/*   Updated: 2024/05/31 11:39:10 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft/libft.h"
#include "../include/minishell.h"

// Проверка, является ли символ пробельным
bool	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r'); // Возвращает true, если символ пробельный
}

// Пропуск пробельных символов в строке
void	next_space(const char **str)
{
	while (is_space(**str))
	{             // Пока текущий символ пробельный
		(*str)++; // Переход к следующему символу
	}
}

// Добавление нового узла в список лексеров
int	add_lexer(t_lexer **lexer_list, const char *str, t_tokens token)
{
	t_lexer *new_node = malloc(sizeof(t_lexer));
		// Выделение памяти для нового узла
	if (!new_node)
		return (0); // Проверка на успешное выделение памяти,возвращение 0 в случае неудачи
	new_node->str = ft_strdup(str); // Дублирование строки
	if (!new_node->str)
	{                   // Проверка на успешное дублирование строки
		free(new_node); // Освобождение выделенной памяти
		return (0);       // Возвращение 0 в случае неудачи
	}
	new_node->token = token; // Установка типа токена
	new_node->i = 0;         // Инициализация индекса
	new_node->next = NULL;   // Инициализация указателя на следующий элемент
	new_node->prev = NULL;   // Инициализация указателя на предыдущий элемент
	if (*lexer_list == NULL)
	{                           // Если список пуст
		*lexer_list = new_node; // Установка первого элемента списка
	}
	else
	{
		t_lexer *temp = *lexer_list; // Временный указатель на начало списка
		while (temp->next != NULL)
		{                      // Проход по списку до последнего элемента
			temp = temp->next; // Переход к следующему элементу
		}
		temp->next = new_node; // Добавление нового узла в конец списка
		new_node->prev = temp; // Установка указателя на предыдущий элемент
	}
	return (1); // Возвращение 1 в случае успеха
}

// Обработка содержимого внутри кавычек
void	handle_quotes(const char **str, char **result, char quote_type)
{
	(*str)++; // Пропуск начальной кавычки
	while (**str && **str != quote_type)
	{                    
		// Пока не достигнут конец строки или закрывающая кавычка
		**result = **str; // Копирование символа в результат
		(*result)++;      // Переход к следующей позиции в результате
		(*str)++;         // Переход к следующему символу в исходной строке
	}
	if (**str == quote_type)
	{             // Если достигнута закрывающая кавычка
		(*str)++; // Пропуск закрывающей кавычки
	}
}

// Обработка токенов
t_tokens	get_token(const char **str)
{
	if (**str == '|')
	{
		(*str)++;    // Переход к следующему символу
		return (PIPE); // Возвращение токена PIPE
	}
	else if (**str == '>')
	{
		(*str)++; // Переход к следующему символу
		if (**str == '>')
		{                       // Если следующий символ тоже '>'
			(*str)++;           // Переход к символу после '>>'
			return (GREAT_GREAT); // Возвращение токена GREAT_GREAT
		}
		return (GREAT); // Возвращение токена GREAT
	}
	else if (**str == '<')
	{             // Если текущий символ - '<'
		(*str)++; // Переход к следующему символу
		if (**str == '<')
		{                     // Если следующий символ тоже '<'
			(*str)++;         // Переход к символу после '<<'
			return (LESS_LESS); // Возвращение токена LESS_LESS
		}
		return (LESS); // Возвращение токена LESS
	}
	return (INVALID); // Неверный токен
}

// Основная функция для чтения и анализа токенов в строке
int	token_reader(t_tools *tools)
{
	const char *str = tools->args; // Указатель на текущую позицию в строке
	char buffer[1024];             // Буфер для хранения текущего слова/токена
	int buf_index = 0;             // Индекс текущей позиции в буфере
	t_tokens token;                // Переменная для хранения текущего токена
	while (*str)
	{                     // Пока не достигнут конец строки
		next_space(&str); // Пропуск пробельных символов
		if (*str == '\0')
			break ; // Проверка на конец строки
		// Обработка токенов
		token = get_token(&str); // Получение токена из строки
		if (token != INVALID)
		{                             // Если найден токен
			buffer[buf_index] = '\0'; // Завершение текущего слова в буфере
			if (buf_index > 0)
			{ // Если в буфере есть слово
				if (!add_lexer(&tools->lexer_list, buffer, -1))
				{             // Добавление слова в список лексеров
					return (0); // Возвращение 0 в случае ошибки
				}
				buf_index = 0; // Сброс индекса буфера
			}
			if (!add_lexer(&tools->lexer_list, str - 1, token))
			{             // Добавление токена в список лексеров
				return (0); // Возвращение 0 в случае ошибки
			}
			continue ; // Переход к следующей итерации цикла
		}
		// Обработка кавычек
		if (*str == '\'' || *str == '"') // Если текущий символ - кавычка
		{                           
			char quote_type = *str; // Сохранение типа кавычки
			str++;                  // Пропуск начальной кавычки
			while (*str && *str != quote_type) // Пока не достигнут конец строки или закрывающая кавычка
			{                                
				
				buffer[buf_index++] = *str++; // Копирование символа в буфер
			}
			if (*str == quote_type) // Если достигнута закрывающая кавычка
			{          
				str++; // Пропуск закрывающей кавычки
			}
			continue ; // Переход к следующей итерации цикла
		}
		// Обработка слова
		buffer[buf_index++] = *str++;
			// Копирование символа в буфер и переход к следующему символу
	}
	// Добавление оставшегося слова в список лексеров
	if (buf_index > 0)
	{
		buffer[buf_index] = '\0'; // Завершение слова в буфере
		if (!add_lexer(&tools->lexer_list, buffer, -1))
		{             // Добавление слова в список лексеров
			return (0); // Возвращение 0 в случае ошибки
		}
	}
	return (1); // Возвращение успешного завершения
}
