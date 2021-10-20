//KARAVANGELI EFTYCHIA - 1115201800062
#include "Structures.h"

/*METHODS FOR STACK: DOUBLE LINKED WITH HEAD & TAIL - NEEDED FOR LRU*/

int Empty_Stack(const Stack_InfoNode *Stack){       

	return (Stack == NULL);
}

Stack_InfoNode* Initialise_Stack(){

    Stack_Node*  head;
    head = malloc(sizeof(Stack_Node));
    head->next = NULL;

    Stack_InfoNode* Stack;
    Stack = malloc(sizeof(Stack_InfoNode));
    Stack->first = head;   //head
    Stack->last = head;    //tail
    
    return Stack;
}


void Insert_at_Stack(Stack_InfoNode** Stack, List_Node* node,  int process_id){  

    if(Empty_Stack(*Stack)){
        *Stack = Initialise_Stack();
        (*Stack)->first->page_number = node->page_number;
        (*Stack)->last->page_number = node->page_number;
        (*Stack)->first->process_id = process_id;
        (*Stack)->last->process_id = process_id;
        (*Stack)->first->next = NULL;
        (*Stack)->last->next = NULL;
        (*Stack)->first->previous = NULL;
        (*Stack)->last->previous = NULL;
    }
    else{
        Stack_Node* new_node = malloc(sizeof(Stack_Node));
        if (node == NULL){
            printf("Error trying inserting new node to the list");
            exit(EXIT_FAILURE);
        }
        new_node->page_number = node->page_number;
        new_node->process_id = process_id;
        new_node->previous = NULL;
        new_node->next = (*Stack)->first;
        (*Stack)->first->previous = new_node;    //new inserted node is the top of the stack each time
        (*Stack)->first = new_node;
    }

}

void RenewStack(Stack_InfoNode* Stack, int page_num, int process_id){
	
    Stack_Node* curr_ptr = NULL;
    Stack_Node* prev = NULL, *next = NULL;
    if ((Stack->first->page_number == page_num) && (Stack->first->process_id == process_id))  
        return;           //the page found at the pagetable is already at the top of the stack

    else if((Stack->last->page_number == page_num) && (Stack->last->process_id == process_id)){         
        //the page found at the pagetable goes at the top of the stack
        curr_ptr = Stack->last;
        Stack->last = Stack->last->previous;
        Stack->last->next = NULL;
        curr_ptr->previous = NULL;
        curr_ptr->next = Stack->first;
        Stack->first->previous = curr_ptr;
        Stack->first = curr_ptr;    
    }
    else{          //the page found at the pagetable goes at the top of the stack
        curr_ptr = Stack->first->next;
        while (curr_ptr != NULL){
            if ((curr_ptr->page_number == page_num) && (curr_ptr->process_id == process_id)){
                break;
            }
            else
                curr_ptr = curr_ptr->next;
        }
            prev = curr_ptr->previous; 
            next = curr_ptr->next;
            prev->next = curr_ptr->next;
            next->previous = curr_ptr->previous;
            curr_ptr->previous = NULL;
            curr_ptr->next = Stack->first;
            Stack->first->previous = curr_ptr;
            Stack->first = curr_ptr;
    }
    return ;
}


int DeleteFromStack(Stack_InfoNode** Stack, int* process_id){

    int page = -1;
    if(!Empty_Stack(*Stack)){
        Stack_Node *last = (*Stack)->last;
        page = (*Stack)->last->page_number;   //page at the bottom of the stack is deleted
        if (*process_id != (*Stack)->last->process_id){  //we keep the process whose page will be deleted
            *process_id = (*Stack)->last->process_id;
        }
        if ((*Stack)->first == (*Stack)->last){
            (*Stack)->first = NULL;
            (*Stack)->last = NULL;
        }
        else{
            (*Stack)->last = last->previous;
            (*Stack)->last->next = NULL;
            free(last);
        }
    }
    return page;  //return the deleted page so that we an find it at the pagetable and set its frame free
}


void Destroy_Stack(Stack_InfoNode** Stack){   //free all nodes and memory
           
    Stack_Node* temp1, *temp2;      
    if (!Empty_Stack(*Stack)){
        temp1 = (*Stack)->first;
        while(temp1 != NULL){
            temp2 = temp1;
            temp1 = temp1->next;
            free(temp2);
        }
        (*Stack)->first = NULL;
        (*Stack)->last = NULL;
        free(*Stack);
        *Stack = NULL;
    }

}


void print_Stack(Stack_InfoNode* Stack){   //prints the Stack, unnecessary
	
    Stack_Node* temp;
	temp = Stack->first;	
	if(Empty_Stack(Stack)){
        printf("The list is empty.");
		return;
	}
	printf("%d\n", temp->page_number);
    printf("FIRST: %p LAST: %p\n", Stack->first, Stack->last);
    while(temp != NULL){
        printf("%p\t%p\t%p\n", temp->previous, temp, temp->next);
        temp = temp->next;
    }

}


int SearchForPageST(Stack_InfoNode* const Stack, int page_num){  //find a page in the Stack
        
    if (!Empty_Stack(Stack)){
        Stack_Node* curr_ptr = Stack->first;
        while (curr_ptr != NULL){
            if (curr_ptr->page_number == page_num){
                return 1;
            }
            else{ 
                curr_ptr = curr_ptr->next;
            }
        }
    }
    return 0;
}

