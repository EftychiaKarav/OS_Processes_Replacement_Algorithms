//KARAVANGELI EFTYCHIA - 1115201800062
#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FRAME_SIZE 4096

typedef struct List_Node        List_Node;
typedef struct List_InfoNode    List_InfoNode;
typedef struct Stack_Node       Stack_Node;
typedef struct Stack_InfoNode   Stack_InfoNode;
typedef struct Frames_Node      Frames_Node;
typedef struct Trace_Node       Trace_Node;
typedef struct Counters         Counters;

struct List_Node{
    int page_number;
    int frame_number;
    char mode;        //R, W
    List_Node* next;
};

struct List_InfoNode{
    List_Node* first;
};

struct Stack_Node{
    int page_number;
    int process_id;     //1 or 2
    int reference_bit;   //0 or 1 - only for Second Chance
    struct Stack_Node* next;
    struct Stack_Node* previous;   //only for LRU
};

struct Stack_InfoNode{
    Stack_Node* first;
    Stack_Node* last;
};

struct Frames_Node{
    int *frames;
    int total_frames;
};

struct Trace_Node{
    unsigned int address;   //the shifted address, or else page number
    unsigned int offset;
    char mode;
};


struct Counters{
    int PageFaults;
    int reads;
    int writes;
};



//methods for PageTable
List_InfoNode* Initialise_List();
int Insert_at_PageTable(List_InfoNode** PageTable, Stack_InfoNode** Main_Memory, Trace_Node trace,
 int lengthPT, Frames_Node* fnode, Counters* count, int process_id, int alg);
void Destroy_PageTable(List_InfoNode **List, int lengthPT);  
int DeleteFromPageTable(List_InfoNode** PageTable, int page_num, Counters *count);
int Empty_List(const List_InfoNode *List);
void print_PageTable(List_InfoNode** Page_Table, int LENGTH);
void print_List(const List_InfoNode* List);   
List_Node* LIST_last(List_InfoNode*  List);  
int SearchForPagePT(List_InfoNode* const List, int page_num); 

//methods for Stack for LRU
Stack_InfoNode* Initialise_Stack();
void Insert_at_Stack(Stack_InfoNode** Stack, List_Node* page, int process_id);
void Destroy_Stack(Stack_InfoNode** Stack);
int DeleteFromStack(Stack_InfoNode** Stack, int* process_id);
int Empty_Stack(const Stack_InfoNode *Stack);
void RenewStack(Stack_InfoNode* Stack, int page_num, int process_id); 
int SearchForPageST(Stack_InfoNode* const Stack, int page_num); 
void print_Stack(Stack_InfoNode* Stack);

//methods for Queue for Second Chance
Stack_InfoNode* Initialise_Queue();
void Insert_at_Queue(Stack_InfoNode** Queue, List_Node* page, int process_id);
void Destroy_Queue(Stack_InfoNode** Queue);
int DeleteFromQueue(Stack_InfoNode** Queue, int* process_id);
int Empty_Queue(const Stack_InfoNode *Queue);
void RenewQueue(Stack_InfoNode* Queue, int page_num, int process_id);  //change reference bit from 0 to 1, if page found in pagetable

//other methods for opening the trace files, executing the simulation and the two algorithms 
FILE* open_file(char* filename);
Trace_Node read_traces(FILE *fp);
void Initialise_Frames(Frames_Node* fnode);
void Initialise_Counters(Counters* Count);
int SearchForAvailableFrame(Frames_Node fnode);
void LRU(List_InfoNode** PT1, List_InfoNode** PT2, Stack_InfoNode** Stack, int length, Frames_Node* fnode, Counters* count, int process_id);
void SecondChance(List_InfoNode** PT1, List_InfoNode** PT2, Stack_InfoNode** Queue, int LENGTH, Frames_Node* fnode, Counters* count, int process_id);
int execute(FILE* fp, List_InfoNode** PageTable1, List_InfoNode** PageTable2, Stack_InfoNode** Main_Memory, int* MEMORY,
 Frames_Node* FRAMES, Counters* count, int *total_references, int max_references, int q, int algorithm, int LENGTH, int process_id);
void Statistics(Counters, Frames_Node, int references, int alg, int q);


#endif