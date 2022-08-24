/*******************************************************************************
usage: dtab <input file> [tabsize]

if tabsize is omitted 4 spaces will be used.
output is to stdout (screen). to output to a file it must be redirected:
  dtab file.c > newfile.c
  
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void showhelp(void);
long get_file_size(char *filename);

int main(int argc, char *argv[])
{
	FILE *infile;
	char in_filename[1025];
	int tab_size = 4;
	long file_size;
	int i, j;
	char ch;

	if (argc < 2)
		showhelp();
	else
		strcpy(in_filename, argv[1]);
	
	if (argc == 3)
		tab_size = atoi(argv[2]);

	file_size = get_file_size(in_filename);

	if ((infile = fopen(in_filename, "r")) == NULL)
	{
		printf("can't open '%s'\n", argv[1]);
		exit(1);
	}

	for (i = 0; i < file_size; i++)
	{
		ch = fgetc(infile);

		if (ch == '\t')
		{
			for (j = 0; j < tab_size; j++)
				fputc(' ', stdout);
		}
		else
		{
			fputc(ch, stdout);
		}
	}

	fclose(infile);

	return 0;
}

void showhelp(void)
{
	printf("usage: dtab <input file> [tabsize]\n\n");
	printf("if tabsize is omitted 4 spaces will be used.\n\n");
	printf("output is to stdout (screen). to output to a file it must be redirected:\n");
	printf("  dtab file.c > newfile.c\n");
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
