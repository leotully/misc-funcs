#include <stdio.h>

/* headers needed for trimwhitespace function */
#include <string.h>
#include <ctype.h>

typedef enum { ALL, LEFT, RIGHT } trim_type;

char *trimwhitespace(char *str, trim_type tt)
{
	char *end;

	/* test for empty string */
	if(*str == '\0')
		return str;

	/* trim left */
	if (tt == ALL || tt == LEFT)
	{
		while(isspace((unsigned char)*str))
			str++;
	}
	
	if (tt == LEFT)
		return str;
	
	/* trim right */
	end = str + strlen(str) - 1;

	while (end > str && isspace((unsigned char)*end))
		end--;
	
	end[1] = '\0';
	
	return str;
}

int main(void)
{
	char str[] = "  Hello, World  ";
	
	printf("raw string: [%s]\n", str);
	printf("trim LEFT:  [%s]\n", trimwhitespace(str, LEFT));
	printf("raw string: [%s]\n", str);
	printf("trim ALL:   [%s]\n", trimwhitespace(str, ALL));
	printf("raw string: [%s]\n", str);
	
	/* after trimming white space from right side (with trim_type ALL or
	 * RIGHT) the string is permanently modified.
	*/
	
	return 0;
}

