#include <iostream>
#include <stdlib.h>

#define size 300

int nrOp = 0;
int stack[size];
int tarjStack[size];
int top = -1;

int topoSize = 0;
int strongConnSize = 0;

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
 * Pushes an element onto the stack, which will be the last element of the array
 * @param elem The element to be pushed
 */
void push(int elem) {
    if(top == size - 1) {
        printf("Too many elements");
    }
    else {
        if (top == -1)
            top = 0;
        stack[top++] = elem;
    }
}

/**
 * Returns the top of the stack
 * @return The top of the stack
 */
int pop() {
    if (top == 0) {
        printf("Can't pop");
        return 0;
    }
    else {
        top--;
        return stack[top];
    }
}

/**
 * Checks if stack is empty or not
 * @return if stack is empty or not
 */
int isStackEmpty() {
    if(top == 0)
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
}

/**
 * Performs DFS walk of graph from starting point
 * @param graph The graph on which it performs BFS
 * @param start The starting point
 * @param demoType = 1 for dfs, = 2 for topological sort
 */
void dfs(myGraph *graph, int start, int demoType) {
    // init the stack
    top = -1;
    graph->headsArr[start].color = GRAY;
    graph->headsArr[start].distance = 0;
    nrOp += 2;
    push(start);

    while(!isStackEmpty()) {
        int i = pop();
        if(demoType == 1) {
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
 * Finds the first WHITE element after DFS algorithm performed from a starting point. For unconnected graphs finds
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
 * Initializes graph, runs DFS
 * @param graph The graph
 * @param start The starting point
 * @param demoType = 1 for dfs, = 2 for topological sort
 */
void dfsRun(myGraph *graph, int start, int demoType) {
    for (int i = 0; i < graph->numberV; i++) {
        graph->headsArr[i].distance = INT_MAX;
        graph->headsArr[i].color = WHITE;
        graph->headsArr[i].parent = 0;
    }
    dfs(graph, start, demoType);
    while(lookAfter(graph) != -1) {
        dfs(graph, lookAfter(graph), demoType);
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

void dfsDemo() {
    int numberV = 5;
    printf("Vertices: %d\n", numberV);
    myGraph *demoG = createGraph(numberV);
    int numberE;
    printf("Edges(below %d) : ", numberV * (numberV - 1));
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
    dfsRun(demoG, sp, 1);
}

void topoSortSrc(int i, bool *visited, int *topoStack, myGraph *graph) {
    visited[i] = true;
    myNode *temp = graph->headsArr[i].head;
    while(temp != NULL) {
        int v = temp->value;
        if(!visited[v])
            topoSortSrc(v, visited, topoStack, graph);
        temp = temp->next;
    }
    topoStack[topoSize++] = i;
}

/**
 * Topological sort works for a directed graph. It is an ordering of vertices such that for every directed edge uv,
 * vertex u comes before vertex v in the ordering.
 * @param graph The graph on which it performs Topological Sort
 */
void topoSort(myGraph *graph) {
    int *topoStack = (int*) malloc(graph->numberV * sizeof(int));
    bool *visited = (bool*) malloc(graph->numberV * sizeof(bool));
    for (int i = 0; i < graph->numberV; i++)
        visited[i] = false;
    for (int i = 0; i < graph->numberV; i++)
        if(visited[i] == false)
            topoSortSrc(i, visited, topoStack, graph);

    for (int i = graph->numberV - 1; i >= 0; i--)
        printf("%d ", topoStack[i]);
}

void topoDemo() {
    int numberV = 6;
    int numberE = 6;
    int start = 0;
    myGraph *topoG = createGraph(numberV);
    printf("Vertices : %d\n", numberV);
    printf("Edges: %d\n", numberE);
    printf("Start node: %d\n",  start);

    // hardcode
    newEdge(topoG, 5, 0);
    newEdge(topoG, 5, 2);
    newEdge(topoG, 4, 0);
    newEdge(topoG, 4, 1);
    newEdge(topoG, 3, 1);
    newEdge(topoG, 2, 3);

    for (int i = 0; i < numberV; i++)
    {
        printf("%d :", i);
        myNode* temp = topoG->headsArr[i].head;
        while (temp != NULL)
        {
            printf("%d ", temp->value);
            temp = temp->next;
        }
        printf("\n");
    }

    topoSort(topoG);
}

void strongConComp(myGraph *graph, int currNode, int index[], int low[], int *stack, bool onStack[]) {
    static int in = 0;
    index[currNode] = low[currNode] = ++in;
    stack[strongConnSize++] = currNode;
    onStack[currNode] = true;
    myNode *temp = graph->headsArr[currNode].head;

    while(temp != NULL) {
        int nextNode = temp->value;

        if(index[nextNode] == -1) {
            strongConComp(graph, nextNode, index, low, stack, onStack);
            low[currNode] = __min(low[currNode], low[nextNode]);
        }
        else if(onStack[nextNode] == true)
            low[currNode] = __min(low[currNode],index[nextNode]);
        temp = temp->next;
    }

    int vertex = 0;
    if(low[currNode] == index[currNode]) {
        while(stack[strongConnSize] != currNode) {
            vertex = stack[strongConnSize];
            onStack[vertex] = false;
            printf("%d ", vertex);
            strongConnSize--;
        }
        vertex = (int)stack[strongConnSize];
        onStack[vertex] = false;
        printf("%d\n", vertex);
        strongConnSize--;
    }
}

void tarjan(myGraph *graph) {
    int *disc = (int*) malloc(graph->numberV * sizeof(int));
    int *low = (int*) malloc(graph->numberV * sizeof(int));
    bool *onStack = (bool*) malloc(graph->numberV * sizeof(bool));
    int *stack = (int*) malloc(graph->numberV * sizeof(int));

    for(int i = 0; i < graph->numberV; i++) {
        onStack[i] = false;
        disc[i] = low[i] = -1;
    }

    for (int i = 0; i < graph->numberV; i++)
        if(disc[i] == -1)
            strongConComp(graph, i, disc, low, stack, onStack);
}

void tarjanDemo() {
    int numberV = 6;
    int numberE = 6;
    int start = 0;
    myGraph *tarjG = createGraph(numberV);
    printf("Vertices : %d\n", numberV);
    printf("Edges: %d\n", numberE);
    printf("Start node: %d\n",  start);

    // hardcode
    newEdge(tarjG, 5, 0);
    newEdge(tarjG, 5, 2);
    newEdge(tarjG, 4, 0);
    newEdge(tarjG, 4, 1);
    newEdge(tarjG, 3, 1);
    newEdge(tarjG, 2, 3);

    for (int i = 0; i < numberV; i++)
    {
        printf("%d :", i);
        myNode* temp = tarjG->headsArr[i].head;
        while (temp != NULL)
        {
            printf("%d ", temp->value);
            temp = temp->next;
        }
        printf("\n");
    }

    tarjan(tarjG);
}
int main() {
    /// CSV
    FILE* fout;
    fout = fopen("lab10.csv", "w+");
    fprintf(fout, "Varying edges & Vertices = 100\nEdges,Operations\n");
    int numberV = 100;
    for (int i = 1000; i <= 5000; i += 100) {
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
        dfsRun(fixedVGraph, 0, 0);
        fprintf(fout, "%d\n", nrOp);
    }
    fprintf(fout, "\n");

    fprintf(fout, "Varying Vertices & Edges = 9000\nVertices,Operations\n");
    int numberE = 9000;
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
        dfsRun(fixedEGraph, 0, 0);
        fprintf(fout, "%d\n", nrOp);
    }

    /// Proof of corectness
    printf("DFS:\n");
    dfsDemo();
    printf("\nTopological Sort:\n");
    topoDemo();
    //printf("\nTarjan:\n");
    //tarjanDemo();

    return 0;
}
