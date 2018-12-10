#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "testType.h"

int initGraphs(){
    //Allocate all the space for the graph lists
    // numNodes * numNodes
    /**********************/
    //graph = (int **)malloc(sizeof(int *)*numNodes);
    //for(i = 0; i < numNodes; i++)
    //    graph[i] = (int *)malloc(sizeof(int)*numNodes);  
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
	//allNodes[i].edges = (int *)malloc(sizeof(int)*(numNodes));
    }
    /*******************************/



}

int cleanUp(){
    //return 0;
    //free(graph);
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
    //if (!graph[vert][edge]) graph[vert][edge] = 1;
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
    i = 0;
    while(fgets(line, 256, inGraph) != NULL){
    //printf("[%d | %d] Read in line:\n\t%s\n",i,numNodes,line);
	i++;
        //If the line doesnt start with a #
        if (line[0] != '#'){
            sscanf(line, "%d" "%d", &vert, &edge);
           // if (vert == 2257) printf("%d  %d\n",vert,edge);
            numEdges++; 
            if (numEdges%10000 == 0) printf("|");
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
 //   printf("Hop from [%d] to its %dth edge", node, edge);
    if(edge > allNodes[node].numEdges){
       printf("Trying to hop too far\n");
       return -1;
    }
    return allNodes[node].edges[edge];
}

int goToRandomNeighbor(int node){
//    printf("Hop from [%d] which has %d edges\n", node, allNodes[node].numEdges);
    if(allNodes[node].numEdges < 1) return -1;

    int edge = rand();
    edge = edge%allNodes[node].numEdges;
    //int edge = 1;
    return goToOneHopNeighbor(node,edge);
}

int addToTop5(int node){
    //get the current node's link count into links
    int ogNode = node;
    int links = allNodes[node].numLinks;
    for(j = 0; j < 5; j++){
    //For every verticie in the top5
        //if the current top5 slot is unfilled
            //fill it
        //printf("Top5 j -> %d    node -> %d\n",top5[j],node);
        if (top5[j] == node) return 0;
        if (top5[j] == -1){
            top5[j] = node;
            return 0;
        }else
        //if the current links are greater than the
        //top5 node's links
        if (links > allNodes[top5[j]].numLinks){
            //get store the links of the old top5 node
	       links = allNodes[top5[j]].numLinks;
            //store the old top5 node
	       int tempNode = top5[j];
            //set the new top5 node
	       top5[j] = node;
            //store the old top5 node as the new node to enter
	       node = tempNode;
	       for (k = j+1; k<5;k++){
            //for the rest of the list 
                if (top5[k] == ogNode){
                    top5[k] = node;
                    return 0;
                }
               tempNode = top5[k];
                top5[k] = node;
                node = tempNode;
	       }
           return 0;
         }

     }
        
}
int walk = 0;
int randomWalk(int node, int length, double damp){
    /*Inputs to the function
    *node -> starting index node
    *length -> how many edges to follow
    *damp (0<X<1) -> the change to the probablity
    */

    /*
    For every edge followed in the length of the walk
    have a node u
    Flip a coin:
        get a random integer, mod by 100
        greater than 100*d then tails
        less than 100*d then heads
    
    if Heads:
        jump to a random node in the entire graph
    if Tails:
        jump to a random node in the set of u's neighbors
    
    After each edge is followed, increment the target verticie's
        numLinks
    */
    double coinToss;
    //printf("Starting a random walk [%d]\n---------------\n",walk++);
    for(i = 0; i < length; i++){
        

        coinToss = (rand()%100);
        coinToss /= 100;

        //coinToss = 100;
        //printf("The cointoss was -> %lf and damp is %lf i am -> ",coinToss,damp);
        if(coinToss < damp || allNodes[node].numEdges == 0){
            //Heads -> go to a random node in the graph
            node = ((rand())%numNodes);
            if (node < 0) node *= -1;
        }
        else if(coinToss > damp){
         //   printf("Random neighbor \n");
            //Tails -> Go to a random 1 hop neighbor
            node = goToRandomNeighbor(node);
        }
/*
        if(coinToss < damp || node == -1){
            printf("Random node \n");
            //Heads -> go to a random node in the graph
            node = ((rand()*numNodes*2)%numNodes);
            if (node < 0) node *= -1;
            while (allNodes[node].numEdges == 0){
                 node = ((rand()*numNodes*2)%numNodes);
                 if (node < 0) node *= -1;
            } 
            //printf("The node i got randomly is %d\n",node);
        }
*/
        allNodes[node].numLinks += 1;
//        printf("%d[%d]{%d}%s",node,allNodes[node].numLinks,allNodes[node].numEdges, (i+1)%5 == 0 ? "\n" : "->");
        addToTop5(node);
    }
}


