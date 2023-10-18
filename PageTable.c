#include "Structures.h"


int Empty_List(const List_InfoNode *List){       

	return (List == NULL);
}

List_InfoNode* Initialise_List(){

    List_Node*  head;
    head = malloc(sizeof(List_Node));
    head->next = NULL;

    List_InfoNode* List;
    List = malloc(sizeof(List_InfoNode));
    List->first = head; 
    
    return List;
}


int Insert_at_PageTable(List_InfoNode** PT, Stack_InfoNode** structure, Trace_Node trace, int lengthPT,
 Frames_Node* fnode, Counters* count, int process_id, int alg){

    short available_frames = 0;
    int free_frame = -1;
        List_Node *node = NULL; 
    if ((free_frame = SearchForAvailableFrame(*fnode)) >= 0){ //number of free frame
        available_frames = 1;
    }
 
    if(SearchForPagePT(*PT, trace.address)){
        if (alg == 1){   //LRU
            RenewStack(*structure, trace.address, process_id);    //Stack
        }
        else if(alg == 2){    //Second Chance
            RenewQueue(*structure, trace.address, process_id);   //Queue
        }
        return -2;    //page already in Pagetable 
    }
    if (available_frames){
        if (Empty_List(*PT)){
            *PT = Initialise_List();
            (*PT)->first->page_number = trace.address;
            (*PT)->first->mode = trace.mode;
            (*PT)->first->next = NULL;
            (*PT)->first->frame_number = free_frame;
            fnode->frames[free_frame] = trace.address;
            node = (*PT)->first;
            count->PageFaults++;
            count->reads++;    
        }
        else{
            List_Node* Last = NULL;
            node = malloc(sizeof(List_Node));
            if (node == NULL){
                printf("Error trying inserting new node to the list");
                exit(EXIT_FAILURE);
            }
            if ((Last = LIST_last(*PT)) == NULL){   //find the last element in the list
                printf("Error trying getting the last node of the list in a Page Table entry");
                exit(EXIT_FAILURE);
            }
            node->page_number = trace.address;
            node->mode = trace.mode;
            node->next = NULL;
            node->frame_number = free_frame;
            fnode->frames[free_frame] = trace.address;   //frame not empty now
            Last->next = node;   //new node comes after the last one
            count->PageFaults++;
            count->reads++;
        }
    }
    else if(!available_frames){
        //CALL REPLACEMENT ALGORITHM AFTER RETURN 
        return -1;           //call replacement algorithm
    }
    if (alg == 1){   //LRU
        Insert_at_Stack(structure, node, process_id);  //the page will be now inserted in the stack
    }
    else if(alg == 2){    //Second Chance
        Insert_at_Queue(structure, node, process_id);  //the page will be now inserted in the queue
    }

    return free_frame;          //new page in Pagetable and in a new frame
}


List_Node* LIST_last(List_InfoNode*  List){     //find the last element in a list of the pagetable

	if(Empty_List(List))           
		return NULL; 

    else{
        List_Node* curr_ptr = List->first;
        while (curr_ptr->next != NULL){
            curr_ptr = curr_ptr->next;
        }
        return (curr_ptr);
    }	
}

int SearchForPagePT(List_InfoNode* const List, int page_num){     

	//searxh a page number in a list in a bucket of the hash table
	if (!Empty_List(List)){
        List_Node* curr_ptr = List->first;
        while (curr_ptr != NULL){
            if (curr_ptr->page_number == page_num)
                return 1;
            else
                curr_ptr = curr_ptr->next;
            
        }
    }
    return 0;
}


int DeleteFromPageTable(List_InfoNode** PageTable, int page_num, Counters *count){
//deletes the page with page number = page_num from the pagetable
    int frame = -1;
    List_Node *prev_ptr = NULL, *node = NULL, *temp = NULL;
    if(Empty_List(*PageTable)){
        return frame;
    }

    node = (*PageTable)->first;
    while(node != NULL){
        if (node->page_number == page_num){
            break;
        }
        else{ 
            node = node->next;
        }
    }

    temp = node;
    frame = node->frame_number;  //frame number of the page which will be deleted
    if (node->mode == 'W'){
        count->writes++;    //page has been modified while being in main memory, write it back
    }
    if ((*PageTable)->first == node){
        node = node->next;
        (*PageTable)->first = node;
    }
    else{
        prev_ptr = (*PageTable)->first;
        while(prev_ptr->next != node){
            prev_ptr = prev_ptr->next;
        }
        prev_ptr->next = node->next;
    }

    free(temp);
    if((*PageTable)->first == NULL){
        free(*PageTable);
        *PageTable = NULL;
    }
        
    return frame;
}
   

void Destroy_PageTable(List_InfoNode **PageTable, int lengthPT){    //free memory and nodes
    
    for (int i = 0; i < lengthPT; i++){
        List_Node *temp1, *temp2;      
        if (!Empty_List(*(PageTable+i))){
            temp1 = (*(PageTable+i))->first;

            while(temp1 != NULL){
                temp2 = temp1;
                temp1 = temp1->next;
                free(temp2);
            }
            (*(PageTable+i))->first = NULL;
            free((*(PageTable+i)));
        }
    }
}
 

void print_PageTable(List_InfoNode** Page_Table, int LENGTH){  //print pagetable unnecessary
    for (int i = 0; i < LENGTH; i++){
        if(!Empty_List(Page_Table[i])){
            printf("\ti = %d\n", i);
            print_List(Page_Table[i]);
        }
    }
}

void print_List(const List_InfoNode* List){   
      
	List_Node* temp;
	temp = List->first;	
	if(Empty_List(List)){
        printf("The list is empty.");
		return;
	}
	printf("%d\n", temp->page_number);
    printf("FIRST: %p \n", List->first);
    while(temp != NULL){
        printf("%p\n", temp);
        temp = temp->next;
    }
}