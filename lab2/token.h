#pragma once

#include <regex.h>
#include <stdbool.h>

typedef enum {
    UNKNOWN,
    STRING,
    NUMBER,
    OPERATION
} token_type;

typedef struct token_t {
    token_type type;
    char *name;
    char *value;
} token_t;

token_t** parse_to_tokens(const char *str);
void free_tokens(token_t *tokens[]);