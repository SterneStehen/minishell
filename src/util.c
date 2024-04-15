#include "../inc/minishell.h"

void ft_free_array(char **arr)
{
    if (arr != NULL)
	{
        int i = 0;
        while (arr[i] != NULL)
		{
            free(arr[i]);
            i++;
        }
        free(arr);
    }
}



void free_command(t_command *command) 
{
    if (command != NULL) {
        if (command->cmd != NULL) 
		{
            free(command->cmd);
        }
        if (command->args != NULL)
		{
            ft_free_array(command->args);
        }
    }
}






// void free_command(t_command *command) 
// {
//     if (command == NULL) {
//         return;
//     }

//     //Освобождение массива переменных окружения
//     if (command->envp != NULL) 
// 	{
//         int i = 0;
//         while (command->envp[i] != NULL) 
// 		{
//             free(command->envp[i]);
//             i++;
//         }
//         free(command->envp);
// 		command->envp = NULL;
//     }

//     //Освобождение поля команды, если оно было выделено
//     if (command->cmd != NULL) 
// 	{
//         free(command->cmd);
// 		command->cmd = NULL;
//     }

//     //Освобождение массива аргументов
//     if (command->args != NULL) {
//         int i = 0;
//         while (command->args[i] != NULL) 
// 		{
//             free(command->args[i]);
//             i++;
//         }
//         free(command->args);
// 		command->args = NULL;
//     }

//     // Освобождение самой структуры команды
//     // free(command);
// }

// void free_execute_command(t_command *command) 
// {
//     if (command == NULL) {
//         return;
//     }

//     // Освобождение строки команды, если она была выделена
//     if (command->cmd != NULL) {
//         free(command->cmd);
//         command->cmd = NULL;
//     }

//     // Освобождение массива аргументов
//     if (command->args != NULL) {
//         for (int i = 0; command->args[i] != NULL; i++) {
//             free(command->args[i]);
//         }
//         free(command->args);
//         command->args = NULL;
//     }
// }


// void ft_free_array(char **array) 
// {
//     if (array != NULL) 
// 	{
//         int i = 0;
//         while (array[i]) 
// 		{
//             free(array[i]);
//             i++;
//         }
//         free(array);
//     }
// }

// void show_history() 
// {
//     // Получаем размер истории
//     int history_length = history_length;
//     HIST_ENTRY *entry;

//     // Перебираем и выводим каждую запись истории
//     for (int i = 1; i <= history_length; i++) 
// 	{
//         entry = history_get(i);
//         if (entry) 
// 		{ // Проверяем, не является ли указатель нулевым
//             printf("%d: %s\n", i, entry->line);
//         }
//     }
// }
