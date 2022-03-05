#pragma once
struct _node;
typedef struct _node* NodePosition;
typedef struct _node
{
	char word[DATA_SIZE];
	NodePosition next;
} Node;

struct _list;
typedef struct _list* ListPosition;
typedef struct _list
{
	NodePosition data;
	int coef;
} List;



char* getCharset();

NodePosition create(char[]);
char* pop(ListPosition, int);






int charToIndex(char);
int getRow(char*);
int getColumn(char*);

int getOffsetWord(char*, int);
int getOffset(int, int, int);
int invertOffset(int, int);
int getOffsetRow(int, int);
int getOffsetColumn(int, int);

int findMax(ListPosition, int);
int findMaxRow(ListPosition, int, int);
int findMaxColumn(ListPosition, int, int);