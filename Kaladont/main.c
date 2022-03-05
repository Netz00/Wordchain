#include "main.h"


int main(void)
{

	// ----------- initialize allowed charset -----------

	char* letters = getCharset();

	printf("Charset length: %d\r\n", strlen(letters));
	int length = strlen(letters);

	for (int i = 0; i < length; i++)
		printf("%c -> %d -> %d\r\n", *(letters + i), *(letters + i), charToIndex(*(letters + i)));

	printf("Allowed letters: %s\r\n\n", letters);

	// ----------- initialize data structure -----------

	int rowLength = length * length;

	printf("Matrix: %d x %d \r\n\n\r", rowLength, rowLength);

	ListPosition roots = NULL;
	roots = (ListPosition)malloc(rowLength * rowLength * sizeof(List));

	for (int i = 0; i < rowLength; i++)
		for (int j = 0; j < rowLength; j++)
		{
			int offset = getOffset(i, j, rowLength);
			roots[offset].data = NULL;
			roots[offset].coef = 0;
		}

	char** wordchain = (char**)malloc(rowLength * rowLength * sizeof(char*));
	for (int i = 0, max = rowLength * rowLength; i < max; i++)
		wordchain[i] = NULL;


	//----------- load data -----------


	int undoCountInit = 4;
	int undoCounterTemp;
	
	int undoCountBest = 0;
	int counterMax = 0;
	

	 //printf("%20s - [ %5d ][ %5d ]\n\r","lt-ov", getRow("lt-ov"), getColumn("lt-ov"));
	 //printf("%20s - [ %5d ][ %5d ]\n\r", "jezikoslovno", getRow("jezikoslovno"), getColumn("jezikoslovno"));

	 //for (undoCountInit = 0; undoCountInit < 15; undoCountInit++)
	{
		undoCounterTemp = undoCountInit;

		loadFile(roots, FILE_NAME, rowLength);

		// ----------- search algorithm -----------

		// ----------- dummy diagonal search -----------
		/*
		int counter = 0;
		NodePosition temp = NULL;
		for (int i = 0; i < rowLength; i++) {
			int offset = getOffset(i, i, rowLength);
			if (roots[offset].data != NULL && roots[offset].data->next != NULL) {
				printf("Diagonal [ %d ][ %d ] Words: %d\n\r", getOffsetRow(offset, rowLength), getOffsetColumn(offset, rowLength), roots[offset].coef);
				temp = roots[offset].data;
				while (temp != NULL) {
					printf("%s\n\r", temp->word );
					counter++;
					temp = temp->next;
				}
				printf("Wordchain: %d\n\r\n\r", counter);
				counter = 0;
			}
		}
		*/
		// ----------- max max algorithm search -----------

		int max = findMax(roots, rowLength);
		int max_column;
		int counter = 0;
		int calculating = 1;

		int max2;

		if (max > 0)
		{

			wordchain[counter] = pop(roots, max);

			// cell is empty
			if (wordchain[counter] == NULL)
			{
				printf("ERROR   %d    %d   %s\n\r", max, roots[max].coef, roots[max].data->word);
				return;
			}

			// printf("%20s - x%4d - [ %5d ]\n\r", roots[max].data->word, roots[max].coef, max);

			counter++;

			while (calculating)
			{

				// get column of MAX
				max_column = getOffsetColumn(max, rowLength);
				max2 = findMaxRow(roots, max_column, rowLength);

				if (max2 < 0)
				{
					undoCounterTemp--;
					if (undoCounterTemp < 0)
						break;


					// clear column
					int tempOffest;
					for (int i = 0; i < rowLength; i++)
					{
						tempOffest = getOffset(i, max_column, rowLength);
						roots[tempOffest].coef = 0;
					}

					
					// remove current(last word) in chain!!!
					counter--;
					free(wordchain[counter]);
					wordchain[counter] = NULL;
					
					// set counter to previous word!!!
					counter--;


					// start from begining again
					if (counter < 0) {
						max = findMax(roots, rowLength);
						counter = 0;
						break;// if this occurs, make deeper inspection
					}

					// find offset of the word before
					max = getOffsetWord(wordchain[counter], rowLength);

					//if (counter > 140 && counter < 150)
					//printf("STEPBACK to WORD: %s counter: %d\n\r", wordchain[counter],counter);

					// increment counter to set not overwrite current word!!!
					counter++;
				}
				else
				{

					undoCounterTemp = undoCountInit;

					// if cell is empty
					if ((wordchain[counter] = pop(roots, max2)) == NULL) {
						// printf("ERROR   %d    %d   %s   c\n\r", max2, roots[max2].coef, wordchain[counter]);
						break;
					}

					//if (counter > 140 && counter<150)
					//printf("%20s - [ %5d ][ %5d ] counter: %d\n\r", wordchain[counter], getRow(wordchain[counter]), getColumn(wordchain[counter]), counter);



					// printf("%20s - x%4d - [ %5d ]\n\r", wordchain[counter], roots[max2].coef, max2);

					counter++;
					max = max2;
				}
			}
		}

		// clean the rest of chain if not NULL
		for (int i = counter, max = rowLength * rowLength; i < max; i++)
			if (wordchain[i] != NULL)
			{
				free(wordchain[i]);
				wordchain[i] = NULL;
			}


		printf("WORD CHAIN: %d  UNDOCOUNTER: %d \n\r", counter, undoCountInit);

		
		if (counter > counterMax)
		{
			counterMax = counter;
			undoCountBest = undoCountInit;
		}
		


		// Write wordchain to disk
		char filename[20];
		snprintf(filename, undoCountInit > 9 ? 19 : 18, "%s_%d.txt", OUTPUT_FILE_NAME, undoCountInit);
		writeFile(wordchain, filename);



		// clean the remaining words from roots before re-adding words from file
		for (int offset = 0, max = rowLength * rowLength; offset < max; offset++) 
		{
			while (roots[offset].data != NULL)
			{
				NodePosition temp = roots[offset].data;
				roots[offset].data = roots[offset].data->next;
				roots[offset].coef--;
				free(temp);
			}
			roots[offset].coef = 0;
		}

		// re-initialize word chain
		for (int i = 0, max = rowLength * rowLength; i < max; i++)
			if (wordchain[i] != NULL)
			{
				free(wordchain[i]);
				wordchain[i] = NULL;
			}



	}

	printf("MAX WORD CHAIN: %d  MAX UNDOCOUNTER: %d \n\r", counterMax, undoCountBest);















	// ----------- cleaning memory -----------


	// clean the wordchain
	for (int i = 0, max = rowLength * rowLength; i < max; i++)
		if (wordchain[i] != NULL) {
			free(wordchain[i]);
			wordchain[i] = NULL;
		}


	// clean the datastructure
	for (int offset = 0, max = rowLength * rowLength; offset < max; offset++) {
		while (roots[offset].data != NULL)
		{
			NodePosition temp = roots[offset].data;
			roots[offset].data = roots[offset].data->next;
			free(temp);
		}
	}

	// clean the wordchain array
	free(wordchain);
	// clean the datastructure array
	free(roots);
	// clean the letters
	free(letters);

	return EXIT_SUCCESS;
}
