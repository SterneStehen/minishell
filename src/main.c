/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <smoreron@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:42:32 by smoreron          #+#    #+#             */
/*   Updated: 2024/05/28 16:42:32 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_global g_global;

int main(int argc, char **argv, char **envp) {
    t_tools tools;

    // Инициализация глобальной переменной
    g_global.error_num = 0;
    g_global.stop_heredoc = 0;
    g_global.in_cmd = 0;
    g_global.in_heredoc = 0;

    if (argc < 1)
        return 0;
    (void) *argv;
    // Инициализация структуры tools
    init_tools(&tools, envp);
    init_signal_handlers();

    // Основной цикл выполнения команд в minishell
    loop(&tools);

    // Освобождение ресурсов перед выходом
    free(tools.pwd);
    free(tools.old_pwd);
    free_array(tools.envp);
    free_array(tools.paths);

    return 0;
}
