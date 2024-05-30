/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <smoreron@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 22:50:00 by smoreron          #+#    #+#             */
/*   Updated: 2024/05/28 16:32:45 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
// Обработчик сигнала SIGINT (Ctrl-C)
void handle_sigint(int sig) {
    (void)sig; // Игнорирование параметра сигнала, чтобы избежать предупреждений компилятора

    // Если не в режиме heredoc, напечатать новую строку
    if (!g_global.in_heredoc)
        ft_putstr_fd("\n", STDERR_FILENO); // Напечатать новую строку в стандартный поток ошибок

    // Если команда выполняется, остановить heredoc и обновить отображение readline
    if (g_global.in_cmd) {
        g_global.stop_heredoc = 1; // Установить флаг остановки heredoc
        rl_replace_line("", 0); // Очистить текущую строку ввода readline
        rl_redisplay(); // Обновить отображение readline
        rl_done = 1; // Завершить readline
        return; // Завершить обработчик сигнала
    }

    rl_on_new_line(); // Перейти на новую строку
    rl_replace_line("", 0); // Очистить текущую строку ввода readline
    rl_redisplay(); // Обновить отображение readline
}

// Обработчик сигнала SIGQUIT (Ctrl-\)
void handle_sigquit(int sig) {
    ft_putstr_fd("Quit: ", STDERR_FILENO); // Напечатать "Quit: " в стандартный поток ошибок
    ft_putnbr_fd(sig, STDERR_FILENO); // Напечатать номер сигнала в стандартный поток ошибок
    ft_putchar_fd('\n', STDERR_FILENO); // Напечатать новую строку в стандартный поток ошибок
}

// Инициализация обработчиков сигналов
void init_signal_handlers(void) {
    struct sigaction sa_int; // Структура для установки обработчика SIGINT
    struct sigaction sa_quit; // Структура для установки обработчика SIGQUIT

    // Инициализация обработчика SIGINT
    sa_int.sa_handler = handle_sigint; // Установка функции-обработчика для SIGINT
    sa_int.sa_flags = SA_RESTART; // Установка флага для перезапуска системных вызовов, прерванных сигналом
    sigemptyset(&sa_int.sa_mask); // Очистка набора сигналов
    sigaction(SIGINT, &sa_int, NULL); // Установка обработчика для SIGINT

    // Инициализация обработчика SIGQUIT
    sa_quit.sa_handler = handle_sigquit; // Установка функции-обработчика для SIGQUIT
    sa_quit.sa_flags = SA_RESTART; // Установка флага для перезапуска системных вызовов, прерванных сигналом
    sigemptyset(&sa_quit.sa_mask); // Очистка набора сигналов
    sigaction(SIGQUIT, &sa_quit, NULL); // Установка обработчика для SIGQUIT
}



