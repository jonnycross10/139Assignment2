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

// function to compare integers for qsort()
int compare(const void *a, const void *b) {
    int *x = (int *)a;
    int *y = (int *)b;
    return *x - *y;
}

void remove_row(int arr[][2], int num_rows, int row_index) {
    // Shift all rows after the one to be removed up by one
    for (int i = row_index; i < num_rows- 1; i++) {
        for (int j = 0; j < 3; j++) {
            arr[i][j] = arr[i+1][j];
        }
    }
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
            printf("type: %s", tok);
            while(tok != NULL){
                //do operation
                if(i==0){
                    if (strcmp(tok, "RR") ==0){
                        state = RR;
                        printf("scheduling type is RR\n");
                    }
                    else if(strcmp(tok,"SJF\n")==0) {
                        state = SJF;
                        printf("scheduling type is SJF\n");
                    }
                    else if(strcmp(tok,"PR_noPREMP\n")==0){
                        state = PR_noPREMP;
                        printf("scheduling type is PR no Premp\n");
                    }
                    else if (strcmp(tok,"PR_withPREMP\n")==0){
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
    
    FILE *fp;
    char OUTNAME[50] = "output";
    strcat(OUTNAME, FILENUMBER);
    strcat(OUTNAME, ".txt");
    fp = fopen(OUTNAME, "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
    
    if(state == RR){
        char *firstLine = "RR";
        fprintf(fp, "%s %d\n",firstLine, quantumTime);
        //add procs to queue
        for (int i=0; i<totalProcs;i++){
            enqueue(queue, procs[i][0]);
        }
        printf("size of queue: %d\n",size(queue));
        //loop until queue is empty
        int output[1][1];
        int totalBurst =0;
        int currentBurst = 0;
        int activeProc = dequeue(queue);
        activeProc--;
        fprintf(fp, "%d %d\n",totalBurst,activeProc+1);
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
                fprintf(fp, "%d %d\n",totalBurst,activeProc+1);
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
                fprintf(fp, "%d %d\n",totalBurst,activeProc+1);
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
        //calculate average wait time
        int sum=0;
        for(int i=0; i<totalProcs;i++){
            sum = sum + procs[i][4];
        }
        int average = sum / totalProcs;
        fprintf(fp, "AVG waiting time is %d\n",average);

    }
    else if(state == SJF){
        char *firstLine = "SJF";
        fprintf(fp, "%s %d\n",firstLine, quantumTime);
        int activeProc = 0;
        int time =0;
        int doneCount =0;
        int currentProcs[totalProcs][2];
        //initialize current proc array
        for(int i=0;i<totalProcs;i++){
            currentProcs[i][0] = 999;
            currentProcs[i][1] = 999;
        }
        int currProcCount =0;
        while (doneCount<totalProcs){
            //loop through procs and check for new
            for(int i=0;i<totalProcs;i++){
                if(time==procs[i][1]){
                    currentProcs[i][0] = procs[i][0]; //adding proc number
                    currentProcs[i][1] = procs[i][2]; //adding burst time
                    currProcCount++;
                    printf("Adding process %d\n",procs[i][0]);
                }
            }
            //sort current procs based on burst time
            int arrLen = sizeof(currentProcs) / sizeof(currentProcs[0]);
            int i, j, temp;
            for(i=0; i<arrLen; i++) {
                for(j=i+1; j<arrLen; j++) {
                    if(currentProcs[i][1] > currentProcs[j][1]) {
                        temp = currentProcs[i][0];
                        currentProcs[i][0] = currentProcs[j][0];
                        currentProcs[j][0] = temp;

                        temp = currentProcs[i][1];
                        currentProcs[i][1] = currentProcs[j][1];
                        currentProcs[j][1] = temp;
                    }
                }
            }
            if(time==0){
                //if its the first iteration set the active to sj
                activeProc = currentProcs[0][0]-1;
                printf("setting active process to: %d\n", activeProc+1);
                fprintf(fp, "%d %d\n",time,activeProc+1);
            } 
            //printf("last procId is %d\n", currentProcs[3][0]);
            // check for a finished process
            if(procs[activeProc][2]==0){
                //remove first row 
                arrLen = sizeof(currentProcs) / sizeof(currentProcs[0]);
                //set the current process'burst time to 99 but it could have been sorted
                for(int i=0; i<totalProcs;i++){
                    if(currentProcs[i][0]==procs[activeProc][0]){
                        currentProcs[i][1] = 99;
                    }
                }
                int i, j, temp;
                for(i=0; i<arrLen; i++) {
                    for(j=i+1; j<arrLen; j++) {
                        if(currentProcs[i][1] > currentProcs[j][1]) {
                            temp = currentProcs[i][0];
                            currentProcs[i][0] = currentProcs[j][0];
                            currentProcs[j][0] = temp;

                            temp = currentProcs[i][1];
                            currentProcs[i][1] = currentProcs[j][1];
                            currentProcs[j][1] = temp;
                        }
                    }
                }
                //set new active process to first in current procs
                activeProc = currentProcs[0][0]-1; 
                printf("setting active process to: %d\n", activeProc+1);
                //increment done
                doneCount++;
                if(doneCount==totalProcs){
                    break;
                }
                fprintf(fp, "%d %d\n",time,activeProc+1);
            }
            //decrement process' burst
            procs[activeProc][2]--;
            printf("current active process: %d\n", activeProc+1);
            //increment other process' wait time
            arrLen = sizeof(currentProcs) / sizeof(currentProcs[0]);
            for(int i=0;i<arrLen;i++){
                if(i != activeProc && procs[i][2]>0){
                    procs[i][4]++;
                }
            }
            time++;
        }
        //calculate average wait time
        int sum=0;
        for(int i=0; i<totalProcs;i++){
            sum = sum + procs[i][4];
        }
        float average = sum / (float)totalProcs;
        fprintf(fp, "AVG waiting time is %f\n",average);
    }
    else if(state == PR_noPREMP){
        char *firstLine = "PR_noPREMP";
        fprintf(fp, "%s %d\n",firstLine, quantumTime);
        int activeProc = 0;
        int time =0;
        int doneCount =0;
        int currentProcs[totalProcs][2];
        //initialize current proc array
        for(int i=0;i<totalProcs;i++){
            currentProcs[i][0] = 999;
            currentProcs[i][1] = 999;
        }
        int currProcCount =0;
        while (doneCount<totalProcs){
            //loop through procs and check for new
            for(int i=0;i<totalProcs;i++){
                if(time==procs[i][1]){
                    currentProcs[i][0] = procs[i][0]; //adding proc number
                    currentProcs[i][1] = procs[i][3]; //adding burst time
                    currProcCount++;
                    printf("Adding process %d\n",procs[i][0]);
                }
            }
            //sort current procs based on burst time
            int arrLen = sizeof(currentProcs) / sizeof(currentProcs[0]);
            int i, j, temp;
            for(i=0; i<arrLen; i++) {
                for(j=i+1; j<arrLen; j++) {
                    if(currentProcs[i][1] > currentProcs[j][1]) {
                        temp = currentProcs[i][0];
                        currentProcs[i][0] = currentProcs[j][0];
                        currentProcs[j][0] = temp;

                        temp = currentProcs[i][1];
                        currentProcs[i][1] = currentProcs[j][1];
                        currentProcs[j][1] = temp;
                    }
                }
            }
            if(time==0){
                //if its the first iteration set the active to sj
                activeProc = currentProcs[0][0]-1;
                printf("setting active process to: %d\n", activeProc+1);
                fprintf(fp, "%d %d\n",time,activeProc+1);
            } 
            //printf("last procId is %d\n", currentProcs[3][0]);
            // check for a finished process
            if(procs[activeProc][2]==0){
                //remove first row 
                arrLen = sizeof(currentProcs) / sizeof(currentProcs[0]);
                //set the current process'burst time to 99 but it could have been sorted
                for(int i=0; i<totalProcs;i++){
                    if(currentProcs[i][0]==procs[activeProc][0]){
                        currentProcs[i][1] = 99;
                    }
                }
                int i, j, temp;
                for(i=0; i<arrLen; i++) {
                    for(j=i+1; j<arrLen; j++) {
                        if(currentProcs[i][1] > currentProcs[j][1]) {
                            temp = currentProcs[i][0];
                            currentProcs[i][0] = currentProcs[j][0];
                            currentProcs[j][0] = temp;

                            temp = currentProcs[i][1];
                            currentProcs[i][1] = currentProcs[j][1];
                            currentProcs[j][1] = temp;
                        }
                    }
                }
                //set new active process to first in current procs
                activeProc = currentProcs[0][0]-1; 
                printf("setting active process to: %d\n", activeProc+1);
                //increment done
                doneCount++;
                if(doneCount==totalProcs){
                    break;
                }
                fprintf(fp, "%d %d\n",time,activeProc+1);
            }
            //decrement process' burst
            procs[activeProc][2]--;
            printf("current active process: %d\n", activeProc+1);
            //increment other process' wait time
            arrLen = sizeof(currentProcs) / sizeof(currentProcs[0]);
            for(int i=0;i<arrLen;i++){
                if(i != activeProc && procs[i][2]>0){
                    procs[i][4]++;
                }
            }
            time++;
        }
        //calculate average wait time
        int sum=0;
        for(int i=0; i<totalProcs;i++){
            sum = sum + procs[i][4];
        }
        float average = sum / (float)totalProcs;
        fprintf(fp, "AVG waiting time is %f\n",average);
    }
    else if(state == PR_withPREMP){
        char *firstLine = "PR_withPREMP";
        fprintf(fp, "%s %d\n",firstLine, quantumTime);
        int activeProc = 0;
        int time =0;
        int doneCount =0;
        int currentProcs[totalProcs][2];
        //initialize current proc array
        for(int i=0;i<totalProcs;i++){
            currentProcs[i][0] = 999;
            currentProcs[i][1] = 999;
        }
        int currProcCount =0;
        while (doneCount<totalProcs){
            //loop through procs and check for new
            for(int i=0;i<totalProcs;i++){
                if(time==procs[i][1]){
                    currentProcs[i][0] = procs[i][0]; //adding proc number
                    currentProcs[i][1] = procs[i][3]; //adding burst time
                    currProcCount++;
                    printf("Adding process %d\n",procs[i][0]);
                }
            }
            //sort current procs based on burst time
            int arrLen = sizeof(currentProcs) / sizeof(currentProcs[0]);
            int i, j, temp;
            for(i=0; i<arrLen; i++) {
                for(j=i+1; j<arrLen; j++) {
                    if(currentProcs[i][1] > currentProcs[j][1]) {
                        temp = currentProcs[i][0];
                        currentProcs[i][0] = currentProcs[j][0];
                        currentProcs[j][0] = temp;

                        temp = currentProcs[i][1];
                        currentProcs[i][1] = currentProcs[j][1];
                        currentProcs[j][1] = temp;
                    }
                }
            }
            if(time==0){
                //if its the first iteration set the active to sj
                activeProc = currentProcs[0][0]-1;
                printf("setting active process to: %d\n", activeProc+1);
                fprintf(fp, "%d %d\n",time,activeProc+1);
            } 
            //printf("last procId is %d\n", currentProcs[3][0]);
            
            arrLen = sizeof(currentProcs) / sizeof(currentProcs[0]);
            
            for(i=0; i<arrLen; i++) {
                for(j=i+1; j<arrLen; j++) {
                    if(currentProcs[i][1] > currentProcs[j][1]) {
                        temp = currentProcs[i][0];
                        currentProcs[i][0] = currentProcs[j][0];
                        currentProcs[j][0] = temp;

                        temp = currentProcs[i][1];
                        currentProcs[i][1] = currentProcs[j][1];
                        currentProcs[j][1] = temp;
                    }
                }
            }
            //set new active process to first in current procs
            int oldActive = activeProc;
            activeProc = currentProcs[0][0]-1; 
            printf("setting active process to: %d\n", activeProc+1);
            //if we changed procs, print it
            if(oldActive != activeProc){
                fprintf(fp, "%d %d\n",time,activeProc+1);
            }
            //decrement process' burst
            procs[activeProc][2]--;
            //increment done if active proc's burst is now 0
            if(procs[activeProc][2]==0){
                doneCount++;
                //set the current process'burst time to 99 but it could have been sorted
                for(int i=0; i<totalProcs;i++){
                    if(currentProcs[i][0]==procs[activeProc][0]){
                        currentProcs[i][1] = 99;
                    }
                }
            }
            if(doneCount==totalProcs){
                break;
            }

            printf("current active process: %d\n", activeProc+1);
            //increment other process' wait time
            arrLen = sizeof(currentProcs) / sizeof(currentProcs[0]);
            for(int i=0;i<arrLen;i++){
                if(i != activeProc && procs[i][2]>0){
                    procs[i][4]++;
                }
            }
            time++;
        }
        //calculate average wait time
        int sum=0;
        for(int i=0; i<totalProcs;i++){
            sum = sum + procs[i][4];
            printf("proc %d's wait time is %d\n",i, procs[i][4]);
        }
        float average = sum / (float)totalProcs;
        fprintf(fp, "AVG waiting time is %f\n",average);
    }
    printf("Finished\n");
    printf("Wait time for process 1 is: %d\n", procs[0][4]);
    fclose(file);
    return 0;
}