#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>


void print_vector(vector_t *vector)
{
    assert(vector);

    printf("\n Vector data:");
    for (int i = 0; i < vector->size; i++)
    {
        void *data;
        
        switch (vector->data_size)
        {
            case sizeof(int):
                data = vector->data;
                printf("%d ", ((int*)data)[i]);
                break;
            case sizeof(char):
                data = vector->data;
                printf("%c", ((char*)data)[i]);
                break;
            default:
                break;
        }
        
    }
    printf("\n");
}


vector_t* new_vector(int data_size, int capacity)
{
    assert(data_size > 0 && capacity > 0);

    vector_t *vector = calloc(sizeof(vector_t), 1);
    
    if (!vector)
    {
        printf("ERROR: new_vector. Could not allocate memmory for vector\n");
        return NULL;
    }

    vector->capacity = capacity;
    vector->data_size = data_size;
    vector->size = 0;

    vector->data = calloc(data_size, capacity);
    if (!vector->data)
    {
        printf("ERROR: new vector. Could not allocate memmoty for data\n");
        free(vector);
        return NULL;
    }

    return vector;
}

void free_vector(vector_t *vector)
{
    if (vector)
    {
        if (vector->data)
        {
            free(vector->data);
        }

        free(vector);
    }
}

static bool realloc_vector(vector_t *vector, int size)
{
    assert(vector && size > vector->size);

    if (vector->capacity > size)
    {
        return true;
    }

    int new_capacity = vector->capacity;
    while (new_capacity < size)
    {
        new_capacity *= 2;
    }

    void *data = realloc(vector->data, new_capacity * vector->data_size);
    if (!data)
    {

        printf("ERROR: realloc_vector. Could no reallocate memmory for data\n");
        return false;
    }

    vector->data = data;
    vector->capacity = new_capacity;

    return true;
}

bool vector_push_back(vector_t *vector, void *data)
{
    assert(vector && data);

    if (vector->size == vector->capacity)
    {
        if (!realloc_vector(vector, vector->size + 1))
        {
            printf("ERROR: vector_push_back. Could not reallocate memmory for data\n");
            return false;
        }
    }

    if (!memcpy(vector->data + vector->data_size * vector->size, data, vector->data_size))
    {
        printf("ERROR: vector_push_back. Could not copy data to vector\n");
        return false;
    }

    vector->size++;
    return true;
}

bool vector_copy_back(vector_t *vector, void *data, int size)
{
    assert(vector && data && size > 0);

    if (vector->size + size > vector->capacity)
    {
        if (!realloc_vector(vector, vector->size + size))
        {
            printf("ERROR: vector_push_back. Could not reallocate memmory for data\n");
            return false;
        }
    }

    if (!memcpy(vector->data + vector->data_size * vector->size, data, vector->data_size * size))
    {
        printf("ERROR: vector_copy_back. Could not copy data\n");
        return false;
    }

    vector->size += size;
    return true;
}

bool vector_insert_at(vector_t *vector, void *data, int pos)
{
    assert(vector && data && pos >= 0 && pos < vector->size);

    if (vector->size == vector->capacity)
    {
        if(!realloc_vector(vector, vector->size + 1))
        {
            printf("ERROR: vector_insert_at. Could not reallocate vector\n");
            return false;
        }
    }

    int tail_count = vector->size - pos;
    if (!memmove(vector->data + vector->data_size * (pos + 1),
            vector->data + vector->data_size * pos,
            vector->data_size * tail_count))
    {
        printf("ERROR: vector_insert_at. Could not copy element\n");
        return false;
    }

    if (!memcpy(vector->data + vector->data_size * pos, data, vector->data_size))
    {
        printf("ERROR: vector_copy_back. Could not copy data\n");
        return false;
    }

    vector->size++;
    return true;
}

bool vector_delete_at(vector_t *vector, int pos)
{
    assert(vector && pos >= 0 && pos < vector->size);

    if (pos < vector->size - 1)
    {
        int tail_count = vector->size - pos - 1;
        if (!memcpy(vector->data + vector->data_size * pos, 
            vector->data + vector->data_size * (pos+1),
            tail_count * vector->data_size))
        {
            printf("ERROR: vector_delete_at. Could not copy data\n");
            return false;
        }
    }

    vector->size--;
    return true;
}


