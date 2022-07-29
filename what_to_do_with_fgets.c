#include <stdio.h>
#include <string.h>

/* buffer length small to show effects truncation                             */
#define BUFFER_LEN    31
#define FILENAME_1    "what_to_do_with_fgets.c"

int main(int argc, char *argv[])
{
	FILE *fp;
	char filename[4096] = FILENAME_1;
	char buffer[BUFFER_LEN];

	printf("Enter some text and press [Enter]:\n> ");
	fgets(buffer, BUFFER_LEN, stdin);

	/* for input from stdin discard the overflow up to newline so the next    */
	/* call will be clean.                                                    */
	if (strchr(buffer, '\n') == NULL)
	    while((fgetc(stdin)) != '\n');

	/* then strip the newline character if the string is to be processed      */
	buffer[strcspn(buffer, "\n")] = '\0';

	printf("\nYou entered:\n> %s\n", buffer);

	printf("\nPress [Enter] to continue... ");
	fgetc(stdin);
	
	/* for input from a file also check for end of file or it will go into    */
	/* an infinite loop                                                       */
	if (argc == 2)
		strcpy(filename, argv[1]);
	
	fp = fopen(filename, "r");

	if (fp == NULL)
	{
		printf("Can't open '%s'\n", FILENAME_1);
		printf("Please provide filename on command line.");

		return 1;
	}

	while (fgets(buffer, BUFFER_LEN, fp) != NULL)
	{
		if (strchr(buffer, '\n') == NULL)
		{
		    char discard;
		    while((discard = fgetc(fp)) != '\n' && discard != EOF);
		}

		buffer[strcspn(buffer, "\n")] = '\0';

		printf("%s\n", buffer);
	}
	
	fclose(fp);

	/* Now buffer contains exactly what one would expect.                     */
	return 0;
}