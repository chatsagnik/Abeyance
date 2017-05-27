#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stack.h"

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

int traversedArray[500];
float latencyArray[SIZE];
int latencyIndex = 0;
struct state* adjacencyList[SIZE];

int presentInStack[SIZE];
int noOfVertices = 0;
int hashTable[500];

void initializeCollisionVector(struct collisionVector* c, int col);
struct collisionVector createInitialCollisionVector(int reservationTable[ROW][COL],int row,int col);
struct collisionVector* createNewCollisionVector(struct collisionVector* c1,struct collisionVector* c2, int latency);
struct state* createNewState(struct collisionVector* c);
void displayState(struct state* st);
void populateDiagram(struct state* firstState, struct collisionVector* initialVector);
void initializeTraversedArray(void);



void initializeTraversedArray()
{
//    initializeLatencyArray();
	int i;
	for(i = 0; i < 500; i++){
		traversedArray[i] = 0;
	}
}

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
		newState->latency[i] = -1;
	}
	return newState;
}

struct collisionVector* createNewCollisionVector(struct collisionVector* c1,struct collisionVector* c2, int latency)
{
	struct collisionVector* cv = (struct collisionVector*)malloc(sizeof(struct collisionVector));
	cv->length = c1->length;
 	initializeCollisionVector(cv,c1->length);
 	int i;
 	for(i = 0; i < c1->length-latency; i++){
 	    cv->arr[i] = c1->arr[i+latency];
 	}
 	for(i = 0; i < cv->length; i++){
 	    cv->arr[i] = cv->arr[i] || c2->arr[i];
 	}
 	return cv;
}
void displayState(struct state* st)
{
    int i=0;
    printf("%d\n",st->cv->length);
    for(i = st->cv->length-1; i>-1; i--){
	    printf("%d\t",st->cv->arr[i]);
	}
	printf("\n");
	/*printf("Value of Collision Vector is: %d\n",st->value);
	printf("Value of children are: \n");
	i=0;
	while(st->latency[i] != -1){
		printf("Latency of edge to this child node is: %d\n", st->latency[i]);
		printf("Value of Collision Vector of child node is: %d\n",st->next[i]->value);
		i++;
	}*/
}
void populateDiagram(struct state* firstState, struct collisionVector* initialVector)
{
    //printf("\nInside populateDiagram!\n \n");
    
    
	int i,j,stateIndex = 0;
	traversedArray[firstState->value-1] = 1;
	
	
	for(i = 0; i<initialVector->length; i++){
	    firstState->latency[i] = -1;
	}
	for(i = 0; i< firstState->cv->length; i++){
 	    if(firstState->cv->arr[i] == 0){
 	        struct collisionVector* c = createNewCollisionVector(firstState->cv, initialVector, i+1); // creating a collision vector using initial collision vector and the latency
 	        struct state* nextState = createNewState(c); // creating the next state using
 	        firstState->next[stateIndex] = nextState; // updating the next state
 	        firstState->latency[stateIndex] = i+1; // updating the latency
 	        stateIndex++;

 		}
 	}
 	adjacencyList[noOfVertices] = firstState;
 	noOfVertices++;
 	//displayState(firstState);
 	i=0;
	while(firstState->latency[i] != -1){
		if(traversedArray[firstState->next[i]->value - 1]){
		    i++;
		    continue;
		}
		populateDiagram(firstState->next[i], initialVector);
		i++;
	}
	return;
}


void populateHashTable()
{
	int i;
	for(i = 0; i<noOfVertices; i++){
		hashTable[adjacencyList[i]->value] = i;
		//printf("Hashtable for %d has value%d\n", adjacencyList[i]->value,i);
	}
}

void printCycle(struct Stack* stack, int stopValue, int originalValue)
{
	struct Stack* temp = createStack(stack->capacity);
	printf("Printing Cycle:\n%d \t",originalValue);
	while(!(isEmpty(stack))){
		int element = pop(stack);
		printf("%d \t",element);
		push(temp,element);
	}
	printf("\n");
	while(!(isEmpty(temp))){
		int element = pop(temp);
		
		push(stack,element);
	}
}

