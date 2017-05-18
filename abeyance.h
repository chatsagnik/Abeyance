#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROW 100
#define COL 100
#define SIZE 100

struct collisionVector{
	int arr[COL];
	int length;
};

struct state{ // Each state has its own collision vector, and an array of next states. In a One-One mapping each of these have a latency associated with it.
	int value;
	struct collisionVector* cv;
	struct state* next[SIZE];
	int latency[SIZE];
};

void initializeCollisionVector(struct collisionVector* c, int col)
{
	int i;
	for(i = 0; i < col; i++){
		c->arr[i] = 0;
	}
}


 struct collisionVector createInitialCollisionVector(int reservationTable[ROW][COL],int row,int col)
 {
 	struct collisionVector cv;
 	initializeCollisionVector(&cv,col);
 	// iterate over each row and find the forbidden latencies
 	int i,j,k,l,latency,max;
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
 	
 	for(i = col-1; i > -1; i--){
 		if(cv.arr[i] == 1){
 			cv.length = i+1;
 			break;
 		}
 	}
 	return cv;
 }

struct state* createNewState(struct collisionVector* c)
{
	struct state* newState = (struct state*)malloc(sizeof(struct state));
	newState->cv = c;
	newState->value = 0;
	int i;
	for(i = 0; i < c->length; i++){
		int mult = (int)pow(2,i);
		newState->value += c->arr[i] * mult;
	}
	return newState;
}

struct collisionVector createNewCollisionVector(struct collisionVector* c1,struct collisionVector* c2, int latency)
{
	struct collisionVector cv;
	cv.length = c1->length;
 	initializeCollisionVector(&cv,c1->length);
 	int i;
 	for(i = 0; i < c1->length-latency; i++){
 	    cv.arr[i] = c1->arr[i+latency];
 	}
 	for(i = 0; i < cv.length; i++){
 	    cv.arr[i] = cv.arr[i] || c2->arr[i];
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

 	struct collisionVector initialVector = createInitialCollisionVector(arr,10,10);
	for(i = c.length; i > -1; i--){
 		printf("%d\t",c.arr[i]);
 	}
	print("\n");
	
 	struct state* newState = createNewState(&initialVector);
 	printf("%d\n",newState->value);
 	
 	for(i = 0; i< newState->cv->length; i++){
 	    if(newState->cv->arr[i] == 0){
 	        struct collisionVector c = createNewCollisionVector(newState->cv, &initialVector, i+1);
 	        for(j = c.length-1; j>-1; j--){
 	            printf("%d\t",c.arr[j]);
 	        }
 	        printf("\n");
 	    }
 	}
 	printf("\n");

 }
 /*
//  Resultant collision vector is 1	0	1	1	0	0	0	1
 */
