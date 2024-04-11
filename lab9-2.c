#include <stdio.h>
#include <stdlib.h>
#define HASH_TABLE_SIZE 23

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
// Hash table-like structure?
struct HashType
{
	struct RecordNode** lists;
	int size;
};

// Extra helper struct (needed to use seperate chaining)
struct RecordNode
{
	struct RecordType record;
	struct RecordNode* next;
};

void freeList (struct RecordNode* head) {
	if (head == NULL) {
		return;
	}

	freeList(head->next);
	free(head);
}

// Compute the hash function
int hash(int x)
{
	// Multiplicative hash function.
	return ((double) ((7417 * (long) x) % 2131)) / ((double) (2131 / HASH_TABLE_SIZE));
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	
	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		if (pHashArray->lists[i] != NULL) {
			printf("Index %d", i);
			struct RecordNode* currentList = pHashArray->lists[i];
			while (currentList != NULL) {
				printf(" -> %d, %c, %d", currentList->record.id, currentList->record.name, currentList->record.order);
				currentList = currentList->next;
			}
			printf("\n");
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	struct HashType* pHashArray = malloc(sizeof(struct HashType));

	if (pHashArray == NULL) {
		return -1;
	}

	pHashArray->size = HASH_TABLE_SIZE;

	struct RecordNode** lists = malloc(sizeof(struct RecordNode*) * HASH_TABLE_SIZE);

	if (lists == NULL) {
		free(pHashArray);
		return -1;
	}

	pHashArray->lists = lists;

	for (int c = 0; c < pHashArray->size; c++) {
		pHashArray->lists[c] = NULL;
	}

	for (int c = 0; c < recordSz; c++) {
		struct RecordNode* newNode = malloc(sizeof(struct RecordNode));
		newNode->record.id = pRecords[c].id;
		newNode->record.name = pRecords[c].name;
		newNode->record.order = pRecords[c].order;
		newNode->next = NULL;

		struct RecordNode* targetNode = pHashArray->lists[hash(pRecords[c].id)];
		if (targetNode == NULL) {
			pHashArray->lists[hash(pRecords[c].id)] = newNode;
		} else {
			while (targetNode->next != NULL) {
				targetNode = targetNode->next;
			}

			targetNode->next = newNode;
		}
	}

	free(pRecords);

	displayRecordsInHash(pHashArray, pHashArray->size);

	for (int c = 0; c < pHashArray->size; c++) {
		freeList(pHashArray->lists[c]);
	}

	free(pHashArray);

}