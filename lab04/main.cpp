/*
 *  CHARTS:
 *      - as we can see the nr of operations grows slightly from the nr of elements
 *      - it grows much faster from the nr of lists
 *      - time complexity is O(nlogk) because heapify is O(k) and we do it n times for each element
 *
 *   ALGORITHM:
 *      - at each step we build a min heap of the first element in each list => list[0] will be the list
 *      with the minimum first element
 *      - insert that element into the output list
 *      - if a list is empty, we reduce the listNr
 *      - if we only have 2 lists, we use the mergeTwoLists algorithm
 *
 */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <time.h>
#include "Profiler.h"

#define MAXN 10001
#define MAXK 500

int operations_K = 0;

/// -------------------------------- SLL -----------------------------------------------

struct node {
    int data;
    struct node *pNext;
};

struct SLL {
    int count;
    struct node *pFirst;
    struct node *pLast;
};

void createEmptySLL(struct SLL *list) {
    list->count = 0;
    list->pFirst = NULL;
    list->pLast = NULL;
}

struct node *createNode(int data) {
    struct node *ptr = (struct node*) malloc(sizeof(node));
    if(ptr) {
        ptr->data = data;
        ptr->pNext = NULL;
    }
    return ptr;
}

bool isEmpty(struct SLL list) {

    if(list.pFirst == NULL)
        return true;
    return false;
}

void insertAtRear(struct SLL *list, int data) {
    struct node *ptr = createNode(data);
    if(isEmpty(*list)) {
        list->pFirst = ptr;
        list->pLast = ptr;
    }
    else {
        list->pLast->pNext = ptr;
        list->pLast = ptr;
    }
    list->count++;
}

void deleteFirst(struct SLL *list) {
    if(!isEmpty(*list)) {
        node *toDelete = list->pFirst;
        list->pFirst = list->pFirst->pNext;
        list->count--;
        free(toDelete);
    }
}

void showList(struct SLL list) {
    struct node *ptr = list.pFirst;
    if(isEmpty(list)) {
        printf("No elements");
    }
    else {
        while(ptr != NULL) {
            printf("%d ", ptr->data);
            ptr = ptr->pNext;
        }
    }
    printf("\n");
}

void showKLists(struct SLL list[], int k) {
    for (int i = 0; i < k; i++) {
        printf("List %d: ", i + 1);
        showList(list[i]);
    }
}

/// -------------------------------- Heap -----------------------------------------------

void heapify(struct SLL list[], int k, int root) {
    int smallest = root;
    int leftChildInd = 2 * root + 1;
    int rightChildInd = 2 * root + 2;

    operations_K++;
    if(leftChildInd < k && list[leftChildInd].pFirst->data < list[smallest].pFirst->data) {
        smallest = leftChildInd;
    }

    operations_K++;
    if(rightChildInd < k && list[rightChildInd].pFirst->data < list[smallest].pFirst->data) {
        smallest = rightChildInd;
    }

    if(smallest != root) {
        struct SLL buff = list[root];
        list[root] = list[smallest];       //swap
        list[smallest] = buff;
        operations_K = operations_K + 3;

        heapify(list, k, smallest);
    }
}

void buildHeap(struct SLL list[], int k) {
    for (int i = k / 2 - 1; i >= 0; i--) {
        heapify(list, k, i);
    }
}

/// -------------------------------- Merge -----------------------------------------------

void mergeTwoLists(struct SLL *listOut, struct SLL list1, struct SLL list2) {
    while(!isEmpty(list1) && !isEmpty(list2)) {
        operations_K++;
        if(list1.pFirst->data < list2.pFirst->data) {
            insertAtRear(listOut, list1.pFirst->data);
            operations_K++;
            deleteFirst(&list1);
        }

        else {
            operations_K++;
            if(list1.pFirst->data > list2.pFirst->data) {
                insertAtRear(listOut, list2.pFirst->data);
                operations_K++;
                deleteFirst(&list2);
            }

            else {
                insertAtRear(listOut, list1.pFirst->data);
                operations_K++;
                insertAtRear(listOut, list2.pFirst->data);
                operations_K++;

                deleteFirst(&list1);
                deleteFirst(&list2);
            }
        }
    }
    operations_K++;

    //insert remaining elements
    while(!isEmpty(list1)) {
        insertAtRear(listOut, list1.pFirst->data);
        operations_K++;
        deleteFirst(&list1);
    }

    while(!isEmpty(list2)) {
        insertAtRear(listOut, list2.pFirst->data);
        operations_K++;
        deleteFirst(&list2);
    }
}

