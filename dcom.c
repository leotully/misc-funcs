#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void showhelp(void);
long get_file_size(char *filename);
char *dcom(char *orig_str);

int main(int argc, char *argv[])
{
	FILE *fp;
	char in_filename[1025];
	char *orig_str;
	char *new_str;
	long file_size;
	int i;

	if (argc != 2)
		showhelp();
	else
		strcpy(in_filename, argv[1]);
	
	file_size = get_file_size(in_filename);
	
	if ((fp = fopen(argv[1], "r")) == NULL)
	{
		printf("can't open %s\n", in_filename);
		exit(1);
	}

	if ((orig_str = malloc(file_size + 1)) == NULL)
		{
		printf("can't allocate\n");
		exit(1);
	}

	for (i = 0; i < file_size; i++)
		orig_str[i] = fgetc(fp);

	orig_str[i] = '\0';

	fclose(fp);

	new_str = dcom(orig_str);
	free(orig_str);

	printf("%s", new_str);
	free(new_str);

	return 0;
}

void showhelp(void)
{
	printf("usage: dcom <filename>\n\n");
	printf("output is to stdout (screen). to output to a file it must be redirected:\n");
	printf("  dcom file.c > newfile.c\n");
	exit(1);
}

long get_file_size(char *filename)
{
    long i;

    FILE *fp = fopen(filename, "r");
	
	if (fp == NULL)
		return -1;

    fseek(fp, 0L, SEEK_END);
    i = ftell(fp);
    rewind(fp);

    fclose(fp);

    return i;
}

char *dcom(char *orig_str)
{
	size_t i;
	size_t str_len;
	size_t new_len = 0;
	char *new_str;
	int single_line = 0;
	int multi_line = 0;

	str_len = strlen(orig_str);
	new_str = malloc(str_len + 1);
	
	if (new_str == NULL)
	{
		printf("new_str allocation problem\n");
		exit(1);
	}

	memset(new_str, ' ', str_len);

	for (i = 0; i < str_len; i++)
	{
		/* end of single line comment */
		if (single_line == 1 && orig_str[i] == '\n')
		{			
			single_line = 0;
			i--;
		}
		/* end of multi line comment */
		else if (multi_line == 1 && orig_str[i] == '*' && orig_str[i+1] == '/')
		{
			multi_line = 0;
			i++;
		}
		/* step over comment characters */
		else if (single_line || multi_line)
		{
			continue;
		}
		/* start of single line comment */
		else if (orig_str[i] == '/' && orig_str[i+1] == '/')
		{
			single_line = 1;
			i++;
		}
		/* start of multi line comment */
		else if (orig_str[i] == '/' && orig_str[i+1] == '*')
		{
			multi_line = 1;
			i++;
		}
		else
		{
			new_str[new_len] = orig_str[i];
			new_len++;
		}
	}
	
	new_str[new_len] = '\0';

	return new_str;
}