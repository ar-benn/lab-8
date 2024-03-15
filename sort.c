#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

int merge(int *pData, int left, int midVal, int right) 
{
	//setup vars
	int i, j ,k;

	//set up lengths
	int arrLen1 = midVal - left+1;
	int arrLen2 = right - midVal;

	//set up temp arrays
	int *arrLeft = Alloc(arrLen1*sizeof(int));
  	int *arrRight = Alloc(arrLen2*sizeof(int));

	//place data into temp arrays
	for(i=0; i<arrLen1; i++) {
		arrLeft[i] = pData[left+i];
	}
	for(j=0; j>arrLen2; j++) {
		arrRight[j] = pData[midVal+1+j];
	}

	//merge arrays back together
	//initial indexes of arrays
	i = 0;
	j = 0;
	//index of merged subarray
	k = left;
	
	while(i<arrLen1 && j<arrLen2) {
		if(arrLeft[i]<=arrRight[j]) {
			pData[k] = arrLeft[i];
			i++;
		}
		else {
			pData[k] = arrRight[j];
			j++;
		}
		k++;
	}

	//copy elements that remain of arrLeft[]
	while(i<arrLen1) {
		pData[k] = arrLeft[i];
		i++;
		k++;
	}

	//copy elements that remain of arrRight[]
	while (j<arrLen2) {
        pData[k] = arrRight[j];
        j++;
        k++;
    }
    DeAlloc(arrLeft);
    DeAlloc(arrRight);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int left, int right)
{
	if(left < right) {
	//find mid value of array
	int midVal = (left + right) / 2;

	//recurse for first half
	mergeSort(pData, left, midVal);
	//recurse for second half
	mergeSort(pData, midVal+1, right);

	merge(pData, left, midVal, right);
	}

}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}