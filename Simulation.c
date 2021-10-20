//KARAVANGELI EFTYCHIA - 1115201800062
#include "Structures.h"


FILE* open_file(char* filename){     //open the file with the traces

    FILE *fp;
    fp = fopen(filename, "r");
        if (fp == NULL){
            fprintf(stderr, "Error in opening gcc.trace file");
            exit(EXIT_FAILURE);
        }
    return fp;
}

Trace_Node read_traces(FILE *fp){
    
    Trace_Node trace;
    unsigned int address, temp;   //hex addresss
    char mode, line;    //R or W and line is "endl"

    if (fscanf(fp, "%x %c %c", &address, &mode, &line) == EOF){
        trace.address = -1;
        return trace;
    }    
    temp = address;
    temp = temp << 20;             //left shift 20 bit
    temp = temp >> 20;            //right shift 20 bit
    address = (address >> 12);    //right shift 12 bit
    trace.address = address;
    trace.offset = temp;
    trace.mode = mode;
    return trace;
}


void Initialise_Frames(Frames_Node* fnode){

    int total_frames = fnode->total_frames;
    fnode->frames = NULL;
    fnode->frames = malloc(total_frames*sizeof(int));
    for(int i = 0; i < total_frames; i++){   //make all frames available
        fnode->frames[i] = -1;
    }
}

void Initialise_Counters(Counters* Count){
    Count->PageFaults = 0;
    Count->reads = 0;
    Count->writes = 0;
}


int main(int argc, char**argv){

    int algorithm = 0, q = 0, max_references = 0;
    Frames_Node FRAMES;
    if (argc == 1 || argc != 5 ){
        printf("Wrong Input\nPlease enter in the command line the following:\n\n");
        printf("\tFor LRU: 1 OR For Second Chance: 2\n");
        printf("\tNumber of Frames for both processes\n");
        printf("\tNumber of references to  be read from each file");
        printf("\tMaximum number of References which will be read from both given files\n\n");
        exit(EXIT_FAILURE);
    }
    algorithm = atoi(argv[1]);
    if ((algorithm != 1) && (algorithm != 2)){
        printf("Wrong Input for the Replacement Algorithm.\n");
        printf("For LRU: 1 OR For Second Chance: 2\n\n");
        exit(EXIT_FAILURE);
    }
    FRAMES.total_frames = atoi(argv[2]);
    q = atoi(argv[3]);
    max_references = atoi(argv[4]);
    if (max_references < q){
        printf("Maximum number of References is less than the number of traces to be read from the files.\n");
        printf("Please enter a larger number.\n\n");
        exit(EXIT_FAILURE);
    }

    int LENGTH = FRAMES.total_frames, i = 0, total_references = 0;
    int fin1 = 0, fin2 = 0;
    int MEMORY[LENGTH];
    FILE *fp1 = NULL, *fp2 = NULL;
    List_InfoNode *Page_Table1[LENGTH];   //for 1st process
    List_InfoNode *Page_Table2[LENGTH];   //for 2nd process
    Stack_InfoNode *Stack = NULL;         //for LRU
    Stack_InfoNode *Queue = NULL;         //for Second Chance
    Counters count;
    fp1 = open_file("bzip.trace");
    fp2 = open_file("gcc.trace");
    Initialise_Frames(&FRAMES);
    Initialise_Counters(&count);
    for (i = 0; i < LENGTH; i++){ 
        MEMORY[i] = -1;
        Page_Table1[i] = NULL;
        Page_Table2[i] = NULL;    
    }
    while(total_references < max_references){

        if(algorithm == 1){     //LRU
            if (!fin1)
            fin1 = execute(fp1, Page_Table1, Page_Table2, &Stack, MEMORY, &FRAMES, &count,&total_references, max_references,q, algorithm, LENGTH, 1);
            if (!fin2)
            fin2 = execute(fp2, Page_Table1, Page_Table2, &Stack, MEMORY, &FRAMES, &count,&total_references, max_references,q, algorithm, LENGTH, 2);
        }
        else if(algorithm == 2){    //Second Chance
            if (!fin1)
            fin1 = execute(fp1, Page_Table1, Page_Table2, &Queue, MEMORY, &FRAMES, &count,&total_references, max_references,q, algorithm, LENGTH, 1);
            if (!fin2)
            fin2 = execute(fp2, Page_Table1, Page_Table2, &Queue, MEMORY, &FRAMES, &count,&total_references, max_references,q, algorithm, LENGTH, 2);
        }
        if (fin1 && fin2) break;   //in case that max_references > total references in both trace files

    }
    // for (int i =0; i< LENGTH; i++)   //print for main memory
    // printf("%d  %d\n", i, MEMORY[i]);

    Statistics(count, FRAMES, total_references, algorithm, q);        

    //print_Stack(Stack);
    //print_PageTable(Page_Table1, LENGTH);
    Destroy_PageTable(Page_Table1, LENGTH);
    Destroy_PageTable(Page_Table2, LENGTH);
    Destroy_Stack(&Stack);
    Destroy_Queue(&Queue);
    free(FRAMES.frames);
    fclose(fp1);
    fclose(fp2);
}