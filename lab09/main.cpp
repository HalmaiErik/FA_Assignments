/*
 * The BFS algorithm starts from a point and walks through all vertices,
 * creating a tree and displaying all vertices taken from a queue. In the queue it has
 * the unused, or in process elements (WHITE or GRAY). The elements that in the queue are
 * going to be taken in the order that they were pushed in, and after the element has been used
 * (BLACK), they won't be worked on anymore.
 *
 * The problem of separate components is solved by the lookAfter algorithm which searches for
 * any WHITE elements in the graph, thus finding any element that hasn't been used.
 *
 * As the charts show, the number of operations for BFS is dependent both on the number of vertices and the number of
 * edges.
 */

#include <iostream>

#define size 300

int nrOp = 0;
int queue[size];
int end = -1;
int front = -1;

typedef enum colors {
    WHITE,
    GRAY,
    BLACK
} myColor;

typedef struct Node {
    int value;
    struct Node *next;
} myNode;

typedef struct List {
    int parent;
    int distance;
    myColor color;
    myNode *head;
} myList;

typedef struct Graph {
    int numberV;
    myList *headsArr;
} myGraph;


/**
 * Pushes an element to the queue, which will be the last element of the array
 * @param elem The element to be pushed
 */
void push(int elem) {
    if(end == size - 1) {
        printf("Too many elements");
    }
    else {
        if (front == -1)
            front = 0;
        end++;
        queue[end] = elem;
    }
}

/**
 * Returns the first element of the queue
 * @return The first element of the array
 */
int pop() {
    if (front == -1 || front > end) {
        printf("Queue empty");
        return 0;
    }
    else {
        int x = queue[front];
        front++;
        return x;
    }
}

/**
 * Checks if queue is empty or not
 * @return if queue is empty or not
 */
int isQueueEmpty() {
    if(front == -1 || front > end)
        return 1;
    return 0;
}

/**
 * Creates a new list of neighbours for a vertex
 * @param value The vertex's value
 * @return The created list of neighbours
 */
