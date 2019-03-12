#include <check.h>
#include <stdio.h>

#include "../token.h"

token_t *token = NULL;

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