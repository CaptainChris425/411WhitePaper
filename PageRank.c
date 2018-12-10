#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
//#include "testType.h"
//#include "testFunctions.c"
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
            // (update anytime a node has an egde to it)
    //edges is the array of all edges the node has (i.e. one hop neighbors)
            // will need to realloc each time an edge is found
typedef struct vertex{
    int numLinks;
    int numEdges;
    //int *edges;
    int *edges;
} VERTEX;

VERTEX *allNodes;
int top5[5] = {-1,-1,-1,-1,-1};

int initGraphs();
int cleanUp();
int addEdgeToNode(VERTEX *node, int edge);
int addEdgeToGraph(int vert,int edge);
int readInFileToGraph(char *file);
int goToOneHopNeighbor(int node, int edge);
int goToRandomNeighbor(int node, int seed);

int main(int argc, char *argv[]){
   // srand((int)time(0));
    if (argc < 5){
        printf("Usage: a.out graph(textfile) #nodes lengthOfWalk dampingRatio\n");
        exit(0);
    }
    //Store numNodes so it is easier to access as well as an int
    numNodes = atoi(argv[2]);
    int length = atoi(argv[3]);
    double damp = atof(argv[4]); 
    int walk = 0;
    initGraphs(); 
    readInFileToGraph(argv[1]);
    //Testing if the edges in the struct work as I think theu should
    addEdgeToGraph(0, 14);
    addEdgeToGraph(0, 15);
    printf("Edge index [0] = %d\n",allNodes[10].edges[0]);
    printf("Edge index [1] = %d\n",allNodes[10].edges[1]);
    printf("A random edge from node 2257 is %d\n", goToRandomNeighbor(2257, 2257));
    printf("2257's edge count = %d\n",allNodes[2257].numEdges);
    //printf("[%d][%d] -> %d\n",10,25,graph[10][25]);
    //printf("[%d][%d] -> %d\n",0,15,graph[2][45]);


/***PARALLEL REGION BELOW****************************/
/*This is where the parallelization needs to start
* the graph is already created at this point
* 
* Parallel Module : OpenMP
* Parallel Algorithm : PageRank
* Omp Parallel Region type : omp for
* Scheduling : Static (every walk is the same length)
* Shared : AllNodes (the representation of the graph)
*          Top5     (the top 5 ranked nodes)
* Private : node (the current node)
*           seed (the seed used for random number gen)
* Atomic Write :    allNodes[node].numLinks++ (Increasing links count)
*                   top5[index] = node        (Adding node to top5)
*
*/
/*1*******Doing a random walk from every node********/
    double start = omp_get_wtime();
    int seed,node;
    double coinToss;
    int everyNode, edge;
    #pragma omp parallel for shared(allNodes, numNodes) private(seed, node,edge, coinToss, i,j,k)
    for(everyNode = 0; everyNode < numNodes; everyNode++){ //
        seed = omp_get_thread_num();
//        printf("I am in rank %d with start node %d\n",seed, everyNode);
        //seed = rand();
        node = everyNode;
        //randomWalk(node,length, .5);
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
       
       for(i = 0; i < length; i++){
/*2*****Take one step until the length of the walk******/
/*3*****Flip a coin to move to next node****************/
//        printf("I am in rank %d",seed);
           coinToss = rand_r(&seed)%100;
           coinToss /= 100;
//        printf(" with cointoss  %lf\n",coinToss);
           if(coinToss < damp || allNodes[node].numEdges == 0){
               //Heads -> go to a random node in the graph
                seed *= node;
               node = rand_r(&seed)%numNodes;
 //               printf("node -> %d\n",node);
               if (node < 0) node *= -1;
           }
           else if(coinToss > damp){
               //Tails -> Go to a random 1 hop neighbor
              // node = goToRandomNeighbor(node, seed);
                seed *= node;
               edge = rand_r(&seed);
               edge = edge%allNodes[node].numEdges;
//                printf("edge -> %d\n",edge);
               node = allNodes[node].edges[edge];
           }
           //printf("The update occurs after this\n");
           #pragma omp atomic
           allNodes[node].numLinks = allNodes[node].numLinks+1;
           //printf("The update occured\n");
/*3*****Moved to a new node and incrementd LC***********/
           //get the current node's link count into links
/*4*****Adding the current node to the top5***********/
/*
          // #pragma omp critical
          //  {
           int ogNode = node;
           int links = allNodes[node].numLinks;
           for(j = 0; j < 5; j++){
           //For every verticie in the top5
               //if the current top5 slot is unfilled
                   //fill it
               if (top5[j] == node) j = 6;
               if (top5[j] == -1){
                   top5[j] = node;
                   j = 6;
               }else
               //if the current links are greater than the
               //top5 node's links
               if (links > allNodes[top5[j]].numLinks){
                   //get store the links of the old top5 node
                  links = allNodes[top5[j]].numLinks;
                   //store the old top5 node
                  int tempNode = top5[j];
                   //set the new top5 node
                  #pragma omp atomic write
                  top5[j] = node;
                   //store the old top5 node as the new node to enter
                  node = tempNode;
                  for (k = j+1; k<5;k++){
                   //for the rest of the list 
                       if (top5[k] == ogNode){
                           #pragma omp atomic write
                           top5[k] = node;
                           j = 6;
                       }
                      tempNode = top5[k];
                       #pragma omp atomic write
                       top5[k] = node;
                       node = tempNode;
                  }
                  j = 6;
                }

            }
            
            node = ogNode;
            //}
*/
/*4******Node has been added to top5**************/
/*2******Walk has been completed************************/
       //printf("[%d](%d){%d}%s",node,allNodes[node].numLinks,allNodes[node].numEdges, (i+1)%5 == 0 ? "\n" : "->"); 
       }
    }
/*1******All random walks have been completed*********************/
    double end = omp_get_wtime();

    int totalLinks = 0;
    for(everyNode = 0; everyNode < numNodes; everyNode++){
        totalLinks += allNodes[everyNode].numLinks;
        //printf("Node [%d] -> links [%d] -> Edges [%d] -> total [%d]\n",everyNode,allNodes[everyNode].numLinks,allNodes[everyNode].numEdges,totalLinks);
    }
    printf("Total links -> %d\n",totalLinks);

    for(everyNode = 0; everyNode < numNodes; everyNode++){
//           printf("Node [%d] -> Links [%d]\n", everyNode, allNodes[everyNode].numLinks);
           node = everyNode;
           int ogNode = node;
           int links = allNodes[node].numLinks;
           for(j = 0; j < 5; j++){
           //For every verticie in the top5
               //if the current top5 slot is unfilled
                   //fill it
               if (top5[j] == node) j = 6;
               if (top5[j] == -1){
                   top5[j] = node;
                   j = 6;
               }else
               //if the current links are greater than the
               //top5 node's links
               if (links > allNodes[top5[j]].numLinks){
                   //get store the links of the old top5 node
                  links = allNodes[top5[j]].numLinks;
                   //store the old top5 node
                  int tempNode = top5[j];
                   //set the new top5 node
                  //#pragma omp atomic write
                  top5[j] = node;
                   //store the old top5 node as the new node to enter
                  node = tempNode;
                  for (k = j+1; k<5;k++){
                   //for the rest of the list 
                       if (top5[k] == ogNode){
                           //#pragma omp atomic write
                           top5[k] = node;
                           j = 6;
                       }
                      tempNode = top5[k];
                       //#pragma omp atomic write
                       top5[k] = node;
                       node = tempNode;
                  }
                  j = 6;
                }
                

            }
            
            node = ogNode;
    }
    for(i=0;i<5;i++){
        printf("Node [%d] -> Links [%d] -> Edges[%d]\n", top5[i], allNodes[top5[i]].numLinks,allNodes[top5[i]].numEdges);

    }

    printf("Time for PageRank to be ran on %d verticies with walk length %d = %lf\n",numNodes,length,end-start); 
}

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
    allNodes = (VERTEX *)malloc(sizeof(VERTEX)*numNodes);
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

int addEdgeToNode(VERTEX *node, int edge){
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

int goToRandomNeighbor(int node, int seed){
//    printf("Hop from [%d] which has %d edges\n", node, allNodes[node].numEdges);
    if(allNodes[node].numEdges < 1) return -1;

    int edge = rand_r(&seed);
    edge = edge%allNodes[node].numEdges;
    //int edge = 1;
    return goToOneHopNeighbor(node,edge);
}



