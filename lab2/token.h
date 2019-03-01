#pragma once

#include <regex.h>

typedef enum {
    UNNOWN,
    STRING,
    INT,
    OPERATON
} token_type;

typedef struct token_t {
    token_type type,
    char *name;
    char *value;
} token_t;

typedef struct regex_token_t {
    token_type type,
    char *name,
    char *pattern;
    regex_t re,
} regex_token_t;









token_t get_token(const char *expression){
    // пропуск пробелов
    while(*expression == ' ' and *expression != '\0')
        expression++;

    if *exp
    
    token_t *match_token = NULL;
    if (match_operand_string(expression, match_token) {}
        return match_token;
    }

    if (match_operation(expression, match_token)) {
        token_t token = {.type.OPERATON, .token=match_token};
        return token;
    } 
}

