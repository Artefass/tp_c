#include "token.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <regex.h>

#include "vector.h"
#include "utils.h"


#define MEMERR 2
#define SUCCESS 0
#define NOT_MATCH 1

typedef int(*match_func_t)(const char**, token_t*);

token_t* new_token()
{
    token_t *token = calloc(sizeof(token_t), 1);
    
    if (!token) 
    {
        printf("Error: new_token faild. could not allocate memmory\n");
        return NULL;
    }

    token->type = UNKNOWN;
    token->value = NULL;
    return token;
}

void free_token(token_t *token)
{
    assert(token);

    if (token->name) free(token->name);
    if (token->value) free(token->value);

    free(token);
}

int match_string(const char **str, token_t *token)
{
    assert(str && token);

    const char *pstr = *str;
    int len = 0;
    // если нашли кавычки, то предпологаем, что начинается строка
    if (*pstr == '"')
    {
        // начинаем искать кавычку со второй позиции
        pstr++;
        while(*pstr != '"' && *pstr != '\0')
        {
            len++;
            pstr++;
        }
        // проверяем, нашли ли вторую ковычку, если нет, то это не строка
        if (*pstr == '"')
        {
            // копируем на 1 симол больше, чтобы в конце вставить '\0'
            // вместо кавычки
            token->value = strndup(&(*str)[1], len+1);
            if (!token->value)
            {
                // ошибка памяти, не смогли скопировать часть строки
                return MEMERR;
            }
            token->value[len] = '\0';
            token->type = STRING;
            *str = pstr;
            (*str)++;

            return SUCCESS;
        }
    }
    return NOT_MATCH;
}

int match_int(const char **str, token_t *token)
{
    assert(str && token);

    const char *pstr = *str;
    int len = 0;

    while (*pstr >= '0' && *pstr <= '9' && *pstr != '\0')
    {
        len++;
        pstr++;
    }

    // нашли число
    if (len > 0)
    {
        // копируем на 1 симол больше, чтобы в конце (len+1) вставить '\0'.
        // это работает, даже если скопируем нулевой символ
        token->value = strndup(*str, len+1);
        if (!token->value)
        {
            // ошибка памяти, не смогли скопировать часть строки
            return MEMERR;
        }
        token->value[len] = '\0';
        token->type = NUMBER;
        *str = pstr;

        return SUCCESS;    
    }

    return NOT_MATCH;
}

int match_operation(const char **str, token_t *token)
{
    assert(str && token);

    switch (**str)
    {
        case '+':  
        case '*':
        case '(':
        case ')':
            // копируем на 1 симол больше, чтобы в конце вставить '\0'.
            // это работает, даже если скопируем нулевой символ
            token->value = strndup(*str, 2);
            if (!token->value)
            {
                // ошибка памяти, не смогли скопировать часть строки
                return MEMERR;
            }
            token->value[1] = '\0';
            token->type = OPERATION;
            (*str)++;
            return SUCCESS;
        default:
            return NOT_MATCH;
    }
}

int match_token(const char **str, token_t *token)
{
    match_func_t match_funcs[] = {
        match_int,
        match_string,
        match_operation
    };

    int error = 0;
    for (int i = 0; i < ARRAYNUM(match_funcs); i++)
    {
        error = match_funcs[i](str, token);
        if (error == SUCCESS)
            return SUCCESS;
        else if (error == MEMERR)
            return MEMERR;
    }

    return NOT_MATCH;
}

token_t* get_token(const char *str, int *error)
{
    assert(str);

    // статические переменные нужны для того, чтобы вычислять позицию токена
    // в строке
    static const char *start = NULL;
    static const char *current = NULL;

    printf("str: %s\n", str);

    // первоначальная инициализация
    if (start == NULL)
    {
        start = current = str;
        
    }

    printf("start: %s\n", start);
    printf("current: %s\n", current);

    

    // если до этого вызывали функцию, но на вход подали другую строку
    if (start != str)
    {
        start = current = str;
    }

    // пропуск пробелов
    while(*current == ' ' && *current != '\0') current++;

    // если достигли конца строки, то все хорошо, обработка 
    // завершена. Ошибки нет.
    if (*current == '\0')
    {
        *error = SUCCESS;
        return NULL;
    }

    token_t *token;
    if (!(token = new_token())) 
    {
        *error = MEMERR;
        return NULL;
    }

    // определение позиции токена. (разница между начальной и текущей позиции
    // строки)
    token->position = current - start;
    *error = match_token(&current, token);
    return token;
}