void findCycles(struct Stack* stack,int index, int originalValue)
{
	int i = index,j;

	push(stack, adjacencyList[i]->value);
	presentInStack[i] = 1;
	j = 0;
    //printf(" %d is pushed onto the stack\n",adjacencyList[i]->value);
    
    while(adjacencyList[i]->latency[j] != -1){
	    	
		//printf("A neighbour node of %d is %d, and its HashValue is %d. Present in stack: %d\n",adjacencyList[i]->value,adjacencyList[i]->next[j]->value, hashTable[adjacencyList[i]->next[j]->value],presentInStack[hashTable[adjacencyList[i]->next[j]->value]]);
		if(adjacencyList[i]->next[j]->value == originalValue){
			//printf("it is a cycle!\n");
			printCycle(stack,adjacencyList[i]->value,originalValue);
			j++;
		}
		else if(presentInStack[hashTable[adjacencyList[i]->next[j]->value]]){
			//printf("It is already present in the stack\n");
			j++;
			continue;
		}
		else{
			//printf("It has to be pushed onto the stack, and a dfs has to be conducted\n");
			
			//push(stack,adjacencyList[i]->next[j]->value);
			
			findCycles(stack,hashTable[adjacencyList[i]->next[j]->value],originalValue);
			
			j++;
		}
	}
	//printf("Problem after this line\n");
	//pop(stack);
	pop(stack);
	presentInStack[hashTable[adjacencyList[i]->value]] = 0;
	return;
}

void removeVertex(int vertexValue)
{
	//printf("Removing vertex %d\n",vertexValue);
	int i = 0,j;
	for(j = i+1; j<noOfVertices; j++){
		//printf("No problem with %d<noOfVertices %d\n",j,adjacencyList[j]->value);
		adjacencyList[i] = adjacencyList[j];
		i++;
	}
	//free(adjacencyList[j]);
	noOfVertices--;
	//Now removing the vertex from individual adjacency lists
	for(i = 0; i<noOfVertices; i++){
		j = 0;
		//printf("No problem with %d<noOfVertices %d\n",i,adjacencyList[i]->value);
 	    while(adjacencyList[i]->latency[j] != -1){
 	    	if(adjacencyList[i]->next[j]->value==vertexValue) 
 	    		adjacencyList[i]->latency[j] = -1;
			j++;
		}
	}
	
}

int main()
 {
 	int arr[ROW][COL], i, j, stateIndex;
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
	struct state* firstState = createNewState(&initialVector);
	
 	
 	struct state* nullState = (struct state*)malloc(sizeof(struct state));
	nullState->next[0] = firstState;
	nullState->latency[0] = 0;
	for(i = 0; i < initialVector.length-1; i++){
		nullState->latency[i+1] = -1;
	}
	nullState->value = 0;
 	initializeTraversedArray();
 	populateDiagram(firstState,&initialVector);
 	initializeTraversedArray();
 	populateHashTable();
 	
 	
 	
 	//struct Stack* blockedStack = createStack(100);
 	int index = 0;
 	int cycles = noOfVertices;
 	for(index = 0; index < cycles; index++){
 	//	printf("---------------Iteration--------------\n");
 	//	printf("Printing the current adjacencyList: \n");
 			for(i = 0;i < noOfVertices; i++){
 			//presentInStack[i] = 0;
 	    	j = 0;
 	  //  	printf("The current node is: %d\n",adjacencyList[i]->value);
 	    	while(adjacencyList[i]->latency[j] != -1){
		//		printf("%d,%d\n",adjacencyList[i]->next[j]->value,adjacencyList[i]->latency[j]);
				j++;
			}
 		}
 		//printf("\n");
 		struct Stack* stack = createStack(100);
 		findCycles(stack,0,adjacencyList[0]->value);
 		//printf("problem at 297\n");
 		removeVertex(adjacencyList[0]->value); // After removing the vertex, the whole array is shifted left so it always starts from 0.
 		//printf("problem at 299\n");
 		populateHashTable();
 		free(stack);
 		
 	}
 	/*printf("Hello\n");
 	struct graph* g = constructGraphFromList();
 	printf("%d\n",g->V);*/
 }
 /*
//  Resultant collision vector is 1	0	1	1	0	0	0	1
 */
