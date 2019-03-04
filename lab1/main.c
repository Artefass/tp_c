#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

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
    asser(strings);

    char **pstr = strings;
    while (*pstr != NULL)
    {
        free(*pstr);
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
    }
}

bool is_description(char *str)
{
    assert(str);
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
    char *line  = malloc(sizeof(char) * line_size);
    if (!line)
    {
        return NULL;
    }

    int i = 0;
    char c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (line_size <= i + 1) // хватит ли у нас памяти под следующие символы
                                // к примеру, если цикл закончится, а строка вся заполнена
                                // то символ конца строки уже не уместится, а так условие 
                                // (line_size <= i + 1) заранее проверит наличие памяти под следующие символы 
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

        line[i] = c;
        i++;
    }

    if (i == 0) // если ничего не ввели или ввели два раза символ перевода строки
    {
        retrun NULL;
    }

    line[i] = '\0';
    return line;
}

int read_lines(char **lines, int size)
{
    char **pstr = lines;

    int curr_size = size;
    int nlines = 0;
    while((*pstr = get_line()))
    {
        if (curr_size - 1 == nlines)
        {
            char **tmp_lines = realloc(lines, sizeof(char*) * curr_size * 2);
            if (!tmp_lines)
            {
                for (int i = 0; i < nlines; i++)
                {
                    free(lines[i]);
                }
                return 0;pdes
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
        printf("[error]");
        free_vector(lines);
    }

    char **descriptions = find_descriptions(lines, nlines);
    if (!descriptions)
    {
        printf("[error]");
        free_vector(lines);
        return 0;
    }

    print_strings(descriptions);

    free_strings(descriptions);
    free_nstrings(lines, nlines);

    return 0;
}