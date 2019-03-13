#include <check.h>
#include <stdio.h>

#include "../token.h"
#include "../vector.h"

token_t *token = NULL;

bool token_eq(void *token_1, void *token_2)
{
    //assert(token_1 && token_2);

    token_t *t1 = (token_t *)token_1;
    token_t *t2 = (token_t *)token_2;

    return (t1->type == t2->type) ? (!strcmp(t1->value, t2->value)) ? true : false : false;
}

void token_setup()
{
    token = new_token();
    ck_assert(token);
    ck_assert_int_eq(UNKNOWN, token->type);
    ck_assert(!token->value);
}

void token_teardown()
{
    free_token(token);
}

START_TEST(test_match_string_success)
{
    char string[] = "\"123\"";
    const char *pstr = string;
    char exc_string[] = "123";
    int error = 0;
    
    error = match_string(&pstr, token);

    ck_assert_int_eq(0, error);
    ck_assert_int_eq(STRING, token->type);
    ck_assert_str_eq(exc_string, token->value);
}
END_TEST

START_TEST(test_match_string_failed)
{
    char string[] = "\"123";
    const char *pstr = string;
    int error = 1;
    
    error = match_string(&pstr, token);

    ck_assert_int_eq(1, error);
    ck_assert_int_eq(UNKNOWN, token->type);
}
END_TEST

START_TEST(test_match_string_failed_at_start)
{
    char string[] = "123";
    const char *pstr = string;
    int error = 1;
    
    error = match_string(&pstr, token);

    ck_assert_int_eq(1, error);
    ck_assert_int_eq(UNKNOWN, token->type);
}
END_TEST

START_TEST(test_match_int_success)
{
    char string[] = "123";
    const char *pstr = string;
    char exc_string[] = "123";
    int error = 0;
    
    error = match_int(&pstr, token);

    ck_assert_int_eq(0, error);
    ck_assert_int_eq(NUMBER, token->type);
    ck_assert_str_eq(exc_string, token->value);
}
END_TEST

START_TEST(test_match_int_failed)
{
    char string[] = "\"123\"";
    const char *pstr = string;
    int error = 0;
    
    error = match_int(&pstr, token);

    ck_assert_int_eq(1, error);
    ck_assert_int_eq(UNKNOWN, token->type);
    ck_assert(!token->value);
}
END_TEST

START_TEST(test_match_int_success_with_another_string)
{
    char string[] = "123\"";
    const char *pstr = string;
    char exc_string[] = "123";
    int error = 0;
    
    error = match_int(&pstr, token);

    ck_assert_int_eq(0, error);
    ck_assert_int_eq(NUMBER, token->type);
    ck_assert_str_eq(exc_string, token->value);
}
END_TEST

START_TEST(test_match_operation_success)
{
    char string_1[] = "*";
    char string_2[] = "+";
    char string_3[] = "(";
    char string_4[] = ")";
    char exc_string_1[] = "*";
    char exc_string_2[] = "+";
    char exc_string_3[] = "(";
    char exc_string_4[] = ")";

    int error = 0;

    const char *pstr = string_1;
    error = match_operation(&pstr, token);

    ck_assert_int_eq(0, error);
    ck_assert_int_eq(OPERATION, token->type);
    ck_assert_str_eq(exc_string_1, token->value);

    free_token(token);
    token = new_token();
    pstr = string_2;
    error = match_operation(&pstr, token);

    ck_assert_int_eq(0, error);
    ck_assert_int_eq(OPERATION, token->type);
    ck_assert_str_eq(exc_string_2, token->value);


    free_token(token);
    token = new_token();
    pstr = string_3;
    error = match_operation(&pstr, token);

    ck_assert_int_eq(0, error);
    ck_assert_int_eq(OPERATION, token->type);
    ck_assert_str_eq(exc_string_3, token->value);


    free_token(token);
    token = new_token();
    pstr = string_4;
    error = match_operation(&pstr, token);

    ck_assert_int_eq(0, error);
    ck_assert_int_eq(OPERATION, token->type);
    ck_assert_str_eq(exc_string_4, token->value);
}
END_TEST

START_TEST(test_match_operation_failed)
{
    char string[] = "i*";
    const char *pstr = string;
    int error = 0;
    
    error = match_operation(&pstr, token);

    ck_assert_int_eq(1, error);
    ck_assert_int_eq(UNKNOWN, token->type);
}
END_TEST

