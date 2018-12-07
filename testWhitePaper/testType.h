#ifndef TYPE_H
#define TYPE_H
//InGraph is the file that the graph is reading in from
FILE *inGraph;
//A numNodes x numNodes ajacency matrix
int **graph;
//the number of nodes
int numNodes;
//the number of edges 
int numEdges;

int i,j,k; //for using for loops
//Struct to hold a representation of a verticie
    //numLinks is the rank it has in pagerank
            // (update any time a node has an egde to it)
    //edges is the array of all edges the node has (i.e. one hop neighbors)
            // will need to realloc each time an edge is found
typedef struct verticie{
    int numLinks;
    int numEdges;
    //int *edges;
    int *edges;
} VERTICIE;

VERTICIE *allNodes;

#endif 

