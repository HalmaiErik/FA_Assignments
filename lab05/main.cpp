/*
 *      - Open Addressing = all elements occupy the hash table itself, no lists
 *      - Quadratic probing uses a hash function of the form: h(k, i) = (h'(k) + c1*i + c2*i^2) % TABLE_SIZE
 *      - works much better than linear probing
 *      - as TABLE_SIZE, it is better to use prime numbers that are not to close to a power of 2 (they give better results)
 *      - as we can see from the table, searching element needs more effort when we increase number of elements inserted in the table.
 *      This is because it doesn't find and empty position, thus it runs until i = TABLE_SIZE, making some searches O(TABLE_SIZE)
 *      - searching effort depends on the hash function
 *
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <time.h>
#include <string.h>
#include "Profiler.h"

#define TABLE_SIZE 9973

int nrSearch = 0;

typedef struct {
    int id;
    char name[30];
} Entry;

void hashTableInit(Entry hashTable[], int n) {
    for (int i = 0; i < n; i++) {
        hashTable[i].id = -1;
        strcpy(hashTable[i].name, "Empty");
    }
}

int hashFunc(int id, int n) {
    return (id % n);
}

int hashQuadratic(int id, int i, int n) {
    return ( (hashFunc(id, n) + i + (i * i)) % n );
}

bool insertElement(Entry hashTable[], int id, int n) {
    int i = 0;
    while (i < n) {
        int pos = hashQuadratic(id, i, n);
        if(hashTable[pos].id == -1) {
            hashTable[pos].id = id;
            return true;
        }
        i++;
    }
    return false;
}

void insertElementDemo(Entry hashTable[], int id, const char name[30], int n) {
    int i = 0;
    while (i < n) {
        int pos = hashQuadratic(id, i, n);
        if(hashTable[pos].id == -1) {
            hashTable[pos].id = id;
            strcpy(hashTable[pos].name, name);
            printf("The element %d was inserted in position %d\n", id, pos + 1);
            return;
        }
        i++;
    }
    printf("id = %d, name = %s can't be inserted in the table\n", id, name);
}

bool searchElement(Entry hashTable[], int id, int n) {
    int i = 0;
    while(i < n) {
        nrSearch++;

        int pos = hashQuadratic(id, i, n);
        if(hashTable[pos].id == -1)
            return false;
        if (hashTable[pos].id == id)
            return true;
        i++;
    }
    return false;
}

bool searchElementDemo(Entry hashTable[], int id, int n) {
    int i = 0;
    while(i < n) {

        int pos = hashQuadratic(id, i, n);
        if(hashTable[pos].id == -1) {
            printf("Element not found\n");
            return false;
        }
        if (hashTable[pos].id == id) {
            printf("ELement found: id = %d, name = %s\n", hashTable[pos].id, hashTable[pos].name);
            return true;
        }
        i++;
    }
    printf("Element not found\n");
    return false;
}


void showHashTable(Entry hashTable[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d element: id = %d, name = %s\n", i+1, hashTable[i].id, hashTable[i].name);
    }
}

int main() {
    /// Proof of Corectness
    printf("Proof of corectness:\n");
    Entry hashTableDemo[5];
    hashTableInit(hashTableDemo, 5);

    printf("\nEmpty hash table:\n");
    showHashTable(hashTableDemo, 5);

    printf("\nInsertion of 5 elements:\n");
    insertElementDemo(hashTableDemo, 10, "a", 5);
    insertElementDemo(hashTableDemo, 12, "b", 5);
    insertElementDemo(hashTableDemo, 16, "c", 5);
    insertElementDemo(hashTableDemo, 19, "d", 5);
    insertElementDemo(hashTableDemo, 22, "e", 5);
    showHashTable(hashTableDemo, 5);

    printf("\nInsertion into a full hash table:\n");
    insertElementDemo(hashTableDemo, 30, "f", 5);
    printf("The table remains the same:\n");
    showHashTable(hashTableDemo, 5);

    printf("\nSuccessful search:\n");
    searchElementDemo(hashTableDemo, 19, 5);
    printf("Unsuccessful search:\n");
    searchElementDemo(hashTableDemo, 20, 5);

    /// CSV work
    FILE* fout;
    fout = fopen("lab5.csv", "w+");
    fprintf(fout, "Filling Factor,Avg Effort found,Max Effort found,Avg Effort not-found,Max Effort not-found\n");

    Entry hashTable[TABLE_SIZE];
    float fillingFactor[] = {0.8, 0.85, 0.9, 0.95, 0.99};

   srand((unsigned int)time(NULL));
    /// fillingFactor array
    for (float i : fillingFactor) {
        fprintf(fout, "%f,", i);
        float avgFound = 0;
        float maxFound = 0;
        float avgNotF = 0;
        float maxNotF = 0;

        int nrElem = (int) (i * TABLE_SIZE);
        /// average case
        for (int k = 0; k < 5; k++) {
            hashTableInit(hashTable, TABLE_SIZE);
            /*
            int elem = 0;
            int randomNr;
            int arr[nrElem];
            while(elem < nrElem) {
                randomNr = rand() % 20000 + 1;
                if(insertElement(hashTable, randomNr, TABLE_SIZE)) {
                    arr[elem] = randomNr;
                    elem++;
                }
            }
            */

           int arr[nrElem];
            FillRandomArray(arr, nrElem, 1, 20000, true, 0);
            for (int j = 0; j < nrElem; j++) {
                if(!insertElement(hashTable, arr[j], TABLE_SIZE)) {
                    printf("%d not added\n", arr[j]);
                }
            }


            /// for found searches
            float totalEffortF = 0;
            int mF = 0;
            int index = 0;

            /// there are nrELem elements in the table and we want 1500 elements to be found
            /// these elements need to be sampled uniformly, so to make it uniform we add to index nrElem / 1500 after each loop
            /// so for ex when fillingFact = 0.8 => nrElem = 8000 approx. => nrELem/1500 = 5 => index gets bigger by 5 each loop so at the final loop index = 7500 = 8000 approx.
            /// to get elements which are in the table we use the generated random array and search for the arr[index] element
            for (int j = 0; j < 1500; j++) {
                nrSearch = 0;
                if (searchElement(hashTable, arr[index], TABLE_SIZE)) {
                    if (nrSearch > mF)
                        mF = nrSearch;
                    totalEffortF = totalEffortF + (float) nrSearch;
                    index = index + (nrElem / 1500);
                }
            }
            totalEffortF = totalEffortF / 1500;
            avgFound = avgFound + totalEffortF;
            maxFound = maxFound + (float) mF;

            /// for not found
            float totalEffortNF = 0;
            int mNF = 0;
            int value = 20001;

            /// we do the same thing, just that this time we don't use elements in the array, but values that are outside of the array's range
            for (int j = 1500; j < 3000; j++) {
                nrSearch = 0;
                if (!searchElement(hashTable, value, TABLE_SIZE)) {
                    if (nrSearch > mNF)
                        mNF = nrSearch;
                    totalEffortNF = totalEffortNF + (float) nrSearch;
                    value = value + 50;
                }
            }
            totalEffortNF = totalEffortNF / 1500;
            avgNotF = avgNotF + totalEffortNF;
            maxNotF = maxNotF + (float) mNF;
        }
        avgFound = avgFound / 5;
        maxFound = maxFound / 5;
        avgNotF = avgNotF / 5;
        maxNotF = maxNotF / 5;

        fprintf(fout, "%.2f,%.2f,%.2f,%.2f\n", avgFound, maxFound, avgNotF, maxNotF);
    }
    return 0;
}