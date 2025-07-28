#include "compiler.h"

struct CharType CharTypes[] = {
    {"/d", "0123456789"},
    {"/h", "0123456789abcdef"},
    {"/H", "0123456789ABCDEF"},
    {"/a", "abcdefghijklmnopqrstuvwxyz"},
    {"/A", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"},
    {"/s", " "},
    {"/w", " \t\n\r\v\f"},
    {"/o", "01234567"},
    {"/p", "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"},
    {"/l", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};

char charmap[MAX_EXP_SIZE][MAX_CHARTYPE_VALUE_SIZE];

void generate_words(char charmap[][MAX_CHARTYPE_VALUE_SIZE], FILE *of, int buffer_size, char separator)
{
    int num_charmaps = 0;

    while (charmap[num_charmaps][0] != '\0')
    {
        num_charmaps++;
    }

    int charmap_sizes[num_charmaps];

    for (int i = 0; i < num_charmaps; i++)
    {
        charmap_sizes[i] = strlen(charmap[i]);
    }

    unsigned long long int total_combinations = 1;

    for (int i = 0; i < num_charmaps; i++)
    {
        total_combinations *= charmap_sizes[i];
    }

    char *buffer = malloc(buffer_size);

    if (buffer == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    memset(buffer, 0, buffer_size);

    int buffer_index = 0;

    for (int i = 0; i < total_combinations; i++)
    {
        int index = i;

        for (int j = 0; j < num_charmaps; j++)
        {
            int charmap_index = index % charmap_sizes[j];
            buffer_index += snprintf(buffer + buffer_index, buffer_size - buffer_index, "%c", charmap[j][charmap_index]);
            index /= charmap_sizes[j];
        }
        buffer_index += snprintf(buffer + buffer_index, buffer_size - buffer_index, "%c", separator);

        if (buffer_index + num_charmaps >= buffer_size)
        {
            fwrite(buffer, 1, buffer_index, of);
            buffer_index = 0;
        }
    }

    if (buffer_index > 0)
    {
        fwrite(buffer, 1, buffer_index, of);
    }

    free(buffer);

    if (of != stdout && of != stderr)
    {
        fclose(of);
    }
}

void pre_compile(char *expression)
{
    char _rep_value[32];
    char _rule[2];
    char _pre_charmap[MAX_EXP_SIZE];
    char _char_class[MAX_EXP_SIZE];
    char rule;

    short int _char_class_rep = 0;
    short int _char_class_rule = 0;
    short int _in_char_class = 0;

    memset(_rep_value, 0, sizeof(_rep_value));
    memset(_char_class, 0, sizeof(_char_class));
    memset(_pre_charmap, 0, sizeof(_pre_charmap));

    /*
     * =======================================
     * class -> expressions inside '[' and ']'
     * =======================================
     */

    for (int c = 0; c < strlen(expression); c++)
    {
        rule = expression[c];

        _rule[0] = rule;
        _rule[1] = '\0';

        /*
         * =======================================================
         * checks if it's inside curly braces in a repeating class
         * =======================================================
         * [hello]{50}
         *         ^^^
         * =======================================================
         */
        if (_char_class_rep)
        {
            if (rule == '}')
            {
                char *end_ptr;

                long int rep_value = strtol(_rep_value, &end_ptr, 10);

                if (*end_ptr != '\0')
                {
                    fprintf(stderr, "pre-compile error: '%s' is not an integer\n", _rep_value);
                    exit(EXIT_FAILURE);
                }

                if (rep_value < 1)
                {
                    fprintf(stderr, "pre-compile error: repetition value cannot be less than 1.\n");
                    exit(EXIT_FAILURE);
                }

                strcat(_pre_charmap, repstr(_char_class, rep_value));
                strcpy(_char_class, "");
                strcpy(_rep_value, "");
                _char_class_rep = 0;
            }
            else
            {
                strcat(_rep_value, _rule);
            }
        }

        /*
         * =========================================
         *  check character after class (class rule)
         *  ========================================
         *  [hello]{50}
         *         ^
         * =========================================
         */
        else if (_char_class_rule)
        {
            if (rule == '{')
            {
                _char_class_rep = 1;
                _char_class_rule = 0;
            }
        }
        /*
         * ==============================
         * checks if the class is closing
         * ==============================
         * [hello]{50}
         *       ^
         * ==============================
         */
        else if (_in_char_class && rule == ']')
        {
            _in_char_class = 0;
            _char_class_rule = 1;
        }
        /*
         * ==============================
         * checks if the class is opening
         * ==============================
         * [hello]{50}
         * ^
         * ==============================
         */
        else if (rule == '[')
        {
            _in_char_class = 1;
        }
        /*
         * ============================
         * check the value of the class
         * ============================
         * [hello]{50}
         *  ^^^^^
         * ============================
         */
        else if (_in_char_class)
        {
            strcat(_char_class, _rule);
        }
        else
        {
            strcat(_pre_charmap, _rule);
        }
    }

    strncpy(expression, _pre_charmap, MAX_EXP_SIZE);
}

char (*compile(char *expression, int a, struct CharType _CharTypes[], int num_types))[MAX_CHARTYPE_VALUE_SIZE]
{
    short int _simple_char = 0;
    short int _in_special_str = 0;
    char rule;
    int i = 0;

    pre_compile(expression);

    for (int c = 0; c < strlen(expression); c++)
    {
        rule = expression[c];

        /*
         * ===========================================================
         * checks if there is a backslash before the current character
         * ===========================================================
         * /d/d\//d/d
         *      ^
         * ===========================================================
         */
        if (_simple_char)
        {
            charmap[i++][0] = rule;
            _simple_char = 0;
        }
        /*
         * ========================================
         * checks if it is in a special string (/ *)
         * ========================================
         * hello/d
         *      ^
         * ========================================
         */
        else if (_in_special_str)
        {
            // char _special_str[MAX_CHARTYPE_VALUE_SIZE];
            char x[3];
            char *value;

            sprintf(x, "/%c", rule);
            value = get_value_by_key(_CharTypes, num_types, x);

            if (value == NULL)
            {
                value = get_value_by_key(CharTypes, NUM_TYPES, x);
                if (value == NULL)
                {
                    fprintf(stderr, "compile error: invalid special char: '%s'\n", x);
                    exit(EXIT_FAILURE);
                }
            }
            strcpy(charmap[i++], value);
            _in_special_str = 0;
        }
        else if (rule == '\\')
        {
            _simple_char = 1;
        }
        else if (rule == '/')
        {
            _in_special_str = 1;
        }
        else
        {
            charmap[i++][0] = rule;
        }
    }
    unsigned long long int combinations = 1;
    int _i = i;

    charmap[i][0] = '\0';

    if (a)
    {
        for (int c = 0; c < i; c++)
        {
            if (!_i)
                break;
            if (combinations >= 11258999068426240)
            {
                break;
            }
            combinations *= strlen(charmap[--_i]);
        }

        int gt10pb = (combinations * i) > 11258999068426240;
        char *g_bytes;

        if (gt10pb)
        {
            g_bytes = "+10PB";
        }
        else
        {
            g_bytes = convert_size((i + 1) * combinations);
        }
        char res[5];

        if (!gt10pb)
        {
            printf("(!) Number of words to be generated: %lld (%s)\n", combinations, g_bytes);
            free(g_bytes);
        }
        else
        {
            printf("(!) Number of words to be generated: +%lld (%s)\n", combinations, g_bytes);
        }
        printf("[Y/n] Continue? ");

        fgets(res, sizeof(res), stdin);

        size_t res_len = strlen(res);

        if (res_len > 0 && res[res_len - 1] == '\n')
        {
            res[res_len - 1] = '\0';
        }

        if ((strcmp(res, "n") == 0) || (strcmp(res, "N") == 0))
            exit(EXIT_SUCCESS);
    }
    return charmap;
}
