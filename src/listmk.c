#include "listmk.h"

struct CharType get_ct(char *exp)
{
    struct CharType _CharType;
    char key[3];
    char value[MAX_CHARTYPE_VALUE_SIZE];
    int _value = 0;

    char r;
    char _r[2];

    size_t exp_len = strlen(exp);

    memset(key, 0, sizeof(key));
    memset(value, 0, sizeof(key));

    for (int i = 0; i < exp_len; i++)
    {
        r = exp[i];
        _r[0] = r;
        _r[1] = '\0';

        if (_value)
        {
            strcat(value, _r);
        }
        else if (r == '=')
        {
            _value = 1;
        }
        else
        {
            sprintf(key, "/%s", _r);
        }
    }

    strcpy(_CharType.k, key);
    strcpy(_CharType.v, value);

    return _CharType;
}

void print_help(char *prog)
{
    printf("usage: %s [options]...\n\n"
           "A simple and effective wordlist creator.\n\n"
           "common options:\n"
           "  -h            display this help and exit\n"
           "  -v            output version information and exit\n"
           "  -y            do not ask if you want to continue and\n"
           "                do not show additional information\n"
           "  -o +          write the wordlist in the file\n\n"
           "generator options:\n"
           "  -e +          expression to generate words\n"
           "  -s +          generated words separator (default is \\n)\n"
           "  -b +          set the buffer size (default is 1024)\n"
           "  -S +          create a custom special char\n"
           "                (e.g. -S 'x=abc')\n\n"
           "=========== SPECIAL CHARS ===========\n"
           "/d = 0123456789\n"
           "/h = /d + abcdef\n"
           "/H = /d + ABCDEF\n"
           "/a = abcdefghijklmnopqrstuvwxyz\n"
           "/A = ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
           "/s = <space>\n"
           "/w = /s + \\t\\n\\r\\v\\f\n"
           "/o = 01234567\n"
           "/p = !\"#$%%&'()*+,-./:;<=>?@[\\]^_`{|}~\n"
           "/l = /a + /A\n"
           "=====================================\n\n"
           "Created by Ryan R.\n",
           prog);
}

int main(int argc, char *argv[])
{
    int c;
    char *endptr;
    FILE *output_file;
    time_t start_time;
    time_t end_time;
    struct
    {
        struct CharType CharTypes[52];
        char output_file[MAX_PATH_SIZE];
        char expression[MAX_EXP_SIZE];
        char expression_file[MAX_PATH_SIZE];
        char separator;
        int buffer;
        int a;
        int i;
    } options = {.buffer = 1024, .a = 1, .separator = '\n', .i = 0};
    const char *shortopts = "hvo:e:s:b:yS:";

    while ((c = getopt(argc, argv, shortopts)) != -1)
    {
        switch (c)
        {
        case 'h':
            print_help(argv[0]);
            return EXIT_SUCCESS;
        case 'v':
            printf("ListMK v%s\n", LISTMK_VERSION);
            return EXIT_SUCCESS;
        case 'o':
            snprintf(options.output_file, sizeof(options.output_file), optarg);
            break;
        case 'e':
            snprintf(options.expression, sizeof(options.expression), optarg);
            break;
        case 's':
            options.separator = optarg[0];
            break;
        case 'b':
            options.buffer = strtol(optarg, &endptr, 10);

            if (*endptr != '\0' || options.buffer < 1)
            {
                fprintf(stderr, "%s: invalid buffer size: %s\n", argv[0], optarg);
                return EXIT_FAILURE;
            }

            break;
        case 'y':
            options.a = 0;
            break;
        case 'S':
            options.CharTypes[options.i++] = get_ct(optarg);
            break;
        case '?':
            fprintf(stderr, "Try '%s -h' for more information.\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (!(strlen(options.expression)) && !(strlen(options.expression_file)))
    {
        print_help(argv[0]);
        return EXIT_FAILURE;
    }

    char(*charmap)[MAX_CHARTYPE_VALUE_SIZE] = compile(options.expression, options.a, options.CharTypes, options.i);

    if (!strlen(options.output_file))
    {
        output_file = stdout;
    }
    else
    {

        output_file = fopen(options.output_file, "w");

        if (output_file == NULL)
        {
            perror("fopen");
            return EXIT_FAILURE;
        }
    }
    if (options.a)
    {
        time(&start_time);

        printf("(!) ListMK v%s started at %s", LISTMK_VERSION, ctime(&start_time));
    }

    generate_words(charmap, output_file, options.buffer, options.separator);

    if (options.a)
    {
        time(&end_time);

        double elapsed_time = difftime(end_time, start_time) / 60.0;

        printf("(!) Wordlist generated in %.2f min.\n", elapsed_time);
    }

    return 0;
}
