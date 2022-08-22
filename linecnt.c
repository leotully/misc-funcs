#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long int file_line_count(char *filename)
{
    long int i = 1;
    char ch;
    FILE *fp;
    
    fp = fopen(filename, "r");

    if (fp == NULL)
        return -1;

    /* if first character is EOF return 0, file is empty */
    if ((ch = fgetc(fp)) == EOF)
    {
        fclose(fp);
        return 0;
    }

    rewind(fp);

    while((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n')
            i++;
    }

    fclose(fp);

    return i;
}

int main(int argc, char *argv[])
{
    long int count;

    if (argc != 2)
    {
        printf("usage: %s <filename>\n", argv[0]);
        return 1;
    }

    if ((count = file_line_count(argv[1])) == -1)
    {
        printf("can't open file '%s'\n", argv[1]);
        return 1;
    }

    printf("file '%s' has %ld %s\n", argv[1], count, count==1?"line":"lines");

    return 0;
}
