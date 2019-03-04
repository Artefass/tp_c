#pragma once

#include <stdbool.h>

/**
 * структура вектора
 */
typedef struct vector_t {
    
    /**
     * текущий размер
     */
    int size;
    
    /*
     * размер одного элемента
     */
    int data_size;
    
    /*
     * максимальная вместимость вектора
     */
    int capacity;
    
    /**
     * указатель на массив элементов
     */
    void *data;
} vector_t;

void print_vector(vector_t *vector);

vector_t* new_vector(int data_size, int capacity);
void free_vector(vector_t *vector);
bool vector_push_back(vector_t *vector, void *data);
bool vector_copy_back(vector_t *vector, void *data, int size);
bool vector_insert_at(vector_t *vector, void *data, int pos);
bool vector_delete_at(vector_t *vector, int pos);