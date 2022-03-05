#include "main.h"

int getOffsetWord(char* word, int rowLength) {
	return getOffset(getRow(word), getColumn(word), rowLength);
}


int getOffsetRow(int offset, int rowLength)
{
	return offset / rowLength;
}
int getOffsetColumn(int offset, int rowLength)
{
	return offset % rowLength;
}

int invertOffset(int offset, int rowLength)
{
	return getOffset(offset % rowLength, offset / rowLength, rowLength);
}

int findMax(ListPosition where, int rowLength)
{
	int max = 0;
	int offset = -1;
	for (int i = 0; i < rowLength * rowLength; i++)
		if (max < where[i].coef)
		{
			max = where[i].coef;
			offset = i;
		}

	return offset;
}
int findMaxRow(ListPosition where, int row, int rowLength)
{
	if(row<0)
		return -1;

	int max = 0;
	int offset = -1;
	int tempOffest;
	for (int i = 0; i < rowLength; i++)
	{
		tempOffest = getOffset(row, i, rowLength);
		if (max < where[tempOffest].coef)
		{
			max = where[tempOffest].coef;
			offset = tempOffest;
		}
	}
	return offset;
}
int findMaxColumn(ListPosition where, int column, int rowLength)
{
	if (column < 0)
		return -1;

	int max = 0;
	int offset = -1;
	int tempOffest;
	for (int i = 0; i < rowLength; i++)
	{
		tempOffest = getOffset(i, column, rowLength);
		if (max < where[tempOffest].coef)
		{
			max = where[tempOffest].coef;
			offset = tempOffest;
		}
	}
	return offset;
}

NodePosition create(char word[])
{
	NodePosition el = NULL;
	el = (NodePosition)malloc(sizeof(Node));

	if (NULL == el)
	{
		printf("Memory allocation failed!\r\n");
		return NULL;
	}

	strcpy(el->word, word);
	el->next = NULL;

	return el;
}

char* pop(ListPosition roots, int offset)
{
	if (roots[offset].data != NULL)
	{
		NodePosition temp = roots[offset].data;
		roots[offset].data = roots[offset].data->next;

		char* word = (char*)malloc((strlen(temp->word) + 1) * sizeof(char));
		strcpy(word, temp->word);

		free(temp);
		roots[offset].coef--;

		return word;
	}

	return NULL;
}

int getOffset(int row, int column, int len)
{
	return row * len + column;
}

int getRow(char* c)
{
	return c!=NULL?charToIndex(c[0]) * 27 + charToIndex(c[1]):-1;
}
int getColumn(char* c)
{
	return c != NULL ? charToIndex(c[strlen(c) - 2]) * 27 + charToIndex(c[strlen(c) - 1]) : -1;
}

int charToIndex(char c)
{
	if (97 <= c && c < 123)
		return (c - 96);

	return 0;
}


char* getCharset()
{

	char* letters = NULL;
	letters = (char*)malloc(28 * sizeof(char));

	if (NULL == letters)
	{
		printf("Memory allocation failed!\r\n");
		return NULL;
	}

	letters[0] = '-';

	int i, j;
	for (i = 1, j = 97; j < 123; j++, i++)
		letters[i] = j;

	letters[i] = 0;

	return letters;
}
