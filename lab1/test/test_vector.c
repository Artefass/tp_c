#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "../vector.h"
#include "../utils.h"

START_TEST(test_vector_init)
{
    int size = 5;
    vector_t *vector = new_vector(sizeof(int), size);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(size, vector->capacity);
    free_vector(vector);
}
END_TEST

START_TEST(test_vector_push_back)
{
    int a[] = {1,2,3,4,5};
    int size  = ARRAYNUM(a);

    vector_t *vector = new_vector(sizeof(int), size);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(size, vector->capacity);
    for (int i = 0; i < size; i++)
    {
        vector_push_back(vector, &a[i]);
    }
    ck_assert_int_eq(size, vector->capacity);
    ck_assert_msg(!memcmp(vector->data, a, size * sizeof(int)),
        "Elements in vector are not excepted");
    free_vector(vector);
}
END_TEST

START_TEST(test_vector_push_back_with_overflow_vector)
{
    int a[] = {1,2,3,4,5};
    int size  = ARRAYNUM(a);

    vector_t *vector = new_vector(sizeof(int), size - 1);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(size - 1, vector->capacity);

    for (int i = 0; i < size - 1; i++)
    {
        vector_push_back(vector, &a[i]);
    }

    ck_assert_int_eq(size - 1, vector->capacity);
    ck_assert_int_eq(size - 1, vector->size);
    ck_assert_msg(!memcmp(vector->data, a, (size-1) * sizeof(int)),
        "Elements in vector are not excepted");

    vector_push_back(vector, &a[size - 1]);

    ck_assert_int_lt(size, vector->capacity);
    ck_assert_int_eq(size, vector->size);
    
    ck_assert_msg(!memcmp(vector->data, a, size * sizeof(int)),
        "Elements in vector are not excepted");

    free_vector(vector);
}
END_TEST

START_TEST(test_vector_copy_back_one_element)
{
    int a[] = {1,2,3,4,5};
    int size  = ARRAYNUM(a);

    vector_t *vector = new_vector(sizeof(int), size);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(size, vector->capacity);

    for (int i = 0; i < size - 1; i++)
    {
        vector_push_back(vector, &a[i]);
    }

    ck_assert_int_le(size - 1, vector->capacity);
    ck_assert_int_eq(size - 1, vector->size);
    ck_assert_msg(!memcmp(vector->data, a, (size-1) * sizeof(int)),
        "Elements in vector are not excepted");

    vector_copy_back(vector, &a[size - 1], 1);

    ck_assert_int_le(size, vector->capacity);
    ck_assert_int_eq(size, vector->size);
    
    ck_assert_msg(!memcmp(vector->data, a, size * sizeof(int)),
        "Elements in vector are not excepted");

    free_vector(vector);
}
END_TEST

START_TEST(test_vector_copy_back_one_element_with_overflow_vector)
{
    int a[] = {1,2,3,4,5};
    int size  = ARRAYNUM(a);

    vector_t *vector = new_vector(sizeof(int), size - 1);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(size - 1, vector->capacity);

    for (int i = 0; i < size - 1; i++)
    {
        vector_push_back(vector, &a[i]);
    }

    ck_assert_int_le(size - 1, vector->capacity);
    ck_assert_int_eq(size - 1, vector->size);
    ck_assert_msg(!memcmp(vector->data, a, (size-1) * sizeof(int)),
        "Elements in vector are not excepted");

    vector_copy_back(vector, &a[size - 1], 1);

    ck_assert_int_le(size, vector->capacity);
    ck_assert_int_eq(size, vector->size);
    
    ck_assert_msg(!memcmp(vector->data, a, size * sizeof(int)),
        "Elements in vector are not excepted");

    free_vector(vector);
}
END_TEST

START_TEST(test_vector_copy_back_two_element)
{
    int a[] = {1,2,3,4,5};
    int size  = ARRAYNUM(a);

    vector_t *vector = new_vector(sizeof(int), size);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(0, vector->size);
    ck_assert_int_eq(size, vector->capacity);

    for (int i = 0; i < size - 2; i++)
    {
        vector_push_back(vector, &a[i]);
    }

    ck_assert_int_le(size - 2, vector->capacity);
    ck_assert_int_eq(size - 2, vector->size);
    ck_assert_msg(!memcmp(vector->data, a, (size-2) * sizeof(int)),
        "Elements in vector are not excepted");

    vector_copy_back(vector, &a[size - 2], 2);

    ck_assert_int_le(size, vector->capacity);
    ck_assert_int_eq(size, vector->size);
    
    ck_assert_msg(!memcmp(vector->data, a, size * sizeof(int)),
        "Elements in vector are not excepted");

    free_vector(vector);
}
END_TEST