void mergeKLists(struct SLL *listOut, struct SLL lists[], int k) {
    int nrLists = k;
    buildHeap(lists, k);

    while(nrLists > 2) {
        insertAtRear(listOut, lists[0].pFirst->data);
        operations_K++;
        deleteFirst(&lists[0]);
        if(isEmpty(lists[0])) {
            lists[0] = lists[nrLists - 1];
            nrLists--;
        }
        heapify(lists, nrLists, 0);
    }
    mergeTwoLists(listOut, lists[0], lists[1]);
}

//k = nr of lists, n = nr of elements in all the lists
void buildLists(struct SLL list[], int k, int n) {
    ///because there are k lists with n elements in total, there will be ideally n/k elements in each list
    ///but if n % k > 0, those extra elements need to go into the lists aswell

    int arr[MAXN];

    int extra = n % k;
    for (int i = 0; i < k; i++) {
        if(extra > 0) {
            FillRandomArray(arr, n/k + 1, 1, 5 * n/k, true, 1);
            createEmptySLL(&list[i]);
            for (int j = 0; j < n/k + 1; j++) {
                insertAtRear(&list[i], arr[j]);
            }
            extra--;
        }
        else {
            FillRandomArray(arr, n/k, 1, 5 * n/k, true, 1);
            createEmptySLL(&list[i]);
            for (int j = 0; j < n/k; j++) {
                insertAtRear(&list[i], arr[j]);
            }
        }
    }

}

int main() {
    FILE* fout;
    fout = fopen("lab4.csv", "w+");

    struct SLL lists[MAXK];
    struct SLL listOut;
    createEmptySLL(&listOut);

    printf("Proof of corectness for k = 4, n = 21:\n");
    buildLists(lists, 4, 22);
    printf("The generated lists:\n");
    showKLists(lists, 4);
    printf("The merged list:\n");
    mergeKLists(&listOut, lists, 4);
    showList(listOut);

    ///------------------------------- P1 -----------------------------------------------
    /// k = 5
    fprintf(fout, "N,Operations\n");
    operations_K = 0;
    for (int n = 400; n <= 4000; n = n + 400) {
        createEmptySLL(&listOut);
        fprintf(fout, "%d,", n);
        buildLists(lists, 5, n);
        mergeKLists(&listOut, lists, 5);
        fprintf(fout, "%d\n", operations_K);
        operations_K = 0;
    }

    /// k = 10
    operations_K = 0;
    fprintf(fout, "\nN,Operations\n");
    for (int n = 400; n <= 4000; n = n + 400) {
        createEmptySLL(&listOut);
        fprintf(fout, "%d,", n);
        buildLists(lists, 10, n);
        mergeKLists(&listOut, lists, 10);
        fprintf(fout, "%d\n", operations_K);
        operations_K = 0;
    }

    /// k = 100
    operations_K = 0;
    fprintf(fout, "\nN,Operations\n");
    for (int n = 400; n <= 4000; n = n + 400) {
        createEmptySLL(&listOut);
        fprintf(fout, "%d,", n);
        buildLists(lists, 100, n);
        mergeKLists(&listOut, lists, 100);
        fprintf(fout, "%d\n", operations_K);
        operations_K = 0;
    }

    ///------------------------------- P2 -----------------------------------------------
    operations_K = 0;
    fprintf(fout, "\nK,Operations\n");
    for (int k = 10; k <= 500; k = k + 10) {
        createEmptySLL(&listOut);
        fprintf(fout, "%d,", k);
        buildLists(lists, k, 10000);
        mergeKLists(&listOut, lists, k);
        fprintf(fout, "%d\n", operations_K);
        operations_K = 0;
    }
    return 0;
}