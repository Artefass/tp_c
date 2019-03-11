#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "token.h"

int main()
{
    char *str = NULL;
    size_t len = 0;
    size_t read_bytes;

    if ((read_bytes = getline(&str, &len, stdin)) <= 0)
    {
        printf("[error]");
        return 0;
    }

    len = read_bytes;
    if (str[len-1] == '\n') str[len-1] = '\0';
    

    if (str) free(str);
    return 0;
}