START_TEST(test_vector_copy_back_two_element_with_overflow_vector)
{
    int a[] = {1,2,3,4,5};
    int size  = ARRAYNUM(a);

    vector_t *vector = new_vector(sizeof(int), size-2);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(0, vector->size);
    ck_assert_int_eq(size-2, vector->capacity);

    for (int i = 0; i < size - 2; i++)
    {
        vector_push_back(vector, &a[i]);
    }

    ck_assert_int_le(size - 2, vector->capacity);
    ck_assert_int_eq(size - 2, vector->size);
    ck_assert_msg(!memcmp(vector->data, a, (size-2) * sizeof(int)),
        "Elements in vector are not excepted");

    vector_copy_back(vector, &a[size - 2], 2);

    ck_assert_int_le(size, vector->capacity);
    ck_assert_int_eq(size, vector->size);
    
    ck_assert_msg(!memcmp(vector->data, a, size * sizeof(int)),
        "Elements in vector are not excepted");

    free_vector(vector);
}
END_TEST

START_TEST(test_vector_insert_at_start)
{
    int a[] = {1,2,3,4,5};
    int size  = ARRAYNUM(a);

    vector_t *vector = new_vector(sizeof(int), size);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(0, vector->size);
    ck_assert_int_eq(size, vector->capacity);

    for (int i = 1; i < size; i++)
    {
        vector_push_back(vector, &a[i]);
    }

    ck_assert_int_le(size - 1, vector->capacity);
    ck_assert_int_eq(size - 1, vector->size);
    ck_assert_msg(!memcmp(vector->data, &a[1], (size-1) * sizeof(int)),
        "Elements in vector are not excepted");

    vector_insert_at(vector, &a[0], 0);

    ck_assert_int_le(size, vector->capacity);
    ck_assert_int_eq(size, vector->size);
    
    ck_assert_msg(!memcmp(vector->data, a, size * sizeof(int)),
        "Elements in vector are not excepted");

    free_vector(vector);
}
END_TEST

START_TEST(test_vector_insert_at_start_with_overflow_vector)
{
    int a[] = {1,2,3,4,5};
    int size  = ARRAYNUM(a);

    vector_t *vector = new_vector(sizeof(int), size-1);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(0, vector->size);
    ck_assert_int_eq(size-1, vector->capacity);

    for (int i = 1; i < size; i++)
    {
        vector_push_back(vector, &a[i]);
    }

    ck_assert_int_le(size - 1, vector->capacity);
    ck_assert_int_eq(size - 1, vector->size);
    ck_assert_msg(!memcmp(vector->data, &a[1], (size-1) * sizeof(int)),
        "Elements in vector are not excepted");

    vector_insert_at(vector, &a[0], 0);

    ck_assert_int_le(size, vector->capacity);
    ck_assert_int_eq(size, vector->size);
    
    ck_assert_msg(!memcmp(vector->data, a, size * sizeof(int)),
        "Elements in vector are not excepted");

    free_vector(vector);
}
END_TEST

START_TEST(test_vector_insert_at_befor_end)
{
    int a[] = {1,2,3,4,5};
    int right_a[] = {1,2,3,5,4};
    int size  = ARRAYNUM(a);

    vector_t *vector = new_vector(sizeof(int), size);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(0, vector->size);
    ck_assert_int_eq(size, vector->capacity);

    for (int i = 0; i < size-1; i++)
    {
        vector_push_back(vector, &a[i]);
    }

    ck_assert_int_le(size - 1, vector->capacity);
    ck_assert_int_eq(size - 1, vector->size);
    ck_assert_msg(!memcmp(vector->data, &a[0], (size-1) * sizeof(int)),
        "Elements in vector are not excepted");

    vector_insert_at(vector, &a[size-1], size-2);

    ck_assert_int_le(size, vector->capacity);
    ck_assert_int_eq(size, vector->size);
    
    ck_assert_msg(!memcmp(vector->data, right_a, size * sizeof(int)),
        "Elements in vector are not excepted");

    free_vector(vector);
}
END_TEST

