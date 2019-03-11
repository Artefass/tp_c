#include <check.h>
#include <string.h>
#include <stdbool.h>

#include "../vector.h"

#define SIZE 4

vector_t *vector;

bool assert_vector_eq_data(vector_t *vector, void *data)
{
    return !memcmp(vector->data, data, vector->size * vector->data_size);
}


void setup()
{
    vector = new_vector(sizeof(int), SIZE);
    ck_assert_int_eq(0, vector->size);
    ck_assert_int_eq(SIZE, vector->capacity);
    ck_assert_int_eq(sizeof(int), vector->data_size);
    ck_assert_msg(vector->data, "Not allocate data");
}

void teardown()
{
    free_vector(vector);
    vector = NULL;
}

START_TEST(test_vector_init)
{
    ck_assert(vector);
}
END_TEST

START_TEST(test_vector_push_back)
{
    int a[SIZE+1] = {1,2,3,4,5};
    
    // добавление в пустой вектор
    ck_assert_int_eq(0, vector->size);
    vector_push_back(vector, &a[0]);
    ck_assert_int_eq(1, vector->size);
    ck_assert_msg(!memcmp(vector->data, &a[0], vector->data_size));  

    // добавление не в пустой вектор
    ck_assert_int_lt(0, vector->size);
    for (int i = 1; i < SIZE; i++)
    {
        vector_push_back(vector, &a[i]);
    }
    ck_assert_int_eq(SIZE, vector->size);
    ck_assert_msg(!memcmp(vector->data, a, vector->size * vector->data_size));

    // добавление в переполненый вектор
    ck_assert_int_eq(SIZE, vector->size);
    ck_assert_int_eq(SIZE, vector->capacity);
    vector_push_back(vector, &a[SIZE]);

    ck_assert_int_eq(SIZE+1, vector->size);
    ck_assert_int_lt(SIZE+1, vector->capacity);
    ck_assert_msg(!memcmp(vector->data, a, vector->size * vector->data_size));
}
END_TEST

START_TEST(test_vector_copy_back)
{
    int a[SIZE+1] = {1,2,3,4,5};

    // копирование в пустой ветор
    ck_assert_int_eq(0, vector->size);
    vector_copy_back(vector, a, 1);
    ck_assert_int_eq(1, vector->size);
    ck_assert_msg(!memcmp(vector->data, a, vector->size * vector->data_size));
    
    // копирование в непустой вектор
    ck_assert_int_lt(0, vector->size);
    vector_copy_back(vector, &a[1], SIZE - 1);
    ck_assert_int_eq(SIZE, vector->size);
    ck_assert_msg(!memcmp(vector->data, a, vector->size * vector->data_size));

    // копирование в заполненый вектор
    ck_assert_int_eq(SIZE, vector->size);
    ck_assert_int_eq(SIZE, vector->capacity);
    vector_copy_back(vector, &a[SIZE], 1);
    ck_assert_int_lt(SIZE+1, vector->capacity);
    ck_assert_int_eq(SIZE+1, vector->size);
    ck_assert_msg(!memcmp(vector->data, a, vector->size * vector->data_size));
}
END_TEST

START_TEST(test_vector_insert_at)
{
    int a[SIZE+1] = {1,2,3,4,5};

    // если добавить элемент в пустой вектор, то вылетит assert
    // поэтому добавим один элемент
    vector_copy_back(vector, &a[1], 1);

    // добавление в начало
    ck_assert_int_eq(1, vector->size);
    ck_assert_msg(assert_vector_eq_data(vector, &a[1]));
    vector_insert_at(vector, &a[0], 0);
    ck_assert_int_eq(2, vector->size);
    ck_assert_msg(assert_vector_eq_data(vector, &a[0]));

    // добавление в середину(любая позиция между началом и концом)
    vector_clean(vector);
    ck_assert_int_eq(0, vector->size);
    vector_push_back(vector, &a[0]);
    vector_push_back(vector, &a[2]);
    ck_assert_int_eq(2, vector->size);

    vector_insert_at(vector, &a[1], 1);
    ck_assert_int_eq(3, vector->size);
    ck_assert_msg(assert_vector_eq_data(vector, a));
    
    // добавление в переполненный вектор
    vector_clean(vector);
    ck_assert_int_eq(0, vector->size);
    vector_copy_back(vector, &a[0], SIZE - 1);
    vector_push_back(vector, &a[SIZE]);
    ck_assert_int_eq(SIZE, vector->size);
    ck_assert_int_eq(SIZE, vector->capacity);
    
    vector_insert_at(vector, &a[SIZE-1], SIZE-1);
    ck_assert_int_eq(SIZE+1, vector->size);
    ck_assert_int_lt(SIZE+1, vector->capacity);
    ck_assert_msg(assert_vector_eq_data(vector, a));
}
END_TEST

START_TEST(test_vector_delete_at)
{
    int a0[SIZE] = {1,2,3,4}; // начальное состояние вектора
    int a1[SIZE] = {2,3,4}; // удаление первого элемента
    int a2[SIZE] = {1,3,4}; // удаление второго элемента
    int a3[SIZE] = {1,2,3}; // удаление последнего элемента

    // удаление первого элемента
    vector_copy_back(vector, a0, SIZE);
    ck_assert_int_eq(SIZE, vector->size);
    ck_assert_msg(assert_vector_eq_data(vector, a0));

    vector_delete_at(vector, 0);
    ck_assert_int_eq(SIZE-1, vector->size);
    ck_assert_msg(assert_vector_eq_data(vector, a1));
    vector_clean(vector);

    // удаление второго элемента
    vector_copy_back(vector, a0, SIZE);
    ck_assert_int_eq(SIZE, vector->size);
    ck_assert_msg(assert_vector_eq_data(vector, a0));

    vector_delete_at(vector, 1);
    ck_assert_int_eq(SIZE-1, vector->size);
    ck_assert_msg(assert_vector_eq_data(vector, a2));
    vector_clean(vector);

    // удаление последнего элемента
    vector_copy_back(vector, a0, SIZE);
    ck_assert_int_eq(SIZE, vector->size);
    ck_assert_msg(assert_vector_eq_data(vector, a0));

    vector_delete_at(vector, vector->size - 1);
    ck_assert_int_eq(SIZE-1, vector->size);
    ck_assert_msg(assert_vector_eq_data(vector, a3));
    vector_clean(vector);
}
END_TEST


Suite *vector_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Vector");

    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_vector_init);
    tcase_add_test(tc_core, test_vector_push_back);
    tcase_add_test(tc_core, test_vector_copy_back);
    tcase_add_test(tc_core, test_vector_insert_at);
    tcase_add_test(tc_core, test_vector_delete_at);
    suite_add_tcase(s, tc_core);

    return s;
}