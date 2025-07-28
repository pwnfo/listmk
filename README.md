# ListMK - A simple and effective wordlist creator.

## DESCRIPTION

ListMK is a versatile wordlist generator designed for simplicity and efficiency. It provides a range of options to customize wordlist generation, allowing users to tailor the output to their specific needs.

## COMMON OPTIONS

- **-h**: Display help information and exit.
- **-v**: Output version information and exit.
- **-y**: Do not prompt for confirmation and suppress additional information.
- **-o FILE**: Write the generated wordlist to the specified FILE.

## GENERATOR OPTIONS

- **-e EXPR**: Specify the expression to generate words.
- **-s SEP**: Set the separator for generated words (default is `\n`).
- **-b SIZE**: Set the buffer size for efficient wordlist creation (default is 1024).
- **-S CHAR=SET**: Create a custom special character set (e.g., `-S 'x=abc'`).

## SPECIAL CHARACTERS

- **/d**: 0123456789
- **/h**: /d + abcdef
- **/H**: /d + ABCDEF
- **/a**: abcdefghijklmnopqrstuvwxyz
- **/A**: ABCDEFGHIJKLMNOPQRSTUVWXYZ
- **/s**: \<space\>
- **/w**: /s + \\t\\n\\r\\v\\f
- **/o**: 01234567
- **/p**: !"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~
- **/l**: /a + /A

## EXAMPLES

1. Basic usage:

    ```bash
    $ listmk -e '/a/d' -o output.txt
    ```

2. Generate words with custom separator:

    ```bash
    $ listmk -e '/a/A/d' -s ',' -o output.txt
    ```

3. Create a wordlist with a custom special character set:

    ```bash
    $ listmk -S 'x=!?#' -e '/a/x' -o output.txt
    ```

4. Create a wordlist with repetitions of expressions:
    ```bash
    $ listmk -e '/p[/a]{4}' -o output.txt
    ```

## AUTHOR

ListMK was developed by [Ryan R.](https://github.com/pwnfo).
