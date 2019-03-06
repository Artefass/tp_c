#include "token.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

#include"utils.h"

#define PTRN_STRING "^\"[0-9a-zA-Z]+\""
#define PTRN_NUMBER "^[0-9]+"
#define PTRN_OPERATION "^(*|+|-|(|))"

typedef struct regex_token_t {
    token_type type;
    char *name;
    char *pattern;
    regex_t re;
} regex_token_t;

#define SET_PATTERN_TOKEN(type, name, pattern) (regex_token_t){(type), (name), (pattern)}

static regex_token_t regex_tokens[] = {
    SET_PATTERN_TOKEN(STRING,"string", PTRN_STRING),
    SET_PATTERN_TOKEN(NUMBER,"number", PTRN_NUMBER),
    SET_PATTERN_TOKEN(OPERATION,"operation",PTRN_OPERATION)    
};

static bool init = false;

static int regex_init()
{
    assert(!init);

    int error;
    for (int i = 0; i < ARRAYNUM(regex_tokens); i++)
    {
        if ((error = regcomp(&regex_tokens[i].re, regex_tokens[i].pattern, REG_EXTENDED|REG_NOSUB)) > 0)
        {
            printf("ERROR: regcopm faild. error: %d pattern:%s", error, regex_tokens[i].pattern);
            return error;
        }
    }
    init = true;
    return 0;
}

static void regex_free()
{
    assert(init);

    for (int i = 0; i < ARRAYNUM(regex_tokens); i++)
    {
         regfree(&regex_tokens[i].re);
    }
}

static token_t* new_token(){
    token_t *token = calloc(sizeof(token_t), 1);
    
    if (!token) 
    {
        printf("Error: new_token faild. could not allocate memmory\n");
        return NULL;
    }

    token->type = UNKNOWN;
    return token;
}

static void free_token(token_t *token)
{
    assert(token);

    if (token->name) free(token->name);
    if (token->value) free(token->value);

    free(token);
}

static bool init_token(const regex_token_t *regex_token, token_t *token, const char *str, int length)
{
    token->name = strdup(regex_token->name);
    token->value = strndup(str, length);
    token->type = regex_token->type;
    if (!token->name || !token->type)
    {
        if (token->name) free(token->name);
        if (token->value) free(token->value);

        printf("Error: init_token faild. could not allocate memmory\n");   
        return false;         
    }

    return true;
}

static bool match_token(const char *str, token_t *token) 
{
    assert(str != NULL && token != NULL);

    regmatch_t match_data;
    for (int i = 0; i < ARRAYNUM(regex_tokens); i++)
    {
        if (!regexec(&regex_tokens[i].re, str, 1, &match_data, 0))
        {
            int len = match_data.rm_eo - match_data.rm_so;
            if (init_token(&regex_tokens[i], token, str, len)){
                str += len;
                return true;
            }
        } 
    }

    return false;
}

static token_t *get_token(const char *str)
{
    // пропуск пробелов
    while(*str == ' ' && *str != '\0') str++;

    token_t *token;
    if (!(token = new_token())) 
    {
        return NULL;
    }

    if (!match_token(str, token)){
        free_token(token);
        return NULL;
    }
    
    return token;
}

#define BASE_SIZE 16

static token_t** new_tokens_massive()
{
    token_t **tokens = calloc(sizeof(token_t*), BASE_SIZE);
    if (!tokens) {
        return NULL;
    }

    tokens[0] = NULL;
    return tokens;
}

void free_tokens(token_t *tokens[])
{
    if (tokens)
    {
        for (token_t **ptoken = tokens; *ptoken != NULL; ptoken++)
        {
            free_token(*ptoken);
        }
        free(tokens);
    }
}

vector* parse_to_tokens(const char *str)
{
    if (regex_init())
    {
        return NULL;
    }

    vector **tokens = NULL;
    if (!(tokens = new_tokens_massive()))
    {
        printf("ERROR: parse_to_tokens. Could not allocate memmory for tokens\n");
        regex_free();
        return NULL;
    }

    token_t *token = NULL;
    while ((token = get_token(str)))
    {
        massive_add_back(tokens, token);
    }

    if (str != '\0')
    {
        printf("ERROR: parse_to_tokens. Invalid literal %s\n", str);
        free_tokens(tokens);
        regex_free();
        return NULL;
    }

    regex_free();
    return tokens;
} 