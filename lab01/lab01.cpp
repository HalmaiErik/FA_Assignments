//bubbleSort = repeatedly swaps the adjacent elements if they are in wrong order
//insertionSort = takes the i+1th element and places it in the correct place
//selectionSort = repeatedly finds the minimum element from unsorted part and puts it at the beginning of the same unsorted part

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <time.h>

#define MAXN 1501

int bubbleC = 0;
int bubbleA = 0;

int insertionC = 0;
int insertionA = 0;

int selectionC = 0;
int selectionA = 0;

void bubbleSort(int arr[], int arrSize) {
	bool cont = false;
	do {
		cont = false;
		for (int j = 0; j < arrSize - 1; j++) {
			bubbleC++;
			if (arr[j] > arr[j + 1]) {
				cont = true;
				std::swap(arr[j], arr[j + 1]);
				bubbleA = bubbleA + 3;
			}
		}
	} while (cont == true);
}

void insertionSort(int arr[], int arrSize) {
	int j, buff;
	for (int i = 1; i < arrSize; i++) {
		buff = arr[i];
		insertionA++;
		j = i - 1;
		while (j >= 0 && arr[j] > buff) {
			insertionC++;
			arr[j + 1] = arr[j];
			insertionA++;
			j--;
		}
		insertionC++;
		arr[j + 1] = buff;
		insertionA++;
	}
}

void selectionSort(int arr[], int arrSize)
{
	int imin;
	for (int i = 0; i < arrSize - 1; i++) {
		imin = i;
		for (int j = i + 1; j <= arrSize - 1; j++) {
			selectionC++;
			if (arr[j] < arr[imin]) {
				imin = j;
			}
		}
		if (i != imin) {
			std::swap(arr[i], arr[imin]);
			selectionA = selectionA + 3;
		}
	}
}

void showArr(int arr[], int arrSize) {
	for (int i = 0; i < arrSize; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n\n");
}

int main()
{
	FILE* fout;
	fout = fopen("lab2.csv", "w+");
	fprintf(fout, "N,Bubble assignment,Bubble comparisons,Bubble total,Insertion assignment,Insertion comparisons,Insertion total,Selection assignment,Selection comparisons,Selection total,Bubble assignment,Bubble comparisons,Bubble total,Insertion assignment,Insertion comparisons,Insertion total,Selection assignment,Selection comparisons,Selection total,Bubble assignment,Bubble comparisons,Bubble total,Insertion assignment,Insertion comparisons,Insertion total,Selection assignment,Selection comparisons,Selection total\n");

	int arrBubble[MAXN];
	int arrInsert[MAXN];
	int arrSelect[MAXN];

	srand((unsigned int)time(NULL));
	for (int arrSize = 500; arrSize < 1500; arrSize = arrSize + 100) {
		bubbleA = 0;
		bubbleC = 0;
		insertionA = 0;
		insertionC = 0;
		selectionA = 0;
		selectionC = 0;

		for (int i = 0; i < arrSize; i++) {
			arrBubble[i] = rand() % MAXN;
			arrInsert[i] = arrBubble[i];
			arrSelect[i] = arrBubble[i];
		}
		for (int i = 0; i < 5; i++) {
			bubbleSort(arrBubble, arrSize);
			fprintf(fout, "%d,%d,%d,%d,", arrSize, bubbleA, bubbleC, bubbleA + bubbleC);
			insertionSort(arrInsert, arrSize);
			fprintf(fout, "%d,%d,%d,", insertionA, insertionC, insertionA + insertionC);
			selectionSort(arrSelect, arrSize);
			fprintf(fout, "%d,%d,%d,", selectionA, selectionC, selectionA + selectionC);
		}

		bubbleA = 0; 
		bubbleC = 0;
		insertionA = 0; 
		insertionC = 0;
		selectionA = 0; 
		selectionC = 0;
		bubbleSort(arrBubble, arrSize);
		fprintf(fout, "%d,%d,%d,", bubbleA, bubbleC, bubbleA + bubbleC);
		insertionSort(arrInsert, arrSize);
		fprintf(fout, "%d,%d,%d,", insertionA, insertionC, insertionA + insertionC);
		selectionSort(arrSelect, arrSize);
		fprintf(fout, "%d,%d,%d,", selectionA, selectionC, selectionA + selectionC);

		bubbleA = 0; 
		bubbleC = 0;
		insertionA = 0; 
		insertionC = 0;
		selectionA = 0; 
		selectionC = 0;
		int j = 0;
		for (int i = arrSize; i >= 0; i--)
		{
			arrBubble[j] = arrInsert[j] = arrSelect[j] = i;
			j++;
		}
		bubbleSort(arrBubble, arrSize);
		fprintf(fout, "%d,%d,%d,", bubbleA, bubbleC, bubbleA + bubbleC);
		insertionSort(arrInsert, arrSize);
		fprintf(fout, "%d,%d,%d,", insertionA, insertionC, insertionA + insertionC);
		selectionSort(arrSelect, arrSize);
		fprintf(fout, "%d,%d,%d\n", selectionA, selectionC, selectionA + selectionC);
		//printf("%d, %d, %d\n", selectionA, selectionC, selectionA + selectionC);
	}
	fclose(fout);

	int n = 10;
	for (int i = 0; i < n;  i++) {
		arrBubble[i] = rand() % MAXN;
		arrInsert[i] = rand() % MAXN;
		arrSelect[i] = rand() % MAXN;
	}

	printf("Proof of corectness bubble sort:\n");
	showArr(arrBubble, n);
	bubbleSort(arrBubble, n);
	showArr(arrBubble, n);
	
	printf("Proof of corectness insertion sort:\n");
	showArr(arrInsert, n);
	insertionSort(arrInsert, n);
	showArr(arrInsert, n);

	printf("Proof of corectness selection sort:\n");
	showArr(arrSelect, n);
	selectionSort(arrSelect, n);
	showArr(arrSelect, n);
}
