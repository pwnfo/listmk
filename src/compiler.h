#ifndef _LISTMK_COMPILER_H

#define _LISTMK_COMPILER_H
#define NUM_TYPES 10
#define MAX_EXP_SIZE 200

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char (*compile(char *expression, int a, struct CharType _CharTypes[], int num_types))[MAX_CHARTYPE_VALUE_SIZE];
void generate_words(char charmap[][MAX_CHARTYPE_VALUE_SIZE], FILE *of, int buffer_size, char separator);

#endif
