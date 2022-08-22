/*******************************************************************************
usage: retab <input file> [spaces]

if spaces is omitted 4 spaces will be assumed.
output is to stdout (screen). to output to a file it must be redirected:
  retab file.c > newfile.c

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

void showhelp(void);
char **loadfile(char *filename, unsigned int *count);
int count_leading_spaces(char *str);

int tab_size = 4;

int main(int argc, char *argv[])
{
    char in_filename[1025];
    char temp_str[BUFFER_SIZE];
    char **lines;
    unsigned int line_count;
    int spaces;
    int tabs_needed = 0;
    unsigned int i, j;
        
    if (argc < 2)
        showhelp();
    else
        strcpy(in_filename, argv[1]);

    if (argc == 3)
    {
        tab_size = atoi(argv[2]);

        if (tab_size == 0)
        {
            printf("space must be 1 or more\n");
            exit(1);
        }
    }    

    if ((lines = loadfile(in_filename, &line_count)) == NULL)
        exit(1);

    for (i = 0; i < line_count; i++)
    {
        memset(temp_str, '\0', strlen(lines[i]) + 1);

        spaces = count_leading_spaces(lines[i]);
        tabs_needed = spaces / tab_size;

        if (tabs_needed > 0)
        {
            strcpy(temp_str, "\t");
            
            for (j = 1; j < tabs_needed; j++)
                strcat(temp_str, "\t");

        }

        strcat(temp_str, lines[i] + spaces);
        strcpy(lines[i], temp_str);
        printf("%s\n", lines[i]);
    }

    for (i = 0; i < line_count; i++)
        free(lines[i]);
    
    free(lines);

    return 0;
}

void showhelp(void)
{
    printf("usage: retab <input file> [spaces]\n\n");
    printf("if spaces is omitted 4 spaces will be assumed.\n\n");
    printf("output is to stdout (screen). to output to a file it must be "
                                                            "redirected:\n");
    printf("  retab file.c > newfile.c\n");
    exit(1);
}

int count_leading_spaces(char *str)
{
    int result = 0;

    while(isspace((unsigned char)*str))
    {
        str++;
        result++;
    }

    return result;
}

char **loadfile(char *filename, unsigned int *count)
{
    FILE *fp;
    char **lines;
    char *s;
    char buffer[BUFFER_SIZE];
    int array_size = 100;
    int line_count = 0;

    if ((fp = fopen(filename, "r")) == NULL) 
    {
        fprintf(stderr, "can't open file '%s'\n", filename);
        fprintf(stderr, "file: %s\nline: %d\n", __FILE__, __LINE__);
        
        return NULL;
    }
    
    if ((lines = malloc(array_size * sizeof(char*))) == NULL)
    {
        fprintf(stderr, "can't allocate memory\n");
        fprintf(stderr, "file: %s\nline: %d\n", __FILE__, __LINE__);

        return NULL;
    }
    
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL)
    {
        if (line_count == array_size)
        {
            char **newptr;
            
            array_size += 100;            
            newptr = realloc(lines, array_size * sizeof(char*));

            if (newptr == NULL)
            {
                fprintf(stderr, "can't allocate memory\n");
                fprintf(stderr, "file: %s\nline: %d\n", __FILE__, __LINE__);

                return NULL;
            }

            lines = newptr;
        }
        
        if(strchr(buffer, '\n') == NULL)
        {
            char discard;
            while((discard = fgetc(fp)) != '\n' && discard != EOF);
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        s = malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(s, buffer);
        lines[line_count] = s;

        line_count++;
    }

    fclose(fp);

    *count = line_count;

    return lines;
}
