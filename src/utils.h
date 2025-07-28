#ifndef _LISTMK_UTILS_H

#define _LISTMK_UTILS_H
#define MAX_CHARTYPE_VALUE_SIZE 53

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

struct CharType
{
    char k[3];
    char v[MAX_CHARTYPE_VALUE_SIZE];
};

char *get_value_by_key(struct CharType char_types[], int num_types, const char *name);
char *repstr(const char *str, int n);
char *convert_size(unsigned long long int size);

#endif