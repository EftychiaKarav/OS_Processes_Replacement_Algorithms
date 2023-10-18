#include "Structures.h"

//run the procedure
int execute(FILE* fp, List_InfoNode** PT1, List_InfoNode** PT2, Stack_InfoNode** structure, int* MEMORY,
Frames_Node* FRAMES, Counters* count, int *total_references, int max_references, int q, int alg, int LENGTH, int process_id){
    
    int i = 0, hash_value = -1, allocated_frame = -1;
    Trace_Node trace;
    while((i < q) && (*total_references < max_references)){
        trace = read_traces(fp);
        if (trace.address != -1){ 
            hash_value = trace.address % LENGTH;   //hash function
            if(process_id == 1){
                if ((allocated_frame = Insert_at_PageTable(&PT1[hash_value], structure, trace, LENGTH, FRAMES, count, process_id, alg)) == -1){
                    if(alg == 1){
                        LRU(PT1, PT2, structure, LENGTH,FRAMES, count, process_id);
                    }
                    else if (alg == 2){
                        SecondChance(PT1, PT2, structure, LENGTH,FRAMES, count, process_id);
                    }
                    allocated_frame = Insert_at_PageTable(&PT1[hash_value], structure, trace, LENGTH, FRAMES, count, process_id, alg);
                }
            }
            else if(process_id == 2){
                if ((allocated_frame = Insert_at_PageTable(&PT2[hash_value], structure, trace, LENGTH, FRAMES, count, process_id, alg)) == -1){
                    if(alg == 1){
                        LRU(PT1, PT2, structure, LENGTH,FRAMES, count, process_id);
                    }
                    else if (alg == 2){
                        SecondChance(PT1, PT2, structure, LENGTH,FRAMES, count, process_id);
                    }
                    allocated_frame = Insert_at_PageTable(&PT2[hash_value], structure,trace, LENGTH, FRAMES, count, process_id, alg);
                }
            }
            if(allocated_frame >= 0) //new page in pagetable
                MEMORY[allocated_frame] = FRAME_SIZE*allocated_frame + trace.offset;  //physical address in main memory
            i++;
            (*total_references)++;
        }
        else return 1;    //no more lines in the trace file - returned from EOF
    }
    return 0;
}

void LRU(List_InfoNode** PT1, List_InfoNode** PT2, Stack_InfoNode** Stack, int LENGTH, Frames_Node* fnode, 
Counters* count, int process_id){
    
    int free_frame = -1, deleted_Page = -1, hash_value = -1;
    int process = process_id; //keep the process_id, beacause the deleted page may no t be in the pagetable of the process who demands a page
    deleted_Page = DeleteFromStack(Stack, &process);
    hash_value = deleted_Page % LENGTH;   //bucket to search the deleted page in the pagetable
    if(process == 1){
        free_frame = DeleteFromPageTable(&PT1[hash_value], deleted_Page, count);
    }
    else if(process == 2){
        free_frame = DeleteFromPageTable(&PT2[hash_value], deleted_Page, count);
    }
    fnode->frames[free_frame] = -1;  //free the returned frame

}
void SecondChance(List_InfoNode** PT1, List_InfoNode** PT2, Stack_InfoNode** Queue, int LENGTH, 
Frames_Node* fnode, Counters* count, int process_id){

    int free_frame = -1, deleted_Page = -1, hash_value = -1;
    int process = process_id; //keep the process_id, beacause the deleted page may no t be in the pagetable of the process who demands a page
    deleted_Page = DeleteFromQueue(Queue, &process);
    hash_value = deleted_Page % LENGTH;  //bucket to search the deleted page in the pagetable
    if(process == 1){
        free_frame = DeleteFromPageTable(&PT1[hash_value], deleted_Page, count);
    }
    else if(process == 2){
        free_frame = DeleteFromPageTable(&PT2[hash_value], deleted_Page, count);
    }
    fnode->frames[free_frame] = -1;  //free the returned frame

}


int SearchForAvailableFrame(Frames_Node fnode){

    for(int i = 0; i < fnode.total_frames; i++){
        if (fnode.frames[i] < 0)   //when a value in the table is < 0: the frame is free
            return i;
    }
    return -1;
}



void Statistics(Counters count, Frames_Node fnode, int references, int alg, int q){
//print statistics
    if (alg == 1)
        printf("\n\n\t\tALGORITHM:\tLRU\n");
    else if (alg == 2)
        printf("\n\n\t\tALGORITHM:\tSECOND CHANCE\n");
    printf("TOTAL PAGEFAULTS: \t\t\t\t%d\n", count.PageFaults);
    printf("TOTAL READS: \t\t\t\t\t%d\n", count.reads);
    printf("TOTAL WRITES: \t\t\t\t\t%d\n", count.writes);
    printf("TOTAL NUMBER OF FRAMES: \t\t\t%d\n", fnode.total_frames);
    printf("TRACES READ BY EACH FILE EACH TIME (q): \t%d\n", q);
    printf("TOTAL NUMBER OF REFERENCES READ: \t\t%d\n\n\n", references);
}


