#include "main.h"

void loadFile(ListPosition where, char* filename, int rowLength)
{

	FILE* fp = NULL;
	fp = fopen(filename, "r");

	if (NULL == fp)
	{
		printf("File %s doesn't exist!", filename);
		return;
	}

	char realString[DATA_SIZE] = { 0 };

	int n = 0;
	char buffer[DATA_SIZE] = { 0 };
	char* poi = NULL;
	int cnt;

	while (fgets(buffer, DATA_SIZE, fp) != NULL)
	{
		poi = buffer;
		while (*poi)
		{

			n = 0;
			cnt = sscanf(poi, "%s %n", realString, &n);
			if (cnt >= 1)
			{
				poi += n; // no error increase pointer for amount of read characters
				// printf("%s %d\r\n", realString, n);

				NodePosition newWord = create(realString);

				int offset = getOffset(getRow(realString), getColumn(realString), rowLength);

				// printf("%s %d %d %d\r\n", realString, n, getRow(realString), getColumn(realString));

				newWord->next = where[offset].data;
				where[offset].data = newWord;
				where[offset].coef++;
			}
			else
			{
				poi++; // ERROR occured->skip one character and repeat
				perror(stderr);
				system("pause");
			}
		}
	}

	fclose(fp);
}

void writeFile(char** wordchain, char* filename)
{

	FILE* fp = NULL;
	fp = fopen(filename, "w");

	if (NULL == fp)
	{
		printf("File %s doesn't exist!", filename);
		return;
	}

	for (int i = 0; wordchain[i] != NULL; i++)
	{
		fprintf(fp, "%s\n", wordchain[i]);
	}

	fclose(fp);
}
