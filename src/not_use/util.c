#include "../include/minishell.h"

// void free_array(char **array) 
// {
//     if (array) {
//         for (size_t i = 0; array[i] != NULL; i++) {
//             free(array[i]);
//         }
//         free(array);
//     }
// }

// void free_PWD(t_tools *tools) {
//     if (tools->pwd) {
//         free(tools->pwd);
//         tools->pwd = NULL;
//     }
//     if (tools->old_pwd) {
//         free(tools->old_pwd);
//         tools->old_pwd = NULL;
//     }
// }