/*METHODS FOR QUEUE: CYCLIC LINKED WITH HEAD & TAIL - NEEDED FOR SECOND CHANCE*/

int Empty_Queue(const Stack_InfoNode *Queue){      

	return (Queue == NULL);
}


Stack_InfoNode* Initialise_Queue(){  //same as Stack

    Stack_Node*  head;
    head = malloc(sizeof(Stack_Node));
    head->next = NULL;

    Stack_InfoNode* Queue;
    Queue = malloc(sizeof(Stack_InfoNode));
    Queue->first = head;
    Queue->last = head;
    
    return Queue;
}


void Insert_at_Queue(Stack_InfoNode** Queue, List_Node* node,  int process_id){  

    if(Empty_Queue(*Queue)){
        *Queue = Initialise_Queue();
        (*Queue)->first->page_number = node->page_number;
        (*Queue)->last->page_number = node->page_number;
        (*Queue)->first->process_id = process_id;
        (*Queue)->last->process_id = process_id;
        (*Queue)->first->reference_bit = 1;    //ref bit set to 1
        (*Queue)->last->reference_bit = 1;
        (*Queue)->first->next = (*Queue)->first;
        (*Queue)->last->next = (*Queue)->first;
    }
    else{
        Stack_Node* new_node = malloc(sizeof(Stack_Node));
        if (node == NULL){
            printf("Error trying inserting new node to the list");
            exit(EXIT_FAILURE);
        }
        new_node->page_number = node->page_number;
        new_node->process_id = process_id;
        new_node->reference_bit = 1;
        new_node->next = (*Queue)->first;
        (*Queue)->last->next = new_node;    //new node at the end of the queue as the most newly arrived
        (*Queue)->last = new_node;
    }

}

void RenewQueue(Stack_InfoNode* Queue, int page_num, int process_id){
	
    if (!Empty_Queue(Queue)){
        Stack_Node* curr_ptr = Queue->first;
        do{   //find the page, which was found in the pagetable for process (1 or 2), in the queue 
            //and set reference bit to 1, give the page a second chance in other words 
            if ((curr_ptr->page_number == page_num) && (curr_ptr->process_id == process_id)){
                curr_ptr->reference_bit = 1;
            }
            curr_ptr = curr_ptr->next;
            
        }while(curr_ptr != Queue->first);
    }
    return;
}


void Destroy_Queue(Stack_InfoNode** Queue){    //free nodes and memory
           
    Stack_Node* temp1, *temp2;      
    if (!Empty_Queue(*Queue)){
        temp1 = (*Queue)->first->next;
        while(temp1 != (*Queue)->last){
            temp2 = temp1;
            temp1 = temp1->next;
            free(temp2);
        }
        free((*Queue)->first);
        free((*Queue)->last);
        (*Queue)->first = NULL;
        (*Queue)->last = NULL;
        free(*Queue);
        *Queue = NULL;
    }
}


int DeleteFromQueue(Stack_InfoNode** Queue, int* process_id){

    int page = -1;
    if(!Empty_Queue(*Queue)){
        Stack_Node *curr_ptr = (*Queue)->first;  //start of the queue
        while(curr_ptr->reference_bit == 1){  //iterate until ref_bit = 0 is found
            curr_ptr->reference_bit = 0;
            (*Queue)->first = curr_ptr->next;
            (*Queue)->last = curr_ptr; 
            (*Queue)->last->next = (*Queue)->first; 
            curr_ptr = curr_ptr->next;    
        }
        if (*process_id != curr_ptr->process_id){
            *process_id = curr_ptr->process_id;   //we keep the process whose page will be deleted
        }
        page = curr_ptr->page_number;
        if ((*Queue)->first != (*Queue)->first->next){
            (*Queue)->first = (*Queue)->first->next;
            (*Queue)->last->next = (*Queue)->first;
            free(curr_ptr);
            curr_ptr = NULL;
        }
        else{
            free(curr_ptr);
            curr_ptr = NULL;
            free(*Queue);
            *Queue = NULL;
        }
    }
    return page; //return the deleted page so that we an find it at the pagetable and set its frame free

}

void print_Queue(Stack_InfoNode* Queue){ //prints the queue, unnecessary
	
    Stack_Node* temp;
	temp = Queue->first;	
	if(Empty_Queue(Queue)){
        printf("The queue is empty.");
		return;
	}
	printf("%d\n", temp->page_number);
    printf("FIRST: %p LAST: %p NEXT_LAST %p\n", Queue->first, Queue->last, Queue->last->next);
    do{
        printf("%p\t%p\n", temp, temp->next);
        temp = temp->next;
    }while(temp != Queue->first);

}

int SearchForPageQueue(Stack_InfoNode* const Queue, int page_num){  //search a page in the queue
        
    if (!Empty_Queue(Queue)){
        Stack_Node* curr_ptr = Queue->first;
        do{
            if (curr_ptr->page_number == page_num){
                return 1;
            }
            else{ 
                curr_ptr = curr_ptr->next;
            }
        }while(curr_ptr != Queue->first);
    }
    return 0;
}