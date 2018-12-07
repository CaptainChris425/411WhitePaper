/*Some Notes
*
*I think allnodes will be all we need not really the ajacency matrix`
*performing the random walks will be easy just mod the one hop neighbor 
*by numEdges
*
*
*
*
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "testType.h"
#include "testGraphFunctions.c"

int main(int argc, char *argv[]){
    srand(time(0));
    if (argc < 3){
        printf("Usage: a.out graph(textfile) #nodes\n");
        exit(0);
    }
    //Store numNodes so it is easier to access as well as an int
    numNodes = atoi(argv[2]);
       
    initGraphs(); 
    readInFileToGraph(argv[1]);
    //Testing if the edges in the struct work as I think theu should
    addEdgeToGraph(0, 14);
    addEdgeToGraph(0, 15);
    printf("Edge index [0] = %d\n",allNodes[10].edges[0]);
    printf("Edge index [1] = %d\n",allNodes[10].edges[1]);
    printf("[%d][%d] -> %d\n",10,25,graph[10][25]);
    printf("[%d][%d] -> %d\n",0,15,graph[2][45]);
    /**************************************************************/





    cleanUp();
}


