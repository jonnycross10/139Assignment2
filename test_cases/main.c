#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    while (fgets(line, sizeof(line), file)){
        //line 1 
        if (lineNo == 0){
            //check first token to see what type of 
            char * type = strtok(line, " ");
            if (strcmp(type, "RR")){
                state = RR;
                printf("state is %d\n", state);
            }
            else if(strcmp(type,"SJF")) {
                state = SJF;
            }
            else if(strcmp(type,"PR_noPREMP")){
                state = PR_noPREMP;
            }
            else if (strcmp(type,"PR_withPREMP")){
                state = PR_withPREMP;
            }
        }
        else if (lineNo == 1){
            totalProcs = atoi(strtok(line, " "));
            //initialize proc array
            procs = (int**) malloc(totalProcs * sizeof(int *));
            for (int i=0; i<totalProcs;i++){
                procs[i] = (int*) malloc(4 * sizeof(int));
            }
            printf("totalprocs = %d\n", totalProcs);
        }


        else if(lineNo >1){
            //loop through line 
            procs[totalProcs][4];
            char *tok= strtok(line, " ");
            int i =0;
            while(tok != NULL){
                //do operation
                procs[lineNo-2][i] = atoi(tok);
                //set the next token
                tok = strtok(NULL, " ");
                i = i +1;
            }
            
        }

        lineNo = lineNo + 1;
    }
    // for (int i=0; i<totalProcs;i++){
    //     printf("proc number: %d\n", procs[i][3]);
    // }
    // create the output depending on scheduling algorithm:
    // start time point, proc #
    if(state == RR){
        //implement Round Robin
    }
    else if(state == SJF){

    }
    else if(state == PR_noPREMP){

    }
    else if(state == PR_withPREMP){
        
    }

    fclose(file);
    return 0;
}