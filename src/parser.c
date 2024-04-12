#include "minishell.h"



void parse_input(char *input, t_command *command) {
    char **commands;
    char **args;
    const char command_delimiter = ';';
    const char arg_delimiter = ' ';

    //free_command(command); // Очищаем команду перед использованием

    commands = ft_split(input, command_delimiter);
    if (commands[0] != NULL) 
	{
        args = ft_split(commands[0], arg_delimiter);
        command->cmd = ft_strdup(args[0]); // Сохраняем имя команды
        command->args = args; // args уже является NULL-terminated массивом, который можно использовать напрямую

        // Важно: команда должна быть освобождена после использования, включая command->cmd и каждый элемент command->args
    } else 
	{
        command->cmd = NULL;
        command->args = NULL;
    }
    
    ft_free_array(commands); // Освобождаем массив команд, так как он больше не нужен
}
