#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


int main(void)
{
    char    *error_message;

    error_message = readline("test here");
    add_history (error_message);
    printf("return is :::: %s\n",error_message);
    error_message = readline("test here");
    add_history (error_message);
    printf("return is :::: %s\n",error_message);
    error_message = readline("test here");
    add_history (error_message);
    printf("return is :::: %s\n",error_message);
    return (0);
}

