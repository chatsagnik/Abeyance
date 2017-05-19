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

int traversedArray[100000];
float latencyArray[SIZE];
int latencyIndex = 0;
float sumOfLatencyToReachNode[100000];
int noOfEdgesToReachNode[100000];

void initializeCollisionVector(struct collisionVector* c, int col);
struct collisionVector createInitialCollisionVector(int reservationTable[ROW][COL],int row,int col);
struct collisionVector* createNewCollisionVector(struct collisionVector* c1,struct collisionVector* c2, int latency);
struct state* createNewState(struct collisionVector* c);
void displayState(struct state* st);
void populateDiagram(struct state* firstState, struct collisionVector* initialVector);
void initializeTraversedArray(void);

void initializeLatencyArray()
{
    int i;
	for(i = 0; i < SIZE; i++){
		latencyArray[i] = -1;
		sumOfLatencyToReachNode[i] = 0.0;
		noOfEdgesToReachNode[i] = 0;
	}
}

void initializeTraversedArray()
{
    initializeLatencyArray();
	int i;
	for(i = 0; i < 100000; i++){
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

void dfs(struct state* curr_node , struct state* prev_node)
{
	traversedArray[curr_node->value] = 1;
	int i=0;
	while(curr_node->latency[i] != -1){
		// check for self loop
		if(!traversedArray[curr_node->next[i]->value])
		{
			sumOfLatencyToReachNode[curr_node->next[i]->value] = curr_node->latency[i]+sumOfLatencyToReachNode[curr_node->value];

			noOfEdgesToReachNode[curr_node->next[i]->value]  = noOfEdgesToReachNode[curr_node->value]+1;
            printf("inside non visited-------------\n%d %d %f %d %f\n",curr_node->value,curr_node->next[i]->value , sumOfLatencyToReachNode[curr_node->next[i]->value] , noOfEdgesToReachNode[curr_node->next[i]->value] , sumOfLatencyToReachNode[prev_node->value]);
            dfs(curr_node->next[i], curr_node);
		}
		else{
		    printf("\n-------------\n%d %d\n",curr_node->value,curr_node->next[i]->value);
		    float num = ((sumOfLatencyToReachNode[curr_node->value] + curr_node->latency[i]) - sumOfLatencyToReachNode[curr_node->next[i]->value]);
		    int deno = ((noOfEdgesToReachNode[curr_node->value] +1) - noOfEdgesToReachNode[curr_node->next[i]->value] );
		    float val = num/deno;
			printf("Value: %f %d %f\n",num, deno, val);
			latencyArray[latencyIndex++] = val;
		}
		i++;
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
 	dfs(firstState,nullState);
 	printf("Printing the latencies\n");
 	for(i = 0;i <= latencyIndex; i++){
 	    printf("%f\n",latencyArray[i]);
 	}
 	
 }
 /*
//  Resultant collision vector is 1	0	1	1	0	0	0	1
 */
