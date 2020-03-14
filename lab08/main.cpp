/*
 * Disjoint sets Operations:
 *      - makeSet: O(1)
 *      - linkSets: O(1)
 *      - findSet: O(log n)     n = nr of vertices
 *              - path compression: - make the found root to be parent of searched node
 *                                  - if searched node is root of subtree, then path from subtree's nodes also compresses
 *      - unionSets: O(log n)   n = nr of vertices
 */

#include <iostream>
#include <time.h>

int nrOp = 0;

typedef struct set {
    struct set *parent;
    int value;
    int rank;
} mySetNode;

typedef struct edge {
    int start;
    int end;
    int weight;
} myEdge;

/**
 * Initializes a set.
 * @param node
 */
void makeSet(mySetNode *node) {
    node->parent = node;
    node->rank = 0;
    nrOp += 2;
}

/**
 * Realizes a link between two sets. The smaller ranked set's parent will be the higher ranked set. It also keeps this
 * property if the two sets have equal ranks.
 * @param node1
 * @param node2
 */
void linkSets(mySetNode *node1, mySetNode *node2) {
    nrOp++;
    if(node1->rank > node2->rank) {
        node2->parent = node1;
        nrOp++;
    }
    else {
        node1->parent = node2;
        nrOp++;

        nrOp++;
        if(node1->rank == node2->rank) {
            node2->rank = node2->rank + 1;
            nrOp++;
        }
    }
}

/**
 * Finds the set that contains node.
 * path compression technique
 * @param node
 * @return root of set that contains node
 */
mySetNode *findSet(mySetNode *node) {
    //search for root
    nrOp++;
    if(node != node->parent) {
        nrOp++;
        //points node to root
        node->parent = findSet(node->parent);
    }
    //returns root
    return node->parent;
}

/**
 * Unites two sets.
 * @param node1
 * @param node2
 */
void unionSet(mySetNode *node1, mySetNode *node2) {
    linkSets(findSet(node1), findSet(node2));
}

int myComp(const void* a, const void* b) {
    const myEdge* a1 = (myEdge*) a;
    const myEdge* b1 = (myEdge*) b;

    if(a1->weight > b1->weight)
        return 1;
    else if (a1->weight < b1->weight)
        return -1;
    return 0;
}

/**
 * Creates a Minimum Spanning Tree from a graph.
 * Greedy algorithm
 * @param edges
 * @param numberV
 * @param numberE
 * @return Minimum Spanning Tree
 */
myEdge *kruskal(myEdge *edges, int numberV, int numberE) {
    //create set of vertices
    mySetNode *nodes = (mySetNode*) malloc(sizeof(mySetNode) * (numberV + 1));
    for (int i = 0; i < numberV; i++) {
        nodes[i].value = i + 1;
        makeSet(&nodes[i]);
    }

    //sort edges
    qsort(edges, numberE, sizeof(edges[0]), myComp);

    //create MST
    myEdge *MST = (myEdge*) malloc(sizeof(myEdge) * numberE);

    int nr = 0;
    int index = 0;
    int i = 0;
    while (index < numberV && i < numberE) {
        if(findSet(&nodes[edges[i].start]) != findSet(&nodes[edges[i].end])) {
            MST[nr++] = edges[i];
            unionSet(&nodes[edges[i].start], &nodes[edges[i].end]);
            index++;
        }
        i++;
    }
    /*
    printf("After kruskal:\n");
    for (int i = 0; i < nr; i++) {
        printf("%d,%d : %d\n", MST[i].start, MST[i].end, MST[i].weight);
    }
    */
    return MST;
}

int isUniqueEdge(myEdge *edges, int numberE, int startV, int endV) {
    for (int i = 0; i < numberE; i++) {
        if(((edges[i].start == startV && edges[i].end == endV) || (edges[i].start == endV && edges[i].end == startV)) && startV != endV)
            return 0;
    }
    return 1;
}
int main() {
    /// ------------------------------------------ Proof of Corectness --------------------------------------------
    const int nr = 6;
    mySetNode *demoSet = (mySetNode*) malloc(sizeof(mySetNode) * nr);
    printf("The sets:\n");
    //create sets
    for (int i = 0; i < nr; i++) {
        demoSet[i].value = i;
        makeSet(&demoSet[i]);
        printf("%d ", findSet(&demoSet[i])->value);
    }
    printf("\n\n");
    //union
    int count = 2;
    for (int i = 0; i < nr/2; i++) {
        printf("step: %d\n", i + 1);
        for (int j = 0; j < nr; j += count) {
            if (j + count > nr)
                break;
            unionSet(&demoSet[j], &demoSet[j + (count - 1)]);
        }
        for (int j = 0; j < nr; j++) {
            printf("%d, parent: %d\n", demoSet[j].value, findSet(&demoSet[j])->value);
        }
        count += 2;
    }
    printf("\nDone\n");
    free(demoSet);

    /// ------------------------------------------ CSV work -----------------------------------------------------
    FILE* fout;
    fout = fopen("lab8.csv", "w+");
    fprintf(fout, "N,Operations\n");
    srand((unsigned int)time(NULL));
    for (int n = 100; n <= 10000; n += 100) {
        fprintf(fout, "%d,", n);
        nrOp = 0;
        myEdge *edges = (myEdge*) malloc(sizeof(myEdge) * (4 * n));
        //init
        for(int i = 0; i < 4 * n; i++) {
            edges[i].start = 0;
            edges[i].end = 0;
            edges[i].weight = 0;
        }
        //make sure it is a connected graph
        for (int i = 0; i < n; i++) {
            edges[i].start = i;
            edges[i].end = (i + 1) % n;
            edges[i].weight = (rand() % 10000) + 1;
        }
        //add remaining edges
        for (int i = n; i < 4 *n ; i++) {
            int startV = rand() % n;
            int endV = rand() % n;
            while(!isUniqueEdge(edges, (4*n), startV, endV)) {
                startV = rand() % n;
                endV = rand() % n;
            }
            edges[i].start = startV;
            edges[i].end = endV;
            edges[i].weight = (rand() % 10000) + 1;
        }
        /*
        printf("Before kurskal:\n");
         for (int i = 0; i < 4 * n; i++)
            printf("%d, %d:%d\n", edges[i].start, edges[i].end, edges[i].weight);
        printf("\n\n")
        */
        myEdge *MST = kruskal(edges, n, (4 * n));
        fprintf(fout,"%d\n", nrOp);
        free(MST);
    }
    fclose(fout);
    return 0;
}