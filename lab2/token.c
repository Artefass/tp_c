#include "token.h"

#define PTRN_STRING "^\"\d\""
#define PTRN_NUMBER "^\d"
#define PTRN_OPERATION "^(*|+|-|(|))"

#define SET_PATTERN_TOKEN(type, name, pattern) (token_t){.type=type,.name=name,.pattern=pattern}

static regex_token_t regex_tokens[] = {
    SET_PATTERN_TOKEN(STRING,'string', PTRN_STRING),
    SET_PATTERN_TOKEN(NUMBER,'number', PTRN_NUMBER),
    SET_PATTERN_TOKEN(OPERATION,'operation',PTRN_OPERATION)    
}

static int regex_init()
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


static int match_token(const char *expression, token_t token) 
{
    assert(expression != NULL);

    token_t token = {.type=UNNOWN};

    regmatch_t match_data;
    for (int i = 0; i < ARRAYNUM(regex_tokens); i++){
        if (!regexec(&regex_tokens[i].re, expression, match_data, )){
            
        } 
    }

    return token;
}

