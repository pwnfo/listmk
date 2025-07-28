#include "utils.h"

void get_charmap_sizes(char **charmap, int *charmap_sizes, int num_charmaps)
{
    for (int i = 0; i < num_charmaps; i++)
    {
        charmap_sizes[i] = strlen(charmap[i]);
    }
}

char *convert_size(unsigned long long int size)
{
    char *size_units[] = {"B", "KB", "MB", "GB", "TB", "PB"};
    int i = 0;

    double value = size;

    while (value >= 1024 && i < 5)
    {
        value /= 1024;
        i++;
    }

    char *formatted_size = (char *)malloc(20); // Alocar espaço suficiente para a string

    snprintf(formatted_size, 20, "%.2f%s", value, size_units[i]);

    return formatted_size;
}

char *get_value_by_key(
    struct CharType char_types[],
    int num_types, const char *name)
{
    for (int i = 0; i < num_types; ++i)
    {
        if (strcmp(char_types[i].k, name) == 0)
        {
            return char_types[i].v;
        }
    }
    return NULL;
}

char *repstr(const char *str, int n)
{
    if (n <= 0 || str == NULL)
    {
        return NULL;
    }

    char *fstr = (char *)malloc(strlen(str) * n + 1);

    if (fstr == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++)
    {

        strcat(fstr, str);
    }

    return fstr;
}