myNode *createNode(int value) {
    myNode *newNode = (myNode*) malloc(sizeof(myNode));
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

/**
 * Creates and initializes a new graph in which we specify the number of vertices and
 * sets headsArr as nulls
 * @param numberV The number of vertices
 * @return The created graph
 */
myGraph *createGraph(int numberV) {
    myGraph *newGraph = (myGraph*) malloc(sizeof(myGraph));

    // init newGraph
    newGraph->numberV = numberV;
    newGraph->headsArr = (myList*) malloc(sizeof(myList) * numberV);

    // set heads array as nulls
    for (int i = 0; i < numberV; i++) {
        newGraph->headsArr[i].head = NULL;
    }
    return newGraph;
}

/**
 * Adds a new edge in a graph with specified source and destination
 * @param graph The graph in which we add the edge
 * @param src The source of the edge
 * @param dest The destination of the edge
 */
void newEdge(myGraph *graph, int src, int dest) {
    // add to adj list of src
    myNode *temp = createNode(dest);
    temp->next = graph->headsArr[src].head;
    graph->headsArr[src].head = temp;
    // undirected, so vice versa too
    temp = createNode(src);
    temp->next = graph->headsArr[dest].head;
    graph->headsArr[dest].head = temp;
}


/**
 * Performs BFS walk of graph from starting point
 * @param graph The graph on which it performs BFS
 * @param start The starting point
 * @param demo Added this parameter to stop the CSV graph from performing BFS walk, else the BFS would be printed
 * every time the program is executed, making the proof of corectness less visible
 */
void bfs(myGraph *graph, int start, int demo) {
    // init the queue
    front = -1;
    end = -1;
    graph->headsArr[start].color = GRAY;
    graph->headsArr[start].distance = 0;
    nrOp += 2;
    push(start);

    while(!isQueueEmpty()) {
        int i = pop();
        if(demo) {
            // pretty print
            for (int j = 0; j < graph->headsArr[i].distance; j++)
                printf(" ");
            printf("%d\n", i);
        }
        myNode *temp = graph->headsArr[i].head;
        while (temp) {
            nrOp++;
            // check if not done already or in process
            if(graph->headsArr[temp->value].color == WHITE) {
                // in process
                graph->headsArr[temp->value].color = GRAY;
                // increment distance
                graph->headsArr[temp->value].distance = graph->headsArr[i].distance + 1;
                graph->headsArr[temp->value].parent = i;
                nrOp += 3;
                // push in queue
                push(temp->value);
            }
            temp = temp->next;
        }
        nrOp++;
        graph->headsArr[i].color = BLACK;
    }
}

/**
 * Finds the first WHITE element after BFS algorithm performed from a starting point. For unconnected graphs finds
 * the first unconnected vertex
 * @param graph The graph
 * @return Index of first WHITE element or -1
 */
int lookAfter(myGraph *graph) {
    int i = 0;
    while (i < graph->numberV)
    {
        if (graph->headsArr[i].color == WHITE)
            return i;
        i++;
    }
    return -1;
}

/**
 * Initializes graph, runs BFS
 * @param graph The graph
 * @param start The starting point
 * @param demo Added this parameter to stop the CSV graph from performing BFS walk, else the BFS would be printed
 * every time the program is executed, making the proof of corectness less visible
 */
void bfsRun(myGraph *graph, int start, int demo) {
    for (int i = 0; i < graph->numberV; i++) {
        graph->headsArr[i].distance = INT_MAX;
        graph->headsArr[i].color = WHITE;
        graph->headsArr[i].parent = 0;
    }
    bfs(graph, start, demo);
    while(lookAfter(graph) != -1) {
        bfs(graph, lookAfter(graph), demo);
    }
}

/**
 * Checks if edge is already in the graph
 * @param graph The graph
 * @param src Source of edge
 * @param dest Destination of edge
 * @return If edge is already in graph or not
 */
int	checkEdge(myGraph *graph, int src, int dest)
{
    if (src == dest) return 1;
    myNode *temp = graph->headsArr[src].head;
    while (temp != NULL)
    {
        if (temp->value == dest)
            return 1;
        temp = temp->next;
    }
    return 0;
}


void demo() {
    int numberV = 7;
    printf("Vertices: %d\n", numberV);
    myGraph *demoG = createGraph(numberV);
    int numberE;
    printf("Edges(below %d) : ", numberV * (numberV - 1) / 2);
    scanf("%d", &numberE);
    int sp;
    printf("\nStartPoint (has to be in(0, %d)) : ", numberV - 1);
    scanf("%d", &sp);
    for (int i = 0; i < numberE; i++)
    {
        int src = rand() % numberV;
        int dest = rand() % numberV;
        while (checkEdge(demoG, src, dest))
        {
            src = rand() % numberV;
            dest = rand() % numberV;
        }
        printf("(%d %d)\n", src, dest);
        newEdge(demoG, src, dest);
    }
    nrOp = 0;
    printf("\n");
    bfsRun(demoG, sp, 1);
}

int main() {
    /// Proof of corectness
    demo();

    /// CSV work
    FILE* fout;
    fout = fopen("lab9.csv", "w+");
    fprintf(fout, "Varying edges & Vertices = 100\nEdges,Operations\n");
    int numberV = 100;
    for (int i = 1000; i < 5000; i += 100) {
        myGraph *fixedVGraph = createGraph(numberV);
        fprintf(fout,"%d,", i);
        //gen i edges
        for (int j = 0; j < i; j++)
        {
            int src = rand() % numberV;
            int dest = rand() % numberV;
            while (checkEdge(fixedVGraph, src, dest))
            {
                src = rand() % numberV;
                dest = rand() % numberV;
            }
            newEdge(fixedVGraph, src, dest);
        }
        nrOp = 0;
        bfsRun(fixedVGraph, 0, 0);
        fprintf(fout, "%d\n", nrOp);
    }
    fprintf(fout, "\n");

    fprintf(fout, "Varying Vertices & Edges = 4500\nVertices,Operations\n");
    int numberE = 4500;
    for (int i = 100; i <= 200; i += 10)
    {
        fprintf(fout,"%d,", i);
        myGraph *fixedEGraph = createGraph(i);
        for (int j = 0; j < numberE; j++)
        {
            int src = rand() % numberV;
            int dest = rand() % numberV;
            while (checkEdge(fixedEGraph, src, dest))
            {
                src = rand() % numberV;
                dest = rand() % numberV;
            }
            newEdge(fixedEGraph, src, dest);
        }
        nrOp = 0;
        bfsRun(fixedEGraph, 0, 0);
        fprintf(fout, "%d\n", nrOp);
    }
    return 0;
}