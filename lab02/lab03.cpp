/*
	1. Bottom-Up implementation:
		-half of the elements in the array are leaves => they are already heaps
		-apply heapify to the first non-leaf node, the node in the tree with the largest index, having at least on child
		-go to the next indexed node until we reach root

	2. Top-Down implementation:
		-we start with a heap of 1 element and we increase the heap by adding the greater elements upwards

	Comparison: 
				The BU method works better because it has to work with only n/2 elements (n/2 are already heaps), while 
			TD method works with all n elements
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <time.h>

#define MAXN 5001

int operationsBU = 0;
int operationsTD = 0;

void heapifyBU(int arr[], int arrSize, int root) {
	int largest = root;
	int leftChildInd = 2 * root + 1;
	int rightChildInd = 2 * root + 2;

	operationsBU++;
	if (leftChildInd < arrSize && arr[leftChildInd] > arr[largest]) {
		largest = leftChildInd;
	}

	operationsBU++;
	if (rightChildInd < arrSize && arr[rightChildInd] > arr[largest]) {
		largest = rightChildInd;
	}

	if (largest != root) {
		std::swap(arr[largest], arr[root]);
		operationsBU = operationsBU + 3;
		heapifyBU(arr, arrSize, largest);
	}
}

void buildHeapBU(int arr[], int arrSize) {
	for (int i = (arrSize / 2) - 1; i >= 0; i--) {
		heapifyBU(arr, arrSize, i);
	}
}

void swimTD(int arr[], int i) {
	while (arr[(i - 1) / 2] < arr[i] && i > 0) {
		operationsTD++;

		std::swap(arr[(i - 1) / 2], arr[i]);
		operationsTD = operationsTD + 3;
		i = (i - 1) / 2;
	}
	operationsTD++;
}

void buildHeapTD(int arr[], int arrSize) {
	for (int i = 1; i < arrSize; i++) {
		swimTD(arr, i);
	}
}

void heapSort(int arr[], int arrSize) {
	buildHeapBU(arr, arrSize);

	for (int i = arrSize - 1; i >= 1; i--) {
		std::swap(arr[0], arr[i]);
		arrSize--;
		heapifyBU(arr, arrSize, 0);
	}
}

void showArr(int arr[], int arrSize) {
	for (int i = 0; i < arrSize; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int main() {
	FILE* fout;
	fout = fopen("lab3.csv", "w+");
	fprintf(fout, "N,Bottom-up Operations,Top-down Operations,Bottom-up Operations,Top-down Operations\n");

	int arrBU[MAXN];
	int arrTD[MAXN];

	srand((unsigned int)time(NULL));
	
	for (int arrSize = 500; arrSize <= 5000; arrSize = arrSize + 500) {
		fprintf(fout, "%d,", arrSize);
		operationsBU = 0;
		operationsTD = 0;

		//average
		for (int m = 1; m <= 5; m++) {
			for (int i = 0; i < arrSize; i++) {
				arrBU[i] = rand() % MAXN;
				arrTD[i] = arrBU[i];
			}
			buildHeapBU(arrBU, arrSize);
			buildHeapTD(arrTD, arrSize);
		}
		operationsBU = operationsBU / 5;
		operationsTD = operationsTD / 5;
		fprintf(fout, "%d,%d,", operationsBU, operationsTD);
		operationsBU = 0;
		operationsTD = 0;


		//worst: when the array is ascending, because we create max heap
		for (int i = 0; i < arrSize; i++) {
			arrBU[i] = i;
			arrTD[i] = i;
		}
		buildHeapBU(arrBU, arrSize);
		buildHeapTD(arrTD, arrSize);
		fprintf(fout, "%d,%d\n", operationsBU, operationsTD);
		operationsBU = 0;
		operationsTD = 0;
	}
	fclose(fout);

	int n = 10;
	for (int i = 0; i < n; i++) {
		arrBU[i] = rand() % 100;
		arrTD[i] = rand() % 100;
	}

	printf("Proof of corectness for bottom-up implementation:\n");
	showArr(arrBU, n);
	buildHeapBU(arrBU, n);
	showArr(arrBU, n);
	printf("\n");

	printf("Proof of corectness for top-down implementation:\n");
	showArr(arrTD, n);
	buildHeapTD(arrTD, n);
	showArr(arrTD, n);
	printf("\n");

	printf("Proof of corectness for heap sort:\n");
	for (int i = 0; i < n; i++) {
		arrBU[i] = rand() % 100;
	}
	showArr(arrBU, n);
	heapSort(arrBU, n);
	showArr(arrBU, n);
}