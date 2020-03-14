/*
	QuickSort:
            - not stable, because <=
            - in-place
            - worst case complexity: n^2
            - best case complexity: n * log n

    QuickSort vs HeapSort:
            - QS behaves better in the average case, as HS does a lot more swaps than QS
            - HS swaps n elements even when they are sorted
            - QS doesn't swap what is already sorted
            - HS behaves better in the worst case

    QuickSort Worst & Best Case:
            - best case: when the partition function always chooses the middle element as a pivot
            - worst case: when the partition function always chooses the pivot in a way that one partition always has a single element
            this can happen if we take a sorted array in descending order and always choosing arr[arrSize - 1] as the pivot
            - in the worst case, many assignments because of the partitioning
*/


#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <time.h>

#define MAXN 5001
#define N 10

int operationsHS = 0;
int operationsQS = 0;

//heapsort

void heapifyBU(int arr[], int arrSize, int root) {
	int largest = root;
	int leftChildInd = 2 * root + 1;
	int rightChildInd = 2 * root + 2;

	operationsHS++;
	if (leftChildInd < arrSize && arr[leftChildInd] > arr[largest]) {
		largest = leftChildInd;
	}

	operationsHS++;
	if (rightChildInd < arrSize && arr[rightChildInd] > arr[largest]) {
		largest = rightChildInd;
	}

	if (largest != root) {
		std::swap(arr[largest], arr[root]);
		operationsHS = operationsHS + 3;
		heapifyBU(arr, arrSize, largest);
	}
}

void buildHeapBU(int arr[], int arrSize) {
	for (int i = (arrSize / 2) - 1; i >= 0; i--) {
		heapifyBU(arr, arrSize, i);
	}
}

void heapSort(int arr[], int arrSize) {
	buildHeapBU(arr, arrSize);

	for (int i = arrSize - 1; i >= 1; i--) {
		std::swap(arr[0], arr[i]);
		operationsHS = operationsHS + 3;
		arrSize--;
		heapifyBU(arr, arrSize, 0);
	}
}

//quicksort average

int partition(int arr[], int left, int right) {
	int pivot = arr[right];
	operationsQS++;
	int i = left - 1;

	for (int j = left; j < right; j++) {
		operationsQS++;
		if (arr[j] <= pivot) {
			i++;

			std::swap(arr[i], arr[j]);
			operationsQS = operationsQS + 3;
		}
	}
	std::swap(arr[i + 1], arr[right]);
	operationsQS = operationsQS + 3;
	return i + 1;
}

void quickSort(int arr[], int left, int right) {
	if (left < right) {
		int partIndex = partition(arr, left, right);
		quickSort(arr, left, partIndex - 1);
		quickSort(arr, partIndex + 1, right);
	}
}

//quicksort best

int partitionBest(int arr[], int left, int right) {
	int pivot = arr[(right + left) / 2];
	operationsQS++;
	int i = left - 1;

	for (int j = left; j < right; j++) {
		operationsQS++;
		if (arr[j] <= pivot) {
			i++;

			std::swap(arr[i], arr[j]);
			operationsQS = operationsQS + 3;
		}
	}
	std::swap(arr[i + 1], arr[right]);
	operationsQS = operationsQS + 3;
	return i + 1;
}

void quickSortBest(int arr[], int left, int right) {
	if (left < right) {
		int partIndex = partitionBest(arr, left, right);
		quickSortBest(arr, left, partIndex - 1);
		quickSortBest(arr, partIndex + 1, right);
	}
}

//quickselect / randomized select

int random(int left, int right) {
	return left + rand() % (right - left);
}

int randomizedPartition(int arr[], int left, int right) {
	int i = random(left, right);

	std::swap(arr[i], arr[right]);
	return partition(arr, left, right);
}

int randomizedSelect(int arr[], int left, int right, int i) {
	if (left == right) {
		return arr[left];
	}

	int q = randomizedPartition(arr, left, right);
	int k = q - left + 1;
	if (i == k) {
		return arr[q];
	}
	else if (i < k) {
		return randomizedSelect(arr, left, q - 1, i);
	}
	else return randomizedSelect(arr, q + 1, right, i - k);
}

void quickSelect(int arr[], int left, int right, int arrSize)
{

    if (left < right)
    {
        int q = randomizedSelect(arr, left, right, (arrSize - 1) / 2);

        quickSort(arr, left, (arrSize - 1) / 2);
        quickSort(arr, (arrSize - 1) / 2 + 1, right);
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
	fprintf(fout, "N,Heapsort Operations,Quicksort Operations,Quicksort Operations,Quicksort Operations\n");

	int arrHS[MAXN];
	int arrQS[MAXN];

	srand((unsigned int)time(NULL));

	for (int arrSize = 500; arrSize <= 5000; arrSize = arrSize + 500) {
		fprintf(fout, "%d,", arrSize);
		operationsHS = 0;
		operationsQS = 0;

		//average
		for (int m = 0; m < 5; m++) {
			for (int i = 0; i < arrSize; i++) {
				arrHS[i] = rand() % MAXN;
				arrQS[i] = arrHS[i];
			}
			heapSort(arrHS, arrSize);
			quickSort(arrQS, 0, arrSize - 1);
		}
		operationsHS = operationsHS / 5;
		operationsQS = operationsQS / 5;
		fprintf(fout, "%d,%d,", operationsHS, operationsQS);
		operationsHS = 0;
		operationsQS = 0;

		//best quicksort: when the partition function always chooses the middle element as a pivot
		quickSortBest(arrQS, 0, arrSize - 1);
		fprintf(fout, "%d,", operationsQS);
		operationsQS = 0;

		//worst case: when the partition function always chooses the pivot in a way that one partition always has a single element
		//			  this can happen if we take a sorted array in descending order and always choosing arr[arrSize - 1] as the pivot

		for (int i = arrSize - 1; i >= 0; i--) {
			arrQS[i] = i;
		}
		quickSort(arrQS, 0, arrSize - 1);
		fprintf(fout, "%d\n", operationsQS);
	}
	fclose(fout);

    int arrQSEL[N];
	for (int i = 0; i < N; i++) {
		arrHS[i] = rand() % 100;
		arrQS[i] = rand() % 100;
		arrQSEL[i] = rand() % 100;
	}

	printf("Proof of corectness heapsort:\n");
	showArr(arrHS, N);
	heapSort(arrHS, N);
	showArr(arrHS, N);

	printf("Proof of corectness quicksort:\n");
	showArr(arrQS, N);
	quickSort(arrQS, 0, N - 1);
	showArr(arrQS, N);

    printf("Proof of corectness quickselect:\n");
    showArr(arrQSEL, N);
    quickSelect(arrQSEL, 0, N - 1, N - 1);
    showArr(arrQSEL, N);
	return 0;

}