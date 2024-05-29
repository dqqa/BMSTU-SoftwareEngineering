#include <assert.h>
#include <stdio.h>
#include <string.h>

#define ERR_OK 0
#define ERR_PARAM 1
#define ERR_ZERO_LEN 2
#define ERR_BUF_OVERFLOW 3
#define ERR_IO 4

#define MAX_WORD_LEN 10
#define MAX_ARR_SIZE 50
#define MAX_STR_LEN 1000

void copy_word(char *dst, const char *src, size_t size);

int extract_words(char *sentence, char (*words)[MAX_WORD_LEN], size_t *size)
{
    assert(sentence != NULL);
    assert(words != NULL);

    if (!sentence || !words)
        return ERR_PARAM;

    *size = 0;

    char *ptr = strtok(sentence, " ");
    while (ptr)
    {
        (*size)++;
        if (*size > MAX_ARR_SIZE)
            return ERR_BUF_OVERFLOW;

        size_t word_size = strlen(ptr);
        if (word_size > MAX_WORD_LEN)
            return ERR_BUF_OVERFLOW;

        copy_word(*(words++), ptr, word_size + 1);
        ptr = strtok(NULL, " ");
    }
    return ERR_OK;
}

void copy_word(char *dst, const char *src, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        *dst = *src;
        dst++;
        src++;
    }
}

int input_str(char *sentence, size_t size)
{
    if (!fgets(sentence, size, stdin))
        return ERR_IO;

    char *newline = strchr(sentence, '\n');
    if (newline)
        *newline = 0;
    else
        return ERR_BUF_OVERFLOW;

    return ERR_OK;
}

void print_err(int err)
{
    switch (err)
    {
        case ERR_IO:
            printf("IO error");
            break;
        case ERR_BUF_OVERFLOW:
            printf("Buffer overflow error");
            break;
        case ERR_PARAM:
            printf("Parameter error");
            break;
        case ERR_ZERO_LEN:
            printf("Zero length error");
            break;
    }
    printf("\n");
}

// Не успел :(
int pos_extract_words(void)
{
    int failed_tests = 0;
    {
        char *sentence = "abc def ghi";
        char should[MAX_ARR_SIZE][MAX_WORD_LEN] = {"abc", "def", "ghi"};
        char cur[MAX_ARR_SIZE][MAX_WORD_LEN];

        size_t size;
        int rc = extract_words(sentence, cur, &size);
        if (size != 3 || rc != ERR_OK)
            failed_tests++;
    }
    {
        char *sentence = "abc";
        char should[MAX_ARR_SIZE][MAX_WORD_LEN] = {"abc"};
        char cur[MAX_ARR_SIZE][MAX_WORD_LEN];

        size_t size;
        int rc = extract_words(sentence, cur, &size);
        if (size != 3 || rc != ERR_OK)
            failed_tests++;
    }

    return failed_tests;
}

int neg_extract_words(void)
{
    int failed_tests = 0;
    {
        char *sentence = NULL;
        char cur[MAX_ARR_SIZE][MAX_WORD_LEN];
        size_t size;
        int rc = extract_words(sentence, cur, &size);
        if (rc != ERR_PARAM)
            failed_tests++;
    }        
    return failed_tests;
}

int main(void)
{
    char sentence[MAX_STR_LEN];
    char words[MAX_ARR_SIZE][MAX_WORD_LEN];
    size_t words_len;
    int rc = input_str(sentence, sizeof(sentence));
    if (rc != ERR_OK)
    {
        print_err(rc);
        return rc;
    }

    rc = extract_words(sentence, words, &words_len);
    if (rc != ERR_OK)
    {
        print_err(rc);
        return rc;
    }

    printf("Result:\n");
    for (size_t i = 0; i < words_len; i++)
        printf("{\"%s\"}, ", words[i]);
    printf("\n");

    return ERR_OK;
}
