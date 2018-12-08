#include <stdio.h>
#include <stdlib.h>



int main(){
    int top5[5] = {-1,-1,-1,-1,-1};
    int inserting[10] = {10,2,5,8,7,3,8,2,9,4};
    int i,j,k,p;
    for(i = 0; i < 10; i++){
/*
    for(p = 0; p< 5; p++)
        printf("[%d] ",top5[p]);
    printf("\n");
*/

    for(j = 0; j < 5; j++){
        //printf("Inserting i -> %d top5 j -> %d\n", inserting[i],top5[j]);
        if (top5[j] == -1){
             top5[j] = inserting[i];
             j = 10;
        }
        else
         if (inserting[i] > top5[j]){
            int temp = top5[j];
            top5[j] = inserting[i];
            for(k = j; k+1 < 5; k++){
                if(temp > top5[k+1]){
                    int temp2 = top5[k+1];
                    top5[k+1] = temp;
                    temp = temp2; 
                }
            }
            j = 10;
            }
        
    }
    }

    
    for(i = 0; i< 5; i++)
        printf("[%d] ",top5[i]);
    printf("\n");


}








