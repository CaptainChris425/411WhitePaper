#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "testType.h"

int initGraphs(){
    //Allocate all the space for the graph lists
    // numNodes * numNodes
    /**********************/
    graph = (int **)malloc(sizeof(int *)*numNodes);
    for(i = 0; i < numNodes; i++)
        graph[i] = (int *)malloc(sizeof(int)*numNodes);  
    /***********************/
    
    
    /*Testing if the graph is allocated*****
    *graph[numNodes/2][numNodes/2] = 1;
    *graph[numNodes-1][numNodes-1] = 1;
    *printf("[%d][%d] -> %d\n",numNodes-1,numNodes-1,graph[numNodes-1][numNodes-1]);
    ***************************************/

    
    //Allocate the list of all nodes 
    /******************************/
    allNodes = (VERTICIE *)malloc(sizeof(VERTICIE)*numNodes);
    for (i = 0; i < numNodes; i++){
        allNodes[i].numEdges = 0;
        allNodes[i].numLinks = 0;
        allNodes[i].edges = (int *)malloc(sizeof(int)*(allNodes[i].numEdges+1));
    }
    /*******************************/



}

int cleanUp(){
    free(graph);
    free(allNodes);
}

int addEdgeToNode(VERTICIE *node, int edge){
    //Add the edge to the last index of the egde list by:
        //Changing the number at edges[numEdges]
        //incrementing numEdges
        //reallocating edges to numEdges + 1;
    
    //printf("%d\n",node->numEdges);
    node->edges[node->numEdges] = edge;
    node->numEdges += 1;
    node->edges = (int *)realloc(node->edges, sizeof(int)*(node->numEdges+1));
    return 0;
}

int addEdgeToGraph(int vert,int edge){
    /*Adds an edge to the graph by:
    *   if it is not already 1
    *   change graph[vert][edge] to 1
    *   call addEdgeToNode on allnodes[vert] with edge to edge
    */
    if (!graph[vert][edge]) graph[vert][edge] = 1;
    addEdgeToNode(&allNodes[vert],edge);
    return 0;

}

int readInFileToGraph(char *file){
    /*Reads in a file representing a graph
    *reads line at a time
    *if the start is not a #
    *split the line into two integers
    *int1 is the verticie
    *int2 is the edge
    *call add edgeToGraph(int1,int2)
    */
    
    char line[256];
    int vert,edge;
    inGraph = fopen(file, "r");
    if (!inGraph){
        printf("Graph could not be read in...\n");
        return 0; 
    }
    //While there is another line to read in the file
    printf("Loading Graph: |");
    while(fgets(line, 256, inGraph) != NULL){
        //If the line doesnt start with a #
        if (line[0] != '#'){
            sscanf(line, "%d" "%d", &vert, &edge);
            //printf("%d  %d\n",vert,edge);
            numEdges++; 
            if (numEdges%1000 == 0) printf("|");
            addEdgeToGraph(vert,edge);

        }//else{
            //printf("%s",line);
          //  continue;
        //}
    }
    printf("| Done Loading...\n");
    fclose(inGraph); 



}

int goToOneHopNeighbor(int node, int edge){
   if(edge > allNodes[node].numEdges){
       printf("Tryping to hop too far");
    }
    return allNodes[node].edges[edge];
}

int goToRandomNeighbor(int node){
    int edge = rand()%allNodes[node].numEdges;
    return goToOneHopNeighbor(node,edge);
}
