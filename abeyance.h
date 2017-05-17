#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROW 100
#define COL 100

struct collisionVector{
	int arr[COL];
};

void initialize(struct collisionVector* c)
{
	int i;
	for(i = 0; i < COL; i++){
		c->arr[i] = 0;
	}
}

 struct collisionVector createCollisionVector(int reservationTable[ROW][COL],int row,int col)
 {
 	struct collisionVector cv;
 	initialize(&cv);
 	// iterate over each row and find the forbidden latencies
 	int i,j,k,l,latency;
 	for(k = 0; k < col; k++){
 		latency = 0;
 		for(i = 0; i < row; i++){//check for every state
 			for(j = 0; j < col-1; j++){
 				if(reservationTable[i][j] == 1){ // i.e if the state has reserved some space in the reservation table
 					for(l = j+1; l < col; l++){
 						if(reservationTable[i][l] == 1){
 							latency = l - j;
 							if(!cv.arr[latency-1])
 								cv.arr[latency-1] = 1;
 						}
 					}	
 				}
 				
 			}
 		}
 	}
 	return cv;
 }
/*
 int main()
 {
 	int arr[ROW][COL], i, j;
 	for(i = 0; i < 5; i++){
 		for(j = 0; j < 9; j++){
 			arr[i][j] = 0;
 		}
 	}
 	arr[0][0] = 1;
 	arr[0][8] = 1;
 	arr[1][1] = 1;
 	arr[1][2] = 1;
 	arr[1][7] = 1;
 	arr[2][3] = 1;
 	arr[3][4] = 1;
 	arr[3][5] = 1;
 	arr[4][6] = 1;
 	arr[4][7] = 1;

 	struct collisionVector c = createCollisionVector(arr, 5, 9);
 	for(i = 8; i > -1; i--){
 		printf("%d\t",c.arr[i]);
 	}
 	printf("\n");

 }
 
 Resultant collision vector is 0	1	0	1	1	0	0	0	1
 */
