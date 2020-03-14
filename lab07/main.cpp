
/*
 * Multiway representation:  - struct with {info, count of children and children vector of 15}.
 *                           - O(n) it puts the left child in than calls recursion for next root,
 *                          than when it comes back up, it puts the remaining children in.
 *                           - the children vector can be allocated for each n making it O(n) additional memory.
 *
 *  Binary representation: - struct with {value, child, sibling}.
 *                         - O(n) puts one child, one sibling then calls recursion. Same as before go left
 *                         until possible then finish right.
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct pNode {
    int info;
    int count;
    int child[15];
} parentNode;

typedef struct mwNode {
    int info;
    int count;
    struct mwNode *child[15];
} multiWayNode;

typedef struct bNode {
    int value;
    struct bNode *child;
    struct bNode *sibling;
} binaryNode;

/// This function transforms the parent representation to a multiway representation
/// by putting all the children in an array of children for the parent.
/// \param parent
/// \param mRoot

void parentToMultiWay(parentNode parent[], multiWayNode **mRoot) {
    int temp = (*mRoot)->info;
    for (int i = 0; i < parent[temp].count; i++) {
        (*mRoot)->child[i] = (multiWayNode*) malloc(sizeof(multiWayNode));
        (*mRoot)->child[i]->count = 0;
        (*mRoot)->count++;
        (*mRoot)->child[i]->info = parent[temp].child[i]; //put value from initial array into child vector
        parentToMultiWay(parent, &(*mRoot)->child[i]); //look for new child
    }
    return;
}

/// This function transforms the multiway representation to a binary one by setting
/// the first (left) child as a child and chaining all the other siblings to it for every parent node
/// \param mRoot
/// \param bRoot

void multiWayToBinary(multiWayNode *mRoot, binaryNode **bRoot) {
    binaryNode *temp = 0;
    int i = 0;
    while(i < mRoot->count) {
        int val = mRoot->child[i]->info;
        if(i == 0) {
            (*bRoot)->child = (binaryNode*) malloc(sizeof(binaryNode));
            (*bRoot)->child->child = (*bRoot)->child->sibling = NULL;
            (*bRoot)->child->value = val;
            temp = (*bRoot)->child;
            i++;
        }
        else {
            temp->sibling = (binaryNode*) malloc(sizeof(binaryNode));
            temp->sibling->child = temp->sibling->sibling = NULL;
            temp->sibling->value = val;
            multiWayToBinary(mRoot->child[i - 1], &temp);
            temp = temp->sibling;
            i++;
        }
    }
}

void prettyPrintMW(multiWayNode *mRoot) {
    int i = 0;
    while(i < mRoot->count) {
        printf("%d, child: %d\n", mRoot->info, mRoot->child[i]->info);
        prettyPrintMW(mRoot->child[i]);
        i++;
    }
}

void prettyPrintB(binaryNode *bRoot, int level = 0) {
    int i = 0;
    while(i < level) {
        printf("  ");
        i++;
    }
    printf("{%d\n", bRoot->value);
    if(bRoot->child != 0)
        prettyPrintB(bRoot->child, level + 1);
    if(bRoot->sibling != 0)
        prettyPrintB(bRoot->sibling, level);
}


int main() {
    int r1[] = {0, 2, 7, 5, 2, 7, 7, -1, 5, 2};
    parentNode parent[10];
    printf("The parent's array:\n");
    for (int i = 1; i < 10; i++) {
        printf("%d ", r1[i]);
    }

    for (int i = 1; i < 10; i++)
        parent[i].count = 0;
    int root = 0;
    for (int i = 1; i < 10; i++) {
        if(r1[i] == -1)
            root = i;
        else
            parent[r1[i]].child[parent[r1[i]].count++] = i;
    }

    multiWayNode *mRoot = (multiWayNode*) malloc(sizeof(multiWayNode));
    mRoot->count = 0;
    mRoot->info = root;
    parentToMultiWay(parent, &mRoot);
    printf("\n\nMultiway representation:\n");
    prettyPrintMW(mRoot);

    binaryNode *bRoot = (binaryNode*) malloc(sizeof(binaryNode));
    bRoot->value = root;
    bRoot->child = bRoot->sibling = NULL;
    multiWayToBinary(mRoot, &bRoot);
    printf("\n\nBinary representation:\n");
    prettyPrintB(bRoot);
    return 0;
}