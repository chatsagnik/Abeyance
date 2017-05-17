#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROW 100
#define COL 100

struct collisionVector{
	int arr[COL];
	int length;
};

void initialize(struct collisionVector* c, int col)
{
	int i;
	for(i = 0; i < col; i++){
		c->arr[i] = 0;
	}
}

 struct collisionVector createCollisionVector(int reservationTable[ROW][COL],int row,int col)
 {
 	struct collisionVector cv;
 	initialize(&cv,col);
 	// iterate over each row and find the forbidden latencies
 	int i,j,k,l,latency,max;
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
 	for(i = col-1; i > -1; i--){
 		if(cv.arr[i] == 1){
 			cv.length = i;
 			break;
 		}
 	}
 	return cv;
 }
/*
 int main()
 {
 	int arr[ROW][COL], i, j;
 	for(i = 0; i < 10; i++){
 		for(j = 0; j < 10; j++){
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

 	struct collisionVector c = createCollisionVector(arr, 10, 10);
 	for(i = c.length; i > -1; i--){
 		printf("%d\t",c.arr[i]);
 	}
 	printf("\n");

 }
 
//  Resultant collision vector is 1	0	1	1	0	0	0	1
 */
