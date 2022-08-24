#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **loadfile(char *filename, int buffersize, int *count);
void loadfile_free(char **array, int count);
int intlen(unsigned n);

int main(int argc, char *argv[])
{
	char **lines;
	int count;
	int int_len;
	int i;

	if (argc != 2)
	{
		printf("usage: %s <filename>\n", argv[0]);
		return 1;
	}

	if ((lines = loadfile(argv[1], 256, &count)) == NULL)
	{
		switch (count)
		{
			case -1:
				printf("can't open '%s'\n", argv[1]);
				break;
			case -2:
			case -3:
			case -4:
			case -5:
				printf("can't allocate memory\n");
				break;
			default:
				break;
		}

		return 1;
	}
	
	int_len = intlen(count);

	for (i = 0; i < count; i++)
		printf("%*d %s\n", int_len, i+1, lines[i]);

	loadfile_free(lines, count);

	return 0;
}

/* error code will be represented in count variable as follows:
 * -1 can't open file
 * -2 can't allocate initial loadfile size 100 * sizeof(char*)
 * -3 can't allocate memory for buffer
 * -4 can't reallocate loadfile size
 * -5 can't allocate memory for element
*/
char **loadfile(char *filename, int buffersize, int *count)
{
	FILE *fp;
	char **lines;
	char *s;
	char *buffer;
	int array_size = 100;
	int line_count = 0;
	
	if ((fp = fopen(filename, "r")) == NULL)
	{
		*count = -1;
		return NULL;
	}

	if ((lines = malloc(array_size * sizeof(char*))) == NULL)
	{
		*count = -2;
		return NULL;
	}

	if ((buffer = malloc(buffersize * sizeof(char))) == NULL)
	{
		*count = -3;
		return NULL;
	}

	while (fgets(buffer, buffersize, fp) != NULL)
	{
		if (line_count == array_size)
		{
			char **newptr;

			array_size += 100;
			newptr = realloc(lines, array_size * sizeof(char*));

			if (newptr == NULL)
			{
				*count = -4;
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
		
		if ((s = malloc((strlen(buffer) + 1) * sizeof(char))) == NULL)
		{
			*count = -5;
			return NULL;
		}
		
		strcpy(s, buffer);
		lines[line_count] = s;

		line_count++;
	}

	fclose(fp);

	*count = line_count;

	free(buffer);

	return lines;
}

void loadfile_free(char **array, int count)
{
	int i;

	for (i = 0; i < count; i++)
		free(array[i]);
	
	free(array);
}

int intlen(unsigned n)
{
	if (n >= 1000000000) return 10;
	if (n >= 100000000) return 9;
	if (n >= 10000000) return 8;
	if (n >= 1000000) return 7;
	if (n >= 100000) return 6;
	if (n >= 10000) return 5;
	if (n >= 1000) return 4;
	if (n >= 100) return 3;
	if (n >= 10) return 2;

	return 1;
}