START_TEST(test_match_token_success_int)
{
    char string[] = "123\"123\"";
    const char *pstr = string;
    char exc_string[] = "123";
    int error = 0;
    
    error = match_token(&pstr, token);

    ck_assert_int_eq(0, error);
    ck_assert_int_eq(NUMBER, token->type);
    ck_assert_str_eq(exc_string, token->value);
}
END_TEST

START_TEST(test_match_token_success_string)
{
    char string[] = "\"123\"123";
    const char *pstr = string;
    char exc_string[] = "123";
    int error = 0;
    
    error = match_token(&pstr, token);

    ck_assert_int_eq(0, error);
    ck_assert_int_eq(STRING, token->type);
    ck_assert_str_eq(exc_string, token->value);
}
END_TEST

START_TEST(test_match_token_success_operation)
{
    char string[] = "(\"123";
    const char *pstr = string;
    char exc_string[] = "(";
    int error = 0;
    
    error = match_token(&pstr, token);

    ck_assert_int_eq(0, error);
    ck_assert_int_eq(OPERATION, token->type);
    ck_assert_str_eq(exc_string, token->value);
}
END_TEST

START_TEST(test_match_token_failed)
{
    char string[] = "s*123";
    const char *pstr = string;
    int error = 0;
    
    error = match_token(&pstr, token);

    ck_assert_int_eq(1, error);
    ck_assert_int_eq(UNKNOWN, token->type);
}
END_TEST

START_TEST(test_get_token_success)
{
    char string[] = " \"123\"* 2";
    char exc_string_1[] = "123";
    char exc_string_2[] = "*";
    char exc_string_3[] = "2";
    int error = 0;
    
    token_t *token = get_token(string, &error);
    ck_assert(token);
    ck_assert_int_eq(0, error);
    ck_assert_int_eq(STRING, token->type);
    ck_assert_str_eq(exc_string_1, token->value);
    ck_assert_int_eq(1, token->position);

    free_token(token);
    token = get_token(string, &error);
    ck_assert(token);
    ck_assert_int_eq(0, error);
    ck_assert_str_eq(exc_string_2, token->value);
    ck_assert_int_eq(OPERATION, token->type);
    
    ck_assert_int_eq(6, token->position);

    free_token(token);
    token = get_token(string, &error);
    ck_assert(token);
    ck_assert_int_eq(0, error);
    ck_assert_int_eq(NUMBER, token->type);
    ck_assert_str_eq(exc_string_3, token->value);
    ck_assert_int_eq(8, token->position);

    free_token(token);
    token = get_token(string, &error);
    ck_assert(!token);
    ck_assert_int_eq(0, error);
}
END_TEST

START_TEST(test_get_token_failed_at_start)
{
    char string[] = " \"123* 2";
    int error = 0;

    token_t *token = get_token(string, &error);
    ck_assert(token);
    ck_assert_int_eq(UNKNOWN, token->type);
    ck_assert_int_eq(1, token->position);
    ck_assert_int_eq(1, error);
    free_token(token);
}
END_TEST


START_TEST(test_get_token_success_with_different_strings)
{
    char string_1[] = " \"123\"* 2";
    char string_2[] = " 2* \"123\" ";
    char exc_string_1[] = "123";
    char exc_string_2[] = "2";
    int error = 0;

    token_t *token = get_token(string_1, &error);
    ck_assert(token);
    ck_assert_int_eq(STRING, token->type);
    ck_assert_int_eq(1, token->position);
    ck_assert_str_eq(exc_string_1, token->value);
    ck_assert_int_eq(0, error);
    free_token(token);

    token = get_token(string_2, &error);
    ck_assert(token);
    ck_assert_int_eq(NUMBER, token->type);
    ck_assert_int_eq(1, token->position);
    ck_assert_str_eq(exc_string_2, token->value);
    ck_assert_int_eq(0, error);
    free_token(token);
}
END_TEST