/*

#define SET_PATTERN_TOKEN(type, name, pattern) (regex_token_t){(type), (name), (pattern)}

int regex_compile(regex_token_t *regex_tokens)
{
    int error;
    for (int i = 0; i < ARRAYNUM(regex_tokens); i++)
    {
        if ((error = regcomp(&regex_tokens[i].re, regex_tokens[i].pattern, REG_EXTENDED|REG_NOSUB)) > 0)
        {
            printf("ERROR: regcopm faild. error: %d pattern:%s", error, regex_tokens[i].pattern);
            return error;
        }
    }

    return 0;
}


void regex_free(regex_token_t *regex_tokens)
{
    for (int i = 0; i < ARRAYNUM(regex_tokens); i++)
    {
         regfree(&regex_tokens[i].re);
    }
}



bool init_token(const regex_token_t *regex_token, token_t *token, const char *str, int copy_len)
{
    token->name = strdup(regex_token->name);
    token->value = strndup(str, copy_len);
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

bool match_token(const char *str, const regex_token_t *regex_tokens, token_t *token) 
{
    assert(str && regex_tokens && token);

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



#define BASE_SIZE 16

vector_t* new_tokens_vector()
{
    vector_t *tokens = new_vector(sizeof(token_t*), BASE_SIZE);
    if (!tokens) {
        return NULL;
    }

    return tokens;
}

void free_tokens_vector(vector_t* tokens)
{
    if (tokens)
    {
        for (int i = 0; i < tokens->size; i++)
        {
            free_token(((token_t**)tokens->data)[i]);
        }

        free_vector(tokens);
    }
}

vector_t* parse_to_tokens(const char *str)
{
    regex_token_t regex_tokens[] = {
        SET_PATTERN_TOKEN(STRING,"string", PTRN_STRING),
        SET_PATTERN_TOKEN(NUMBER,"number", PTRN_NUMBER),
        SET_PATTERN_TOKEN(OPERATION,"operation",PTRN_OPERATION)    
    };

    if (!regex_compile(regex_tokens))
    {
        return NULL;
    }

    vector_t *tokens = NULL;
    if (!(tokens = new_tokens_vector()))
    {
        printf("ERROR: parse_to_tokens. Could not allocate memmory for tokens\n");
        regex_free(regex_tokens);
        return NULL;
    }

    token_t *token = NULL;
    while ((token = get_token(str, regex_tokens)) && token->type != UNKNOWN)
    {
        if(!vector_push_back(tokens, token))
        {
            printf("ERROR: parse_to_tokens. Could not insert token");
            free_token(token);
            free_tokens_vector(tokens);
            regex_free(regex_tokens);
            return NULL;
        }
    }

    if (token->type == UNKNOWN)
    {
        printf("ERROR: parse_to_tokens. Invalid literal %s\n", &str[token->position]);
        free_token(token);
        free_tokens_vector(tokens);
        regex_free(regex_tokens);
        return NULL;
    }

    regex_free(regex_tokens);
    return tokens;
}

void print_tokens(vector_t *tokens)
{
    assert(tokens);
    
    for (int i = 0; i < tokens->size; i++)
    {
        token_t *token = ((token_t**)tokens->data)[i];
        printf("Token #%d:\n", i);
        printf("\tname: %s\n", token->name);
        printf("\ttype: %d\n", token->type);
        printf("\tposition: %d\n", token->position);
        printf("\tvalue: %s\n", token->value);
    }
}
*/