START_TEST(test_vector_insert_at_befor_end_with_overflow_vector)
{
    int a[] = {1,2,3,4,5};
    int right_a[] = {1,2,3,5,4};
    int size  = ARRAYNUM(a);

    vector_t *vector = new_vector(sizeof(int), size-1);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(0, vector->size);
    ck_assert_int_eq(size-1, vector->capacity);

    for (int i = 0; i < size-1; i++)
    {
        vector_push_back(vector, &a[i]);
    }

    ck_assert_int_le(size - 1, vector->capacity);
    ck_assert_int_eq(size - 1, vector->size);
    ck_assert_msg(!memcmp(vector->data, &a[0], (size-1) * sizeof(int)),
        "Elements in vector are not excepted");

    vector_insert_at(vector, &a[size-1], size-2);

    ck_assert_int_le(size, vector->capacity);
    ck_assert_int_eq(size, vector->size);
    
    ck_assert_msg(!memcmp(vector->data, right_a, size * sizeof(int)),
        "Elements in vector are not excepted");

    free_vector(vector);
}
END_TEST


START_TEST(test_vector_delete_at_start)
{
    int a[] = {1,2,3,4,5};
    int right_a[] = {2,3,4,5};
    int size  = ARRAYNUM(a);

    vector_t *vector = new_vector(sizeof(int), size);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(0, vector->size);
    ck_assert_int_eq(size, vector->capacity);

    for (int i = 0; i < size; i++)
    {
        vector_push_back(vector, &a[i]);
    }

    ck_assert_int_le(size, vector->capacity);
    ck_assert_int_eq(size, vector->size);
    ck_assert_msg(!memcmp(vector->data, &a[0], size * sizeof(int)),
        "Elements in vector are not excepted");

    vector_delete_at(vector, 0);

    ck_assert_int_le(size-1, vector->capacity);
    ck_assert_int_eq(size-1, vector->size);
    
    ck_assert_msg(!memcmp(vector->data, right_a, vector->size * sizeof(int)),
        "Elements in vector are not excepted");

    free_vector(vector);
}
END_TEST

START_TEST(test_vector_delete_at_end)
{
    int a[] = {1,2,3,4,5};
    int right_a[] = {1,2,3,4};
    int size  = ARRAYNUM(a);

    vector_t *vector = new_vector(sizeof(int), size);
    ck_assert_msg(vector != NULL, "Vector not allcoate\n");
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_int_eq(0, vector->size);
    ck_assert_int_eq(size, vector->capacity);

    for (int i = 0; i < size; i++)
    {
        vector_push_back(vector, &a[i]);
    }

    ck_assert_int_le(size, vector->capacity);
    ck_assert_int_eq(size, vector->size);
    ck_assert_msg(!memcmp(vector->data, &a[0], size * sizeof(int)),
        "Elements in vector are not excepted");

    vector_delete_at(vector, vector->size - 1);

    ck_assert_int_le(size-1, vector->capacity);
    ck_assert_int_eq(size-1, vector->size);
    
    ck_assert_msg(!memcmp(vector->data, right_a, vector->size * sizeof(int)),
        "Elements in vector are not excepted");

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
    tcase_add_test(tc_core, test_vector_push_back_with_overflow_vector);
    tcase_add_test(tc_core, test_vector_copy_back_one_element);
    tcase_add_test(tc_core, test_vector_copy_back_one_element_with_overflow_vector);
    tcase_add_test(tc_core, test_vector_copy_back_two_element);
    tcase_add_test(tc_core, test_vector_copy_back_two_element_with_overflow_vector);
    tcase_add_test(tc_core, test_vector_insert_at_start);
    tcase_add_test(tc_core, test_vector_insert_at_start_with_overflow_vector);
    tcase_add_test(tc_core, test_vector_insert_at_befor_end);
    tcase_add_test(tc_core, test_vector_insert_at_befor_end_with_overflow_vector);
    tcase_add_test(tc_core, test_vector_delete_at_start);
    tcase_add_test(tc_core, test_vector_delete_at_end);
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