/*
 *      OS_SELECT: rank is given
 *                 return the ith smallest node in the tree
 *                 O(h) complexity
 *      OS_DELETE: deletes the ith smallest node from the tree
 *                 rank is given, than with OS_SELECT we find the key at that rank
 *                 call BST_DELETE for the found key
 *                 OS_SELECT = O(h), BST_DELETE = O(h) => OS_DELETE = O(2h) = O(h)
 *
 */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include "Profiler.h"
#include <time.h>


int operationsDel = 0;
int operationsSel = 0;

struct node {
    int key;
    node *left;
    node *right;
    int size;
};

struct node *createNewNode(int key) {
    struct node *ptr = (struct node*) malloc(sizeof(node));
    if(ptr) {
        ptr->key = key;
        ptr->size = 1;
        ptr->left = NULL;
        ptr->right = NULL;
    }
    return ptr;
}

struct node *buildPBT(int arr[], int left, int right) {
    if(left <= right) {
        int mid = (left + right) / 2;
        struct node *newRoot = createNewNode(arr[mid]);
        newRoot->left = buildPBT(arr, left, mid - 1);
        newRoot->right = buildPBT(arr, mid + 1, right);

        ///handle size
        if(newRoot->left) {
            newRoot->size = newRoot->size + newRoot->left->size;
        }
        if(newRoot->right) {
            newRoot->size = newRoot->size + newRoot->right->size;
        }

        return newRoot;
    }
    else return NULL;
}

struct node *OS_Select(struct node *root, int i) {
    operationsSel++;
    if(root) {
        int r = 1;
        operationsSel++;
        if(root->left != NULL)
            r = root->left->size + 1;

        if(i == r)
            return root;
        else {
            if(i < r)
                return OS_Select(root->left, i);
            else return OS_Select(root->right, i - r);
        }
    }
    else return NULL;
}

struct node *findSucc(struct node *pNode) {
    while(pNode->left != NULL) {
        pNode = pNode->left;
        operationsDel = operationsDel + 2;
    }
    operationsDel++;
    return pNode;
}

struct node *BST_Delete(struct node *root, int key) {
    operationsDel++;
    if(root == NULL) {
        return root;
    }
    operationsDel++;
    if(key < root->key) {
        root->size--;
        root->left = BST_Delete(root->left, key);
        operationsDel++;
    }
    else if(key > root->key) {
        root->size--;
        root->right = BST_Delete(root->right, key);
        operationsDel++;
    }

    else {
        operationsDel++;
        if(root->left == NULL) {
            struct node *temp = root->right;
            free(root);
            return temp;
        }
        else {
            operationsDel++;
            if (root->right == NULL) {
                struct node *temp = root->left;
                free(root);
                return temp;
            }
        }
        root->size--;
        operationsDel++;
        struct node *temp = findSucc(root->right);
        root->key = temp->key;
        operationsDel++;
        root->right = BST_Delete(root->right, temp->key);
    }
    return root;
}

struct node *OS_Delete(struct node *root, int i) {
    if(root == NULL) {
        printf("Tree empty, can't delete\n");
        return root;
    }
    struct node *ptr = OS_Select(root, i);
    if(ptr == NULL) {
        printf("Can't find element %d, can't delete\n", i);
        return root;
    }
    else return BST_Delete(root, ptr->key);
}

void inorder(struct node *root, int level) {
    if(root != NULL) {
        inorder(root->left, level + 1);
        printf("%*s(%d,%d)\n\n", 4 * level, "", root->key, root->size);
        inorder(root->right, level + 1);
    }
}

void inorderM(struct node *root) {
    if(root != NULL) {
        inorderM(root->left);
        printf("key = %d, size = %d\n", root->key, root->size);
        inorderM(root->right);
    }
}
int main() {
    ///Corectness
    printf("Proof of corectness:\n");
    struct node *demo = NULL;
    int arr[10000];
    int n = 11;
    FillRandomArray(arr, 10000, 1, 10001, true, 1);;
    demo = buildPBT(arr, 0, n - 1);
    printf("\nTree:\n");
    inorder(demo, 0);
    printf("\n");

    srand(time(NULL));
    for (int i = 0; i < 3; i++) {
        int randNr = rand() % n + 1;
        printf("Searching for element %d to delete\n", randNr);
        demo = OS_Delete(demo, randNr);
        inorder(demo, 0);
        printf("\n");
        n--;
    }

    ///CSV work
    FILE* fout;
    fout = fopen("lab6.csv", "w+");
    fprintf(fout, "N,Select OPs,Delete Ops,Total\n");

    struct node *root = NULL;

    for (int size = 1000; size <= 5000; size = size + 100) {
        operationsDel = 0;
        operationsSel = 0;
        for (int m = 0; m < 5; m++) {
            root = buildPBT(arr, 0, 10000);
            int sequences = size;
            for (int i = 0; i < size; i++) {
                int randNr = rand() % sequences + 1;
                //OS_Select(root, randNr);
                root = OS_Delete(root, randNr);
                sequences--;
            }
        }
        operationsSel /= 5;
        operationsDel /= 5;
        fprintf(fout, "%d,%d,%d,%d\n", size, operationsSel, operationsDel, operationsDel + operationsSel);
    }

    return 0;
}