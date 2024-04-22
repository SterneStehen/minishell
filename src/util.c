#include "../inc/minishell.h"

void ft_free_array(char **arr) 
{
    if (arr != NULL) 
	{
        int i = 0;
        while (arr[i] != NULL) 
		{
            free(arr[i]);  // освобождаем каждую строку в массиве
            i++;
        }
        free(arr);  // освобождаем сам массив
    }
	//arr = NULL;
}
void free_command(t_command *command) {
    if (command != NULL) {
        // Освобождение массива команд
        if (command->cmd != NULL) 
		{
            for (int i = 0; command->cmd[i] != NULL; i++) 
			{
                free(command->cmd[i]);  // Освобождаем каждую команду в массиве
            }
            free(command->cmd);  // Освобождаем массив команд
            command->cmd = NULL;
        }

        // Освобождение массива переменных окружения
        if (command->envp != NULL) 
		{
            for (int i = 0; command->envp[i] != NULL; i++) 
			{
                free(command->envp[i]);  // Освобождаем каждую переменную окружения
            }
            free(command->envp);  // Освобождаем массив переменных окружения
            command->envp = NULL;
        }

        // Освобождение пути, если он есть
        if (command->path != NULL) 
		{
            free(command->path);
            command->path = NULL;
        }

        // Наконец, освобождение самой структуры команды
        free(command);
        command = NULL;
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
