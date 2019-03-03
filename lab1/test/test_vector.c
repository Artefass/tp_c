#include <check.h>

#include "../vector.h"

#define SIZE 4

START_TEST(test_vector_init)
{
    int size = 4;
    vector_t *vector = new_vector(sizeof(int), size);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(size, vector->capacity);
    free_vector(vector);
}
END_TEST

START_TEST(test_vector_push_back)
{
    int a[SIZE] = {1,2,3,4};

    vector_t *vector = new_vector(sizeof(int), SIZE);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(SIZE, vector->capacity);
    for (int i = 0; i < SIZE; i++)
    {
        vector_push_back(vector, &a[i]);
    }
    ck_assert_mem_eq(vector->data, a, SIZE * sizeof(int));
    free_vector(vector);
}
END_TEST

Suite *vector_suite(void)
{
    Suite *s;
    TCase *tc_core;
    
    s = suite_create("Vector");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_vector_init);
    tcase_add_test(tc_core, test_vector_push_back);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int no_faild = 0;
    Suite *s;
    SRunner *runner;

    s = vector_suite();
    runner = srunner_create(s);

    srunner_run_all(runner, CK_NORMAL);
    no_faild = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (no_faild == 0) ? 0 : -1;
}

