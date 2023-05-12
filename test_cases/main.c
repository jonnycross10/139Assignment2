#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define a struct for a queue node
typedef struct node {
    int data;
    struct node* next;
} Node;

// Define a struct for the queue
typedef struct {
    Node* front;
    Node* rear;
    int size;
} Queue;

// Function to create a new node
Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

// Function to create an empty queue
Queue* create_queue() {
    Queue* new_queue = (Queue*)malloc(sizeof(Queue));
    new_queue->front = NULL;
    new_queue->rear = NULL;
    new_queue->size = 0;
    return new_queue;
}

// Function to check if the queue is empty
int is_empty(Queue* queue) {
    return (queue->front == NULL);
}

// Function to add an element to the queue
void enqueue(Queue* queue, int data) {
    Node* new_node = create_node(data);
    if (is_empty(queue)) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    queue->size++;
}

// Function to remove an element from the queue
int dequeue(Queue* queue) {
    if (is_empty(queue)) {
        printf("Error: Queue is empty.\n");
        return -1;
    }
    int data = queue->front->data;
    Node* temp = queue->front;
    queue->front = queue->front->next;
    free(temp);
    queue->size--;
    return data;
}

// Function to get the front element of the queue
int front(Queue* queue) {
    if (is_empty(queue)) {
        printf("Error: Queue is empty.\n");
        return -1;
    }
    return queue->front->data;
}

// Function to get the size of the queue
int size(Queue* queue) {
    return queue->size;
}

int main(int argc, char *argv[]) {
    if (argc != 2){
        printf("incorrect number of CLargs");
        return 1;
    }

    char FILENAME[50] = "input";
    char FILENUMBER[3];
    strcpy(FILENUMBER, argv[1]);
    strcat(FILENAME, FILENUMBER);
    strcat(FILENAME, ".txt");
    printf("processing input file: %s\n", FILENAME);

    FILE *file = fopen(FILENAME, "r");
    if (file ==NULL) {
        printf("Error opening file");
        return 1;
    }

    enum {RR, SJF, PR_noPREMP, PR_withPREMP} state;

    char line[50];
    //TODO may need to keep track of line number
    int lineNo = 0;
    int totalProcs = 0;
    int **procs;
    int quantumTime = 0;
    while (fgets(line, sizeof(line), file)){
        //line 1 
        if (lineNo == 0){
            //check first token to see what type of 
            char * tok = strtok(line, " ");
            int i =0;
            while(tok != NULL){
                //do operation
                if(i==0){
                    if (strcmp(tok, "RR") ==0){
                        state = RR;
                        printf("scheduling type is RR\n");
                    }
                    else if(strcmp(tok,"SJF")==0) {
                        state = SJF;
                        printf("scheduling type is SJF\n");
                    }
                    else if(strcmp(tok,"PR_noPREMP")==0){
                        state = PR_noPREMP;
                        printf("scheduling type is PR no Premp\n");
                    }
                    else if (strcmp(tok,"PR_withPREMP")==0){
                        state = PR_withPREMP;
                        printf("scheduling type is PR with Premp\n");
                    }
                }
                else if (i==1){
                    quantumTime = atoi(tok);
                }
                //set the next token
                tok = strtok(NULL, " ");
                i = i +1;
            }
            
        }
        else if (lineNo == 1){
            totalProcs = atoi(strtok(line, " "));
            //initialize proc array
            procs = (int**) malloc(totalProcs * sizeof(int *));
            // for (int i=0; i<totalProcs;i++){
            //     procs[i] = (int*) malloc(5 * sizeof(int));
            // }
            printf("totalprocs = %d\n", totalProcs);
        }
        


        else if(lineNo >1){
            procs[lineNo-2] = (int*) malloc(5 * sizeof(int));
            //loop through line 
            char *tok= strtok(line, " ");
            int i =0;
            while(tok != NULL){
                //do operation
                procs[lineNo-2][i] = atoi(tok);
                //set the next token
                tok = strtok(NULL, " ");
                i = i +1;
            }
            procs[lineNo-2][4] = 0;
        }
        lineNo = lineNo + 1;
    }
    // create the output depending on scheduling algorithm:
    // start time point, proc #
    Queue* queue = create_queue();
    
    
    if(state == RR){
        //add procs to queue
        for (int i=0; i<totalProcs;i++){
            enqueue(queue, procs[i][0]);
        }
        printf("size of queue: %d\n",size(queue));
        //loop until queue is empty
        int totalBurst =0;
        int currentBurst = 0;
        int activeProc = dequeue(queue);
        activeProc--;
        while(size(queue)>0||procs[activeProc][3]>0){
            //if its burst time is completed
            if(procs[activeProc][2] ==0){
                printf("proc %d is finished\n", activeProc+1);
                if(is_empty(queue)){
                    //done if last proc
                    break;
                }
                activeProc = dequeue(queue);
                activeProc--;
                currentBurst=0;
            }
            //if its quantum time is up
            else if (currentBurst>=quantumTime){
                //requeue process
                printf("requeuing proc %d\n", activeProc+1);
                enqueue(queue,activeProc+1);
                
                //set new active process
                activeProc = dequeue(queue);
                activeProc--;
                //reset the current burst
                currentBurst =0;
            }

            //decrement the process' burst time
            procs[activeProc][2]--;
            
            //increment the other process' wait time
            for(int i=0;i<totalProcs;i++){
                //if not the active process and is still in queue
                if(i!=activeProc && procs[i][2]>0){
                    procs[i][4]++;
                }
            }
            printf("current active process: %d\n", activeProc+1);
            currentBurst++;
            totalBurst++;
        }

    }
    else if(state == SJF){

    }
    else if(state == PR_noPREMP){

    }
    else if(state == PR_withPREMP){
        
    }
    printf("Finished\n");
    printf("Wait time for process 2 is: %d\n", procs[2][4]);
    fclose(file);
    return 0;
}