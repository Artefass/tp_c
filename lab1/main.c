
#include <stdio.h>

int main(int argc, char *argv[])
{
    vector *lines = create_vector(sizeof(char*), 32);
    if (!lines)
    {
        printf("ERROR: main. Could not allocate memmory for lines\n");
        return 0;
    }

    int nlines = 0;
    if (!(nlines = read_lines(lines)))
    {
        printf("ERROR: main. Empty input\n");
        free_vector(lines);
    }

    vector *descriptions = find_descriptions(lines, nlines);
    if (!letter)
    {
        printf("ERROR: main. Letter have NULL pointer\n");
        free_vector(lines);
        return 0;
    }

    print_lines(descriptions);

    free_vector(descriptions);
    free_vector(lines);

    return 0;
}