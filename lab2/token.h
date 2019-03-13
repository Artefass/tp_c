#pragma once

#include <stdbool.h>
#include <regex.h>

#include "vector.h"

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
    int position;
} token_t;

typedef struct regex_token_t {
    token_type type;
    char *name;
    char *pattern;
    regex_t re;
} regex_token_t;

token_t* init_token(token_t *token, token_type type, char *data, int len, int pos);
token_t* new_token();
void free_token(token_t *token);

int match_string(const char **str, token_t *token);
int match_int(const char **str, token_t *token);
int match_operation(const char **str, token_t *token);
int match_token(const char **str, token_t *token);

token_t* get_token(const char *str, int *error);

vector_t* new_tokens_vector();
void free_tokens_vector(vector_t* tokens);

vector_t* parse_to_tokens(const char *str);

void print_tokens(vector_t *tokens);

/*
vector_t* parse_to_tokens(const char *str);

int regex_compile(regex_token_t *regex_tokens);
void regex_free(regex_token_t *regex_tokens);

bool init_token(const regex_token_t *regex_token, token_t *token, const char *str, int copy_len);
bool match_token(const char *str, const regex_token_t *regex_tokens, token_t *token) ;
token_t* get_token(const char *str, const regex_token_t *regex_tokens);

vector_t* new_tokens_vector();
void free_tokens_vector(vector_t* tokens);

void print_tokens(vector_t *tokens);

*/

