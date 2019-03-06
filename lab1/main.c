// Кривцов Павел АПО-13

// Чтобы показать информацию о пришедшем письме, нужно сначала её найти.
// Для этого в файле письма необходимо найти специальные заголовки.

// Составить программу построчной фильтрации текста.
// Суть фильтра — отбор строк, начинающихся с одного из следующих выражений: 
// «Date:», «From:»,«To:», «Subject:».
// Текстовые строки подаются на стандартный ввод программы, результат 
// программы должен подаваться на стандартный вывод.

// Процедура фильтрации должна быть оформлена в виде отдельной функции, 
// которой подается на вход массив строк, выделенных в динамической памяти и его длина.
// На выходе функция возвращает указатель на NULL-терминированный массив с 
// найденными строками (последним элементом массива добавлен NULL для обозначения, 
// что данных больше нет).

// Программа должна уметь обрабатывать возникающие ошибки (например, ошибки 
// выделения памяти). В случае возникновения ошибки нужно вывести в поток 
// стандартного вывода сообщение "[error]" и завершить выполнение программы. 

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

void free_nstrings(char **strings, int size)
{
    assert(strings);

    for (int i = 0 ; i < size; i++)
    {
        free(strings[i]);
    }
    free(strings);
}

void free_strings(char **strings)
{
    assert(strings);

    char **pstr = strings;
    while (*pstr != NULL)
    {
        free(*pstr);
        pstr++;
    }

    free(strings); 
}

void print_strings(char **strings)
{
    assert(strings);

    char **pstr = strings;
    while (*pstr != NULL)
    {
        printf("%s\n", *pstr);
        pstr++;
    }
}

void print_nstrings(char **strings, int size)
{
    assert(strings);

    for (int i = 0; i < size; i++)
    {
        printf("%s\n", strings[i]);
    }
}

bool is_description(char *str)
{
    assert(str != NULL);

    int len = strlen(str);

    if (strlen("Date:") <= len && !strncmp(str, "Date:", strlen("Date:")))
    {
        return true;
    }
    else if (strlen("From:") <= len && !strncmp(str, "From:", strlen("From:")))
    {
        return true;
    }
    else if (strlen("To:") <= len && !strncmp(str, "To:", strlen("To:")))
    {
        return true;
    }
    else if (strlen("Subject:") <= len && !strncmp(str, "Subject:", strlen("Subject:")))
    {
        return true;
    }

    return false;
}

char** find_descriptions(char **lines, int nlines)
{
    assert(lines);

    char **descriptions = calloc(sizeof(char*), nlines + 1);
    int ndes = 0;

    if (!descriptions)
    {
        return NULL;
    }

    for (int i = 0; i < nlines; i++)
    {
        if (is_description(lines[i]))
        {
            descriptions[ndes] = strdup(lines[i]);
            if (!descriptions[ndes])
            {
                for (int j = 0; j < ndes; j++)
                {
                    free(descriptions[j]);
                    return NULL;
                }
            }

            ndes++;
        }
    }

    descriptions[ndes] = NULL;
    return descriptions;
}


char* get_line()
{
    int line_size = 32;

    // выделяем память под струку
    char *line  = malloc(sizeof(char) * line_size);
    if (!line)
    {
        return NULL;
    }

    int len = 0;
    char c;
    // считываем по одному символу, пока не встретим перевод строки
    // или пустой ввод
    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (line_size <= len + 1) // хватит ли у нас памяти под следующие символы
                                // к примеру, если цикл закончится, а строка вся заполнена
                                // то символ конца строки уже не уместится, а так условие 
                                // (line_size <= i + 1) заранее проверит наличие памяти 
                                //под следующие символы 
        {
            char *tmp_line = realloc(line, sizeof(char) * line_size * 2);
            if (!tmp_line)
            {
                free(line);
                return NULL;
            }

            line = tmp_line;
            line_size = line_size * 2;
        }

        line[len] = c;
        len++;
    }

    if (len == 0) // len == 0 если ничего не ввели или ввели 
                  // два раза символ перевода строки
    {
        // обязательно освобождаем строку
        free(line);
        return NULL;
    }

    line[len] = '\0';
    return line;
}

int read_lines(char **lines, int size)
{
    assert(lines && size > 0);

    int curr_size = size;
    
    int nlines = 0;
    while((lines[nlines] = get_line()))
    {
        // если мы заполнили массив строк
        if (curr_size - 1 == nlines)
        {
            // то увеличиваем размер массива строк в 2 раза
            char **tmp_lines = realloc(lines, sizeof(char*) * curr_size * 2);
            if (!tmp_lines)
            {
                for (int i = 0; i < nlines; i++)
                {
                    free(lines[i]);
                }
                return 0;
            }

            lines = tmp_lines;
            curr_size *= 2;
        }

        nlines++;
    }

    return nlines;
}

int main(int argc, char *argv[])
{
    int nlines = 32;
    char **lines = calloc(sizeof(char*), nlines);
    if (!lines)
    {
        printf("[error]");
        return 0;
    }

    if (!(nlines = read_lines(lines, nlines)))
    {
        free(lines);
        return 0;
    }

    char **descriptions = find_descriptions(lines, nlines);
    if (!descriptions)
    {
        printf("[error]");
        free_nstrings(lines, nlines);
        return 0;
    }

    print_strings(descriptions);

    free_strings(descriptions);
    free_nstrings(lines, nlines);

    return 0;
}
