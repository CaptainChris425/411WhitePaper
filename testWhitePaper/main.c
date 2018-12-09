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
#include "testFunctions.c"

int main(int argc, char *argv[]){
    srand(time(0));
    if (argc < 4){
        printf("Usage: a.out graph(textfile) #nodes lengthofwalk\n");
        exit(0);
    }
    //Store numNodes so it is easier to access as well as an int
    numNodes = atoi(argv[2]);
    int length = atoi(argv[3]);
       
    initGraphs(); 

    readInFileToGraph(argv[1]);
    //Testing if the edges in the struct work as I think theu should
    addEdgeToGraph(0, 14);
    addEdgeToGraph(0, 15);
    printf("Edge index [0] = %d\n",allNodes[10].edges[0]);
    printf("Edge index [1] = %d\n",allNodes[10].edges[1]);
    printf("A random edge from node 2257 is %d\n", goToRandomNeighbor(2257));
    printf("2257's edge count = %d\n",allNodes[2257].numEdges);
    //printf("[%d][%d] -> %d\n",10,25,graph[10][25]);
    //printf("[%d][%d] -> %d\n",0,15,graph[2][45]);
    int everyNode = 0;
    for(; everyNode < numNodes; everyNode++){
        randomWalk(everyNode,length, .5);
    }
    /**************************************************************/


    int totalLinks = 0;
    for(everyNode = 0; everyNode < numNodes; everyNode++){
        totalLinks += allNodes[everyNode].numLinks;
//        printf("Node %d has links %d making the total %d\n",everyNode,allNodes[everyNode].numLinks,totalLinks);


    }
    printf("Total links -> %d\n",totalLinks);

    for(i=0;i<5;i++){
        printf("Node [%d] -> Links [%d] -> Edges[%d]\n", top5[i], allNodes[top5[i]].numLinks,allNodes[top5[i]].numEdges);

    }


    cleanUp();
}


