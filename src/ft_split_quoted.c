#include "../inc/minishell.h"

int ft_word_count_plus(const char *str, char c) {
    int count = 0, in_quotes = 0;
    char quote_char = 0;

    while (*str) {
        if (*str == c && !in_quotes) {
            count++;
            while (*str == c) str++;
        } else {
            if ((*str == '\'' || *str == '\"') && !in_quotes) {
                in_quotes = 1;
                quote_char = *str;
            } else if (*str == quote_char && in_quotes) {
                in_quotes = 0;
            }
            str++;
        }
    }
    return count;
}


char **ft_split_quoted(const char *s, char c) {
    char **result;
    int i = 0, j = 0;
    bool in_quotes = false;
    char quote_char = 0;

    result = malloc(sizeof(char *) * (ft_word_count_plus(s, c) + 1));
    if (!result) return NULL;

    int start = 0;  // Инициализируем переменную вне цикла для лучшей читаемости
    for (; s[i]; i++) {
        if (!in_quotes && (s[i] == c || s[i] == '\0')) {
            if (i > start) {
                result[j++] = ft_substr(s, start, i - start);
            }
            start = i + 1;
        } else if (s[i] == '"' || s[i] == '\'') {
            if (!in_quotes) {
                in_quotes = true;
                quote_char = s[i];
                start = i + 1;  // Пропускаем открывающую кавычку
            } else if (quote_char == s[i]) {
                in_quotes = false;
                result[j++] = ft_substr(s, start, i - start);
                start = i + 1;  // Пропускаем закрывающую кавычку
            }
        }
    }
    if (in_quotes) {
        while (--j >= 0) free(result[j]); // Очищаем уже выделенную память для элементов массива
        free(result);
        return NULL;  // Несбалансированные кавычки
    }
    result[j] = NULL;
    return result;
}