// START_TEST(test_parse_to_tokens)
// {
//     char test_string[] = "2* (\"123\"+ \"123\" ) ";
//     char str_token_1[] = "2";   int tpos1 = 0;  token_type tp1 = NUMBER;
//     char str_token_2[] = "*";   int tpos2 = 1;  token_type tp2 = OPERATION;
//     char str_token_3[] = "(";   int tpos3 = 3;  token_type tp3 = OPERATION;
//     char str_token_4[] = "123"; int tpos4 = 4;  token_type tp4 = STRING;
//     char str_token_5[] = "+";   int tpos5 = 9;  token_type tp5 = OPERATION;
//     char str_token_6[] = "123"; int tpos6 = 11; token_type tp6 = STRING;
//     char str_token_7[] = ")";   int tpos7 = 17; token_type tp7 = OPERATION;

//     vector_t *test_tokens = new_vector(sizeof(token_t*), 16);
//     ck_assert_msg(test_tokens, "Не создался вектор токенов");

//     token_t *token = NULL;

//     ck_assert((token = new_token()));
//     ck_assert_msg(init_token(token, tp1, str_token_1, strlen(str_token_1), tpos1), "Не инициализирован первый токен");
//     ck_assert(vector_push_back(test_tokens, token));

//     ck_assert((token = new_token()));
//     ck_assert_msg(init_token(token, tp2, str_token_2, strlen(str_token_2), tpos2), "Не инициализирован второй токен");
//     ck_assert(vector_push_back(test_tokens, token));

//     ck_assert((token = new_token()));
//     ck_assert_msg(init_token(token, tp3, str_token_3, strlen(str_token_3), tpos3), "Не инициализирован третий токен");
//     ck_assert(vector_push_back(test_tokens, token));

//     ck_assert((token = new_token()));
//     ck_assert_msg(init_token(token, tp4, str_token_4, strlen(str_token_4), tpos4), "Не инициализирован четвертый токен");
//     ck_assert(vector_push_back(test_tokens, token));

//     ck_assert((token = new_token()));
//     ck_assert_msg(init_token(token, tp5, str_token_5, strlen(str_token_5), tpos5), "Не инициализирован пятый токен");
//     ck_assert(vector_push_back(test_tokens, token));

//     ck_assert((token = new_token()));
//     ck_assert_msg(init_token(token, tp6, str_token_6, strlen(str_token_6), tpos6), "Не инициализирован шестой токен");
//     ck_assert(vector_push_back(test_tokens, token));

//     ck_assert((token = new_token()));
//     ck_assert_msg(init_token(token, tp7, str_token_7, strlen(str_token_7), tpos7), "Не инициализирован седьмой токен");
//     ck_assert(vector_push_back(test_tokens, token));
    
//     ck_assert_int_eq(7, test_tokens->size);

//     vector_t *tokens = parse_to_tokens(test_string);

//     //assert_vector_eq(test_tokens, tokens);

//     free_tokens_vector(test_tokens);
//     free_tokens_vector(tokens);
// }
// END_TEST

Suite *token_suite(void)
{
    Suite *s;
    //TCase *tc_core;
    TCase *tc_token_func;

    s = suite_create("Token");

    //tc_core = tcase_create("Core");
    tc_token_func = tcase_create("token_func");

    tcase_add_checked_fixture(tc_token_func, token_setup, token_teardown);

    tcase_add_test(tc_token_func, test_match_string_success);
    tcase_add_test(tc_token_func, test_match_string_failed);
    tcase_add_test(tc_token_func, test_match_string_failed_at_start);
    tcase_add_test(tc_token_func, test_match_int_success);
    tcase_add_test(tc_token_func, test_match_int_failed);
    tcase_add_test(tc_token_func, test_match_int_success_with_another_string);
    tcase_add_test(tc_token_func, test_match_operation_success);
    tcase_add_test(tc_token_func, test_match_operation_failed);
    tcase_add_test(tc_token_func, test_match_token_success_int);
    tcase_add_test(tc_token_func, test_match_token_success_string);
    tcase_add_test(tc_token_func, test_match_token_success_operation);
    tcase_add_test(tc_token_func, test_match_token_failed);
    tcase_add_test(tc_token_func, test_get_token_success);
    tcase_add_test(tc_token_func, test_get_token_failed_at_start);
    tcase_add_test(tc_token_func, test_get_token_success_with_different_strings);


    //tcase_add_test(tc_core, );
    //tcase_add_test(tc_core, );
    //tcase_add_test(tc_core, );
    //tcase_add_test(tc_core, );
    //suite_add_tcase(s, tc_core);
    suite_add_tcase(s, tc_token_func);

    return s